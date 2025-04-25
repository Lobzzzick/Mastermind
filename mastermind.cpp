#include "mastermind.h"
#include "lcd_wrapper.h"
#include <Arduino.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <LiquidCrystal_I2C.h>

extern LiquidCrystal_I2C lcd;

char* generate_code(bool repeat, int length) {
    if (length < 1) return NULL;

    char* code = (char*)malloc((length + 1) * sizeof(char));
    if (code == NULL) return NULL;

    for (int i = 0; i < length; i++) {
        code[i] = '0' + random(10);
        if (!repeat) {
            for (int j = 0; j < i; j++) {
                if (code[i] == code[j]) {
                    i--;
                    break;
                }
            }
        }
    }
    code[length] = '\0';
    return code;
}


void get_score(const char* secret, const char* guess, int* peg_a, int* peg_b) {
    *peg_a = 0;
    *peg_b = 0;

    int freq_secret[10] = {0};
    int freq_guess[10] = {0};

    for (int i = 0; i < 4; i++) {
        if (secret[i] == guess[i]) {
            (*peg_a)++;
        } else {
            freq_secret[secret[i] - '0']++;
            freq_guess[guess[i] - '0']++;
        }
    }

    for (int i = 0; i < 10; i++) {
        *peg_b += min(freq_secret[i], freq_guess[i]);
    }
}

void turn_off_leds() {
    digitalWrite(LED_BLUE_1, LOW);
    digitalWrite(LED_GREEN_1, LOW);
    digitalWrite(LED_BLUE_2, LOW);
    digitalWrite(LED_GREEN_2, LOW);
    digitalWrite(LED_BLUE_3, LOW);
    digitalWrite(LED_GREEN_3, LOW);
    digitalWrite(LED_BLUE_4, LOW);
    digitalWrite(LED_GREEN_4, LOW);
}

void render_leds(const int peg_a, const int peg_b) {
    turn_off_leds();

    for (int i = 0; i < peg_a; i++) {
        digitalWrite(LED_GREEN_1 - i * 2, HIGH);
    }

    for (int i = 0; i < peg_b; i++) {
        digitalWrite(LED_BLUE_1 + i * 2, HIGH);
    }
}

char* guess_input(const char* secret, char** history, int history_index) {
    const int buttonPins[] = {BUTTON_1, BUTTON_2, BUTTON_3, BUTTON_4, BUTTON_ENTER};
    int digits[4] = {0, 0, 0, 0};
    bool enterPressed = false;
    bool lastButtonState[5] = {false, false, false, false, false};

    while (!enterPressed) {
        for (int i = 0; i < 4; i++) {
            bool buttonState = digitalRead(buttonPins[i]);
            if (buttonState != lastButtonState[i] && buttonState == LOW) {
                digits[i] = (digits[i] + 1) % 10;
            }
            lastButtonState[i] = buttonState;
        }

        bool enterButtonState = digitalRead(BUTTON_ENTER);
        bool oneButtonState = digitalRead(BUTTON_1);
        bool threeButtonState = digitalRead(BUTTON_3);
        bool twoButtonState = digitalRead(BUTTON_2);
        bool fourButtonState = digitalRead(BUTTON_4);

        if ((enterButtonState != lastButtonState[4] && enterButtonState == LOW) || (oneButtonState && threeButtonState && enterButtonState)) {
            enterPressed = true;
        }

        if (twoButtonState && fourButtonState) {
            lcd.clear();
            for (int i = 0; i < history_index; i++) {
                render_history(secret, history, i);
                delay(2000);
            }
            lcd.clear();
        }

        lastButtonState[4] = enterButtonState;

        lcd.setCursor(0, 0);
        lcd.print("Enter your guess");
        lcd.setCursor(0, 1);
        for (int i = 0; i < 4; i++) {
            lcd.print(digits[i]);
            lcd.print(" ");
        }
        delay(200);
    }

    char* guess = (char*)malloc(5 * sizeof(char));
    for (int i = 0; i < 4; i++) {
        guess[i] = '0' + digits[i];
    }
    guess[4] = '\0';

    return guess;
}

void render_history(const char* secret, char** history, int entry) {
    int peg_a, peg_b;
    get_score(secret, history[entry], &peg_a, &peg_b);
    lcd.setCursor(0, 0);
    lcd.print(entry + 1);
    lcd.print(": ");
    lcd.print(history[entry]);
    lcd.print(" ");
    lcd.print(peg_a);
    lcd.print("A");
    lcd.print(peg_b);
    lcd.print("B");
}

void play_game(const char* secret) {
    int peg_a, peg_b;
    char* history[10];
    int history_index = 0;

    lcd.clear();
    lcd.print("Welcome to");
    lcd.setCursor(0, 1);
    lcd.print("MasterMind");
    delay(2000);
    lcd.clear();
    lcd.print("Your goal is to");
    lcd.setCursor(0, 1);
    lcd.print("guess my secret");
    delay(2000);
    lcd.clear();
    lcd.print("combination.");
    delay(2000);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Let's start!");
    delay(2000);
    lcd.clear();

    while (history_index < 10) {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Enter your guess");

        char* guess = guess_input(secret, history, history_index);

        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Code sent for");
        lcd.setCursor(0, 1);
        lcd.print("checking...");
        delay(1000);

        turn_off_leds();
        
        get_score(secret, guess, &peg_a, &peg_b);

        render_leds(peg_a, peg_b);

        history[history_index++] = strdup(guess);
        render_history(secret, history, history_index - 1);

        if (peg_a == 4) {
            lcd.clear();
            lcd.setCursor(0, 0);
            lcd.print("You win!");
            delay(2000);
            lcd.clear();
            lcd.print("Code is:");
            lcd.setCursor(0, 1);
            lcd.print(secret);
            break;
        }

        free(guess);
    }

    if (history_index == 10) {
        lcd.clear();
        lcd.print("You used all");
        lcd.setCursor(0, 1);
        lcd.print("attempts");
        delay(2000);
        lcd.clear();
        lcd.print("Secret code is");
        lcd.setCursor(0, 1);
        lcd.print(secret);
        delay(2000);
    }

    for (int i = 0; i < history_index; i++) {
        free(history[i]);
    }
}
