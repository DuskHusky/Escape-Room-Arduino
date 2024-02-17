#ifndef CONFIG_H
#define CONFIG_H
#include "Definitions.h"

// Uncomment this line to make the arduino register switch state changes as presses.
// If left commented, the switches/buttons will need to fall to be registered as a press.
// - This will remove the requirement for all handles to be in the correct position to start.
// #define TRIGGER_ON_STATE_CHANGE

// Uncomment this line if using an led strip (e.g. ws2812/2811) instead of individual leds.
 #define ARGB

// Uncomment the following line to make the puzzle randomise only when it is activated.
// If the line remains commented, the puzzle will randomise every sequence.
// #define RANDOMISE_ON_ACTIVATION

#define NUM_STAGES        2
#define NUM_SWITCHES      10
#define MAX_STAGE_LENGTH  5

#define START_BUTTON_PIN  13
#define MILLIS_PER_FLASH  500

const RANDOM_MODE random_mode = RANDOM_BD_NO_REPEAT;

const uint8_t switch_pins[NUM_SWITCHES] = {2, 3, 4, 5, 6, 7, 8, 9, 10, 11};
uint8_t order[MAX_STAGE_LENGTH] = {};
uint8_t stage_lengths[NUM_STAGES] = {5, 5};

#ifdef ARGB // Needs definition only if using an addr. LED strip

#define NUM_LEDS    20
#define DATA_PIN    12
#define LED_TYPE    WS2812
#define COLOR_ORDER RGB
#define BRIGHTNESS  20
#define LEDS_PER_SWITCH 2
CRGB main_color = CRGB::Red;

#else // Needs definition only if not using an addr. LED strip

uint8_t light_pins[NUM_SWITCHES] = {};

#endif

#endif