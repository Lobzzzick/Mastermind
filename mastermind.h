#ifndef _MASTERMIND_H
#define _MASTERMIND_H

#define BUTTON_ENTER 1
#define BUTTON_1 2
#define BUTTON_2 3
#define BUTTON_3 4
#define BUTTON_4 5

#define LED_BLUE_4 12
#define LED_GREEN_1 13
#define LED_BLUE_3 10
#define LED_GREEN_2 11
#define LED_BLUE_2 8
#define LED_GREEN_3 9
#define LED_BLUE_1 6
#define LED_GREEN_4 7

char* generate_code(bool repeat, int length);
void play_game(const char* secret);
void render_leds(int peg_a, int peg_b);
void turn_off_leds();
void get_score(const char* secret, const char* guess, int* peg_a, int* peg_b);
void guess_input(char* guess, const char* secret);
void render_history(const char* secret, char** history, int entry);

#endif


