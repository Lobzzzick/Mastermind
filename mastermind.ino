#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include "mastermind.h"
#include "lcd_wrapper.h"

LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup() {
    lcd.init();
    lcd.backlight();
    Serial.begin(9600);

    turn_off_leds();

    pinMode(BUTTON_1, INPUT_PULLUP);
    pinMode(BUTTON_2, INPUT_PULLUP);
    pinMode(BUTTON_3, INPUT_PULLUP);
    pinMode(BUTTON_4, INPUT_PULLUP);
    pinMode(BUTTON_ENTER, INPUT_PULLUP);

    pinMode(LED_BLUE_1, OUTPUT);
    pinMode(LED_GREEN_1, OUTPUT);
    pinMode(LED_BLUE_2, OUTPUT);
    pinMode(LED_GREEN_2, OUTPUT);
    pinMode(LED_BLUE_3, OUTPUT);
    pinMode(LED_GREEN_3, OUTPUT);
    pinMode(LED_BLUE_4, OUTPUT);
    pinMode(LED_GREEN_4, OUTPUT);
}

void loop() {
    char* code = generate_code(false, 4);
    play_game(code);
    free(code);
}
