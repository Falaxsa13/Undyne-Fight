#ifndef UTILS_H
#define UTILS_H

#include "sound.h"
#include "gba.h"
#include "gbaUI.h"
#include "images/character.h"

#define MAX_BULLETS 10

typedef enum
{
    NORTH,
    SOUTH,
    EAST,
    WEST
} Direction;

typedef struct
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

typedef struct
{
    int row;
    int col;
    int width;
    int height;
    int health;
    int maxHealth;
} Player;

extern Bullet bullets[MAX_BULLETS];

void clearBullet(Bullet *bullet);
void initBullets(void);
void addBullet(Direction dir, int speed);
void updateBullets(Shield *shield);
int checkShieldCollision(Shield *shield, Bullet *bullet); // Use struct Bullet here for consistency.
void updateShield(Shield *shield, u32 currentButtons);
void drawBullets(void);
ShieldDirection drawShield(Shield *shield, Player *player, ShieldDirection oldDirection);

#endif
