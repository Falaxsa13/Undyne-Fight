#ifndef GBAUI_H
#define GBAUI_H

// Constants for the UI layout
#define ENEMY_WIDTH 50
#define ENEMY_HEIGHT 50
#define ENEMY_ROW 10
#define ENEMY_COL 95

#define BOX_TOP 70
#define BOX_BOTTOM 120
#define BOX_LEFT 80
#define BOX_RIGHT 160

#define UI_LABEL_ROW 130
#define UI_LABEL_COL 10

// Constants for the buttons
#define BUTTON_WIDTH 35
#define BUTTON_HEIGHT 12
#define BUTTON_SPACING 10
#define BUTTON_ROW 145

#define HEALTH_BAR_FULL_COLOR YELLOW  // Assuming YELLOW is defined appropriately
#define HEALTH_BAR_LOW_COLOR RED      // Assuming RED is defined appropriately
#define HEALTH_BAR_BORDER_COLOR WHITE // Color for the border of the health bar

#define HEALTH_BAR_MAX_WIDTH 50 // Maximum width of the health bar
#define HEALTH_BAR_HEIGHT 5     // Height of the health bar
#define HEALTH_BAR_BORDER 1     // Thickness of the health bar border
#define HEALTH_BAR_WIDTH 50     // Adjust as needed to fit UI_LABEL_ROW
#define HEALTH_BAR_HEIGHT 5
#define HEALTH_BAR_Y UI_LABEL_ROW // Set this to the row where you want the health bar
#define HEALTH_BAR_X (160 - HEALTH_BAR_WIDTH)

void drawEnemy(void);
void drawBox(void);
void drawUI(void);
void drawButtons(void);
void drawHealthBarBorder(void);

#endif