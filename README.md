## Environment
Operting System: Windows 11

Microcontroller: STMicroelectronics NUCLEO-F446RE

IDE: STM32CubeIDE

## Instructions
- Connect microcontroller to computer via a USB Type-A to Mini-B cable
- Compile and run keyboard_hook.c to transmit keystrokes to microcontroller
- Every minute, OLED will update with the number of letters typed in the last minute

## Libraries Used
stm32-ssd1306 (https://github.com/afiskon/stm32-ssd1306)