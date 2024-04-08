#include "../include/player.h"

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