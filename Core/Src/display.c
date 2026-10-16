#include "display.h"


extern I2C_HandleTypeDef hi2c1;

/*
 * @brief Initialize OLED display and clear display
 * @param None
 * @retval None
 */
void Display_Init(void) {
	ssd1306_Init();

	// Clear Screen
	ssd1306_Fill(Black);
	ssd1306_UpdateScreen();

	// Write Header
	ssd1306_SetCursor(0, 0);
	ssd1306_WriteString("Typing Speed", Font_7x10, White);
	ssd1306_UpdateScreen();
}

/*
 * @brief Write typing speed on OLED display
 * @param numCharsTyped is the number of characters typed in last 60 seconds
 * @retval None
 */
void Display_WriteSpeed(uint16_t numCharsTyped) {
	char charBuffer[20];
	ssd1306_SetCursor(25, 25);
	snprintf(charBuffer, sizeof(charBuffer), "%d char/min", numCharsTyped);
	ssd1306_WriteString(charBuffer, Font_7x10, White);
	ssd1306_UpdateScreen();
}

/*
 * @brief Clear typing speed on OLED display
 * @param None
 * @retval None
 */
void Display_ClearSpeed() {
	// Erase the previous number
	for (int y = 25; y < 25 + 12; y++) {
		for (int x = 25; x < 128; x++) {
			ssd1306_DrawPixel(x, y, Black);
		}
	}
	ssd1306_UpdateScreen();
}
