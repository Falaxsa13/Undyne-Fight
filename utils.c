#include "utils.h"

Bullet bullets[MAX_BULLETS];

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
                Note block = {NOTE_B5, 20};
                play_note(block);

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
