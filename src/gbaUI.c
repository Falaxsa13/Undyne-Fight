#include "../include/gbaUI.h"

// You can define colors like this
#define COLOR_ENEMY WHITE
#define COLOR_BOX WHITE
#define COLOR_UI WHITE
#define COLOR_TEXT BLACK

const int buttonsStartCol = (240 - (4 * BUTTON_WIDTH + 3 * BUTTON_SPACING)) / 2;

#define FIRST_BUTTON_COL buttonsStartCol // Updated to use the calculated start position

void drawEnemy(void)
{ // 112@66
    drawImageDMA(ENEMY_ROW, 74, 100, 100, undyne);
}

void drawBox(void)
{
    drawRectDMA(BOX_TOP, BOX_LEFT, BOX_RIGHT - BOX_LEFT, 1, COLOR_BOX);    // Top border
    drawRectDMA(BOX_BOTTOM, BOX_LEFT, BOX_RIGHT - BOX_LEFT, 1, COLOR_BOX); // Bottom border
    drawRectDMA(BOX_TOP, BOX_LEFT, 1, BOX_BOTTOM - BOX_TOP, COLOR_BOX);    // Left border
    drawRectDMA(BOX_TOP, BOX_RIGHT, 1, BOX_BOTTOM - BOX_TOP, COLOR_BOX);   // Right border
}

void drawUI(void)
{
    char *label = "FALAXSA";

    drawString(HEALTH_BAR_Y - HEALTH_BAR_BORDER, 50, label, WHITE);
}
void drawButtons(void)
{
    int col = FIRST_BUTTON_COL;
    for (int i = 0; i < 4; i++)
    {
        drawRectDMA(BUTTON_ROW, col, BUTTON_WIDTH, BUTTON_HEIGHT, YELLOW);
        col += BUTTON_WIDTH + BUTTON_SPACING;
    }
    // Positioning text within the buttons can be similarly centered if needed
    drawCenterString(BUTTON_ROW + 3, FIRST_BUTTON_COL, BUTTON_WIDTH, "FIGHT", COLOR_TEXT);
    drawCenterString(BUTTON_ROW + 3, FIRST_BUTTON_COL + BUTTON_WIDTH + BUTTON_SPACING, BUTTON_WIDTH, "ACT", COLOR_TEXT);
    drawCenterString(BUTTON_ROW + 3, FIRST_BUTTON_COL + 2 * (BUTTON_WIDTH + BUTTON_SPACING), BUTTON_WIDTH, "ITEM", COLOR_TEXT);
    drawCenterString(BUTTON_ROW + 3, FIRST_BUTTON_COL + 3 * (BUTTON_WIDTH + BUTTON_SPACING), BUTTON_WIDTH, "MERCY", COLOR_TEXT);
}

void drawHealthBarBorder(void)
{
    drawRectDMA(HEALTH_BAR_Y - HEALTH_BAR_BORDER, HEALTH_BAR_X - HEALTH_BAR_BORDER,
                HEALTH_BAR_MAX_WIDTH + (HEALTH_BAR_BORDER * 2), HEALTH_BAR_HEIGHT + (HEALTH_BAR_BORDER * 2),
                HEALTH_BAR_BORDER_COLOR);
}
