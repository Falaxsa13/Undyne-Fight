#include "main.h"
#include <stdio.h>
#include <stdlib.h>

#include "gba.h"
#include "gbaUI.h"

#include "images/character.h"
#include "images/background2.h"
#include "images/background1.h"

// start

#define REG_SOUNDCNT_X *(volatile u16 *)0x04000084
#define REG_SOUNDCNT_L *(volatile u16 *)0x04000080
#define REG_SOUNDCNT_H *(volatile u16 *)0x04000082

#define REG_SOUND1CNT_L *(volatile u16 *)0x04000060
#define REG_SOUND1CNT_H *(volatile u16 *)0x04000062
#define REG_SOUND1CNT_X *(volatile u16 *)0x04000064

// Sound control bits
#define SOUND1_ENABLE (1 << 12)
#define DSOUND_A_RIGHT_CHANNEL (1 << 8)
#define DSOUND_A_LEFT_CHANNEL (1 << 9)
#define DSOUND_A_FIFO_RESET (1 << 11)

typedef struct
{
  int frequency; // The frequency of the note
  int duration;  // How long to play the note, in frames
} Note;

// Example theme: Simple C major scale
Note theme[] = {
    {NOTE_A2, 20}, {NOTE_B2, 20}, {NOTE_C3, 20}, {NOTE_D3, 20}, {NOTE_E3, 20}, {NOTE_F3, 20}, {NOTE_G3, 20}, {NOTE_A3, 20}, {NOTE_B3, 20}, {NOTE_C4, 20}, {NOTE_D4, 20}, {NOTE_E4, 20}, {NOTE_F4, 20}, {NOTE_G4, 20}, {NOTE_A4, 20}, {NOTE_B4, 20}, {NOTE_C5, 20}, {NOTE_D5, 20}, {NOTE_E5, 20}, {NOTE_F5, 20}, {NOTE_G5, 20}, {NOTE_A5, 20}, {NOTE_B5, 20}, {NOTE_C6, 20}, {NOTE_D6, 20}, {NOTE_E6, 20}, {NOTE_F6, 20}, {NOTE_G6, 20}, {NOTE_A6, 20}, {NOTE_B6, 20}};

void play_note(Note note)
{
  if (note.frequency > 0)
  {
    int freq = 2048 - (131072 / note.frequency);
    REG_SOUND1CNT_L = 0x0070;
    REG_SOUND1CNT_H = 0xF780;
    REG_SOUND1CNT_X = freq | 0x8000;
  }
  else
  {
    // Silence or pause
    REG_SOUND1CNT_X = 0;
  }
}

void play_theme(Note *theme)
{
  static int currentNote = 0;
  static int noteTimer = 0;

  if (noteTimer <= 0)
  {
    play_note(theme[currentNote]);

    noteTimer = theme[currentNote].duration;

    currentNote++;

    if (theme[currentNote].frequency == 0)
    {
      currentNote = 0;
    }
  }
  else
  {
    // Decrement the timer until it's time for the next note
    noteTimer--;
  }
}

void sound_init(void)
{
  // Turn on the sound circuit
  REG_SOUNDCNT_X = 0x80;

  // Set sound output
  REG_SOUNDCNT_L = 0x1177;
  REG_SOUNDCNT_H = 0x0;
}

void play_sound(void)
{
  // Set the frequency (2048 - (131072 / frequency))
  // For Middle C (C4), the frequency is about 261.63 Hz
  int freq = 2048 - (131072 / 262);

  // Set the duty cycle (50%), volume (max), and attack/decay/sustain
  REG_SOUND1CNT_L = 0x0070;        // Duty cycle = 50%
  REG_SOUND1CNT_H = 0xF780;        // Volume = max, no envelope
  REG_SOUND1CNT_X = freq | 0x8000; // Set frequency and enable the channel
}

// end

typedef enum
{
  NORTH,
  SOUTH,
  EAST,
  WEST
} Direction;

typedef struct Bullet
{
  int row;
  int col;
  int width;
  int height;
  int active;
  int speed;
  u16 color;
  Direction direction;
} Bullet;

typedef struct
{
  int row;
  int col;
  int width;
  int height;
  int health;
  int maxHealth;
} Player;

typedef enum
{
  SHIELD_UP,
  SHIELD_DOWN,
  SHIELD_LEFT,
  SHIELD_RIGHT
} ShieldDirection;

typedef struct
{
  int row;
  int col;
  int size;
  ShieldDirection direction;
  int active;
} Shield;

typedef enum
{
  START,
  PLAY,
  WIN,
  LOSE,
} gba_state;

#define MAX_BULLETS 10
Bullet bullets[MAX_BULLETS];

void updateShield(Shield *shield, u32 currentButtons)
{
  if (KEY_DOWN(BUTTON_UP, currentButtons))
  {
    shield->direction = SHIELD_UP;
  }
  else if (KEY_DOWN(BUTTON_DOWN, currentButtons))
  {
    shield->direction = SHIELD_DOWN;
  }
  else if (KEY_DOWN(BUTTON_LEFT, currentButtons))
  {
    shield->direction = SHIELD_LEFT;
  }
  else if (KEY_DOWN(BUTTON_RIGHT, currentButtons))
  {
    shield->direction = SHIELD_RIGHT;
  }
}

void initBullets(void)
{
  for (int i = 0; i < MAX_BULLETS; i++)
  {
    bullets[i].active = 0;
    bullets[i].color = GREEN;
  }
}

void addBullet(Direction dir, int speed)
{
  // Adjusted bullet initialization logic
  for (int i = 0; i < MAX_BULLETS; i++)
  {
    if (!bullets[i].active)
    {
      bullets[i].active = 1;
      bullets[i].color = GREEN;
      bullets[i].speed = speed;
      bullets[i].width = 3;
      bullets[i].height = 3;
      bullets[i].direction = dir;

      // Set bullet's starting position inside the box, depending on direction
      switch (dir)
      {
      case NORTH:
        bullets[i].row = BOX_BOTTOM - bullets[i].height - 1;
        bullets[i].col = (BOX_LEFT + BOX_RIGHT) / 2;
        break;
      case SOUTH:
        bullets[i].row = BOX_TOP + 1;
        bullets[i].col = (BOX_LEFT + BOX_RIGHT) / 2;
        break;
      case EAST:
        bullets[i].row = (BOX_TOP + BOX_BOTTOM) / 2;
        bullets[i].col = BOX_LEFT + 1;
        break;
      case WEST:
        bullets[i].row = (BOX_TOP + BOX_BOTTOM) / 2;
        bullets[i].col = BOX_RIGHT - bullets[i].width - 1;
        break;
      }
      break;
    }
  }
}

void clearBullet(Bullet *bullet)
{
  if (bullet->active)
  {
    // Clear the bullet by drawing a black rectangle over its current position.
    drawRectDMA(bullet->row, bullet->col, bullet->width, bullet->height, BLACK);
  }
}

int checkShieldCollision(Shield *shield, Bullet *bullet)
{
  if (!bullet->active)
  {
    return 0; // No collision if the bullet is inactive
  }

  // Define the shield's bounds
  int shieldLeft, shieldRight, shieldTop, shieldBottom;

  switch (shield->direction)
  {
  case SHIELD_UP:
    shieldTop = shield->row - shield->size;
    shieldBottom = shield->row;
    shieldLeft = shield->col - shield->size / 2;
    shieldRight = shield->col + shield->size / 2;
    break;
  case SHIELD_DOWN:
    shieldTop = shield->row;
    shieldBottom = shield->row + shield->size;
    shieldLeft = shield->col - shield->size / 2;
    shieldRight = shield->col + shield->size / 2;
    break;
  case SHIELD_LEFT:
    shieldTop = shield->row - shield->size / 2;
    shieldBottom = shield->row + shield->size / 2;
    shieldLeft = shield->col - shield->size;
    shieldRight = shield->col;
    break;
  case SHIELD_RIGHT:
    shieldTop = shield->row - shield->size / 2;
    shieldBottom = shield->row + shield->size / 2;
    shieldLeft = shield->col;
    shieldRight = shield->col + shield->size;
    break;
  }

  // Check collision with the bullet
  if (bullet->row < shieldBottom &&
      bullet->row + bullet->height > shieldTop &&
      bullet->col < shieldRight &&
      bullet->col + bullet->width > shieldLeft)
  {
    return 1; // Collision detected
  }

  return 0; // No collision
}

void updateBullets(Shield *shield)
{
  for (int i = 0; i < MAX_BULLETS; i++)
  {
    if (bullets[i].active)
    {
      // Clear the current bullet before updating its position
      clearBullet(&bullets[i]);

      // Update the bullet's position based on its direction
      switch (bullets[i].direction)
      {
      case NORTH:
        bullets[i].row -= bullets[i].speed;
        break;
      case SOUTH:
        bullets[i].row += bullets[i].speed;
        break;
      case EAST:
        bullets[i].col += bullets[i].speed;
        break;
      case WEST:
        bullets[i].col -= bullets[i].speed;
        break;
      }

      if (checkShieldCollision(shield, &bullets[i]) && shield->active)
      {
        bullets[i].active = 0;
        clearBullet(&bullets[i]); // This will clear the bullet from the screen
      }
      else if (bullets[i].row < 0 || bullets[i].row > HEIGHT || bullets[i].col < 0 || bullets[i].col > WIDTH)
      {
        bullets[i].active = 0;
      }
      else
      {
        drawRectDMA(bullets[i].row, bullets[i].col, bullets[i].width, bullets[i].height, bullets[i].color);
      }
    }
  }
}

void drawBullets(void)
{
  for (int i = 0; i < MAX_BULLETS; i++)
  {
    if (bullets[i].active)
    {
      drawRectDMA(bullets[i].row, bullets[i].col, bullets[i].width, bullets[i].height, bullets[i].color);
    }
  }
}

void drawHealthBar(Player *p)
{
  // First, draw the full health bar in red
  drawRectDMA(HEALTH_BAR_Y, HEALTH_BAR_X, HEALTH_BAR_WIDTH, HEALTH_BAR_HEIGHT, HEALTH_BAR_LOW_COLOR);

  // Calculate the current width of the health bar based on the player's health
  int currentHealthWidth = (p->health * HEALTH_BAR_WIDTH) / p->maxHealth;

  // Then, overlay the current health in yellow
  drawRectDMA(HEALTH_BAR_Y, HEALTH_BAR_X, currentHealthWidth, HEALTH_BAR_HEIGHT, HEALTH_BAR_FULL_COLOR);
}

void updatePlayer(Player *p, u32 currentButtons)
{
  int speed = 1;

  if (KEY_DOWN(BUTTON_UP, currentButtons))
  {
    p->row -= speed;
    if (p->row < 0)
    {
      p->row = 0;
    }
  }
  if (KEY_DOWN(BUTTON_DOWN, currentButtons))
  {
    p->row += speed;
    if (p->row > HEIGHT - p->height)
    { // Keep player within the bottom boundary
      p->row = HEIGHT - p->height;
    }
  }
  if (KEY_DOWN(BUTTON_LEFT, currentButtons))
  {
    p->col -= speed;
    if (p->col < 0)
    { // Keep player within the left boundary
      p->col = 0;
    }
  }
  if (KEY_DOWN(BUTTON_RIGHT, currentButtons))
  {
    p->col += speed;
    if (p->col > WIDTH - p->width)
    { // Keep player within the right boundary
      p->col = WIDTH - p->width;
    }
  }

  if (p->row < BOX_TOP + 1)
  {
    p->row = BOX_TOP + 1;
  }
  else if (p->row > BOX_BOTTOM - p->height)
  {
    p->row = BOX_BOTTOM - p->height;
  }

  if (p->col < BOX_LEFT + 1)
  {
    p->col = BOX_LEFT + 1;
  }
  else if (p->col > BOX_RIGHT - p->width)
  {
    p->col = BOX_RIGHT - p->width;
  }
}

void clearPlayer(Player *p)
{
  drawRectDMA(p->row, p->col, p->width, p->height, BLACK);
}

int checkCollision(Player *p, Bullet *b)
{
  if (b->active)
  {
    if (p->row < b->row + b->height &&
        p->row + p->height > b->row &&
        p->col < b->col + b->width &&
        p->col + p->width > b->col)
    {
      return 1; // Collision detected
    }
  }
  return 0; // No collision
}

void updateHealth(Player *p, Shield *shield)
{
  for (int i = 0; i < MAX_BULLETS; i++)
  {
    if (checkCollision(p, &bullets[i]) && !checkShieldCollision(shield, &bullets[i]))
    {
      play_sound();
      p->health -= 10; // Decrease health by 10 for each collision not blocked by the shield

      // Clear the bullet from the screen and deactivate it
      clearBullet(&bullets[i]);
      bullets[i].active = 0; // Deactivate the bullet
    }
  }
}

ShieldDirection drawShield(Shield *shield, Player *player, ShieldDirection oldDirection)
{
  // Assuming the shield is a line that rotates around the player
  int length = shield->size; // The length of the shield line
  int x = shield->col - 2;
  int y = shield->row + 1;

  // First, clear the old shield position by drawing a black box over it

  switch (oldDirection)
  {
  case SHIELD_UP:
    drawRectDMA(y - 8, x, length, 3, BLACK); // Vertical line upwards
    break;
  case SHIELD_DOWN:
    drawRectDMA(y + 8, x, length, 3, BLACK); // Vertical line downwards
    break;
  case SHIELD_LEFT:
    drawRectDMA(y - 2, x - 5, 3, length, BLACK); // Horizontal line to the left
    break;
  case SHIELD_RIGHT:
    drawRectDMA(y - 2, x + 11, 3, length, BLACK); // Horizontal line to the right
    break;
  }

  drawImageDMA(player->row, player->col, player->width, player->height, character);

  // Draw the new shield position based on the direction
  switch (shield->direction)
  {
  case SHIELD_UP:
    drawRectDMA(y - 8, x, length, 3, WHITE); // Vertical line upwards
    break;
  case SHIELD_DOWN:
    drawRectDMA(y + 8, x, length, 3, WHITE); // Vertical line downwards
    break;
  case SHIELD_LEFT:
    drawRectDMA(y - 2, x - 5, 3, length, WHITE); // Horizontal line to the left
    break;
  case SHIELD_RIGHT:
    drawRectDMA(y - 2, x + 11, 3, length, WHITE); // Horizontal line to the right
    break;
  }

  return shield->direction;
}

int main(void)
{

  REG_DISPCNT = MODE3 | BG2_ENABLE;
  sound_init();
  Player player = {93, 118, 5, 5, 100, 100};
  Player player2 = {93, 118, 5, 5, 100, 100};

  Player oldPlayer = player;
  Player oldPlayer2 = player2;

  Shield shield = {93, 118, 9, SHIELD_UP, 1};
  int bulletCounter = 0;
  int switcher = 0;
  int switcher2 = 0;
  int switcher3 = 0;
  int switcher4 = 0;

  u32 previousButtons = BUTTONS;
  u32 currentButtons = BUTTONS;

  ShieldDirection oldShieldDirection = SHIELD_UP;

  gba_state state = START;

  while (1)
  {

    currentButtons = BUTTONS;
    oldPlayer = player;
    oldPlayer2 = player2;

    switch (state)
    {
    case START:

      player.health = 100;
      bulletCounter = 0;
      shield.active = 1;
      bulletCounter = 0;

      if (switcher2 == 0)
      {
        player2.col = 118;
        player2.row = 93;

        switcher2 = 1;
      }
      drawCenteredString(50, 72, 100, 10, "Press Start to Start", WHITE);
      drawCenteredString(70, 72, 100, 10, "Survive 20 seconds ", WHITE);
      drawCenteredString(90, 72, 100, 10, "To win", WHITE);

      clearPlayer(&oldPlayer2);
      updatePlayer(&player2, currentButtons);
      drawImageDMA(player2.row, player2.col, player2.width, player2.height, character);

      if (KEY_DOWN(BUTTON_START, currentButtons))
      {
        state = PLAY; // Change the game state to PLAY
        if (switcher)
        {
          switcher = 0;
          switcher2 = 0;
          switcher3 = 0;
          switcher4 = 0;
        }
      }

      waitForVBlank();

      break;

    case PLAY:
      currentButtons = BUTTONS;
      oldPlayer = player;

      if (switcher == 0)
      {
        waitForVBlank();
        drawFullScreenImageDMA(background2);
        drawBox();
        drawUI();
        drawButtons();
        drawHealthBarBorder();
        initBullets();
        switcher = 1;
      }

      waitForVBlank();

      drawEnemy();

      if (shield.active)
      {
        updateShield(&shield, currentButtons);
      }
      else
      {
        updatePlayer(&player, currentButtons);
      }

      updateBullets(&shield);

      updateHealth(&player, &shield);

      drawBox();

      if (shield.active)
      {
        oldShieldDirection = drawShield(&shield, &player, oldShieldDirection);
      }

      bulletCounter++;

      clearPlayer(&oldPlayer);
      drawHealthBar(&player);
      drawBullets();
      drawImageDMA(player.row, player.col, player.width, player.height, character);

      if (bulletCounter >= 1000)
      {
        shield.active = 0;
        state = WIN;
        fillScreenDMA(BLACK); // Optional: Clear the screen
      }

      if (rand() % 50 == 0)
      {
        Direction dir = rand() % 4;
        addBullet(dir, rand() % 2 + 1);
      }

      if (player.health <= 0)
      {
        state = LOSE;
        fillScreenDMA(BLACK); // Optional: Clear the screen
      }

      break;
    case WIN:
      waitForVBlank();
      if (switcher4 == 0)
      {
        drawFullScreenImageDMA(background1);
        drawCenteredString(70, 72, 100, 10, "Congratulations!", WHITE);
        drawCenteredString(90, 72, 100, 10, "Press Start to Retry", WHITE);
        switcher4 = 1;
      }
      if (KEY_DOWN(BUTTON_START, currentButtons))
      {
        state = START;
        fillScreenDMA(BLACK); // Optional: Clear the screen
      }
      break;
    case LOSE:
      waitForVBlank();
      if (switcher3 == 0)
      {
        drawFullScreenImageDMA(background1);
        drawCenteredString(70, 72, 100, 10, "Game Over!", WHITE);
        drawCenteredString(90, 72, 100, 10, "Press Start to Retry", WHITE);
        switcher3 = 1;
      }
      if (KEY_DOWN(BUTTON_START, currentButtons))
      {
        state = START;        // Reset the game state to START
        fillScreenDMA(BLACK); // Optional: Clear the screen
      }
      break;
    }

    if (KEY_DOWN(BUTTON_SELECT, currentButtons))
    {
      state = START;
      fillScreenDMA(BLACK);
      continue;
    }

    previousButtons = currentButtons;
  }

  UNUSED(previousButtons);

  return 0;
}
