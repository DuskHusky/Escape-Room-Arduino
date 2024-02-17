#ifndef SETUP_H
#define SETUP_H
#include "configuration.h"

void on_switch_correct(int id); void on_switch_incorrect(int id);

uint8_t bag[NUM_SWITCHES];
Bounce switches[NUM_SWITCHES]; 

#ifdef ARGB

LEDStrip<LED_TYPE, DATA_PIN, COLOR_ORDER> led_strip = LEDStrip<LED_TYPE, DATA_PIN, COLOR_ORDER>(NUM_LEDS, BRIGHTNESS);

void initialisation() {
  led_strip.set_leds_per_section(LEDS_PER_SWITCH);
  led_strip.clear();
}

void set_light(int position, uint8_t power) {
  led_strip.set_section(position, power ? main_color : CRGB::Black);
}

void fill_lights(uint8_t power) {
  led_strip.fill(power ? main_color : CRGB::Black);
}

#else

void initialisation() {
  for (int i = 0; i < NUM_SWITCHES; i++) {
    pinMode(light_pins[i], OUTPUT);
  }
}

void set_light(int position, uint8_t power) {
  digitalWrite(light_pins[position], power);
}

void fill_lights(uint8_t power) {
  for (int i = 0; i < NUM_SWITCHES; i++) {
    digitalWrite(light_pins[i], power);
  }
}

#endif

void check_switches() {
  for (int i = 0; i < NUM_SWITCHES; i++) {
    switches[i].update();

    #ifdef TRIGGER_ON_STATE_CHANGE
    if (switches[i].changed())
    #else
    if (switches[i].fell())
    #endif
    {
      if (order[counter] == i) {
        on_switch_correct(i);
        counter++;
      }
      #if MAX_STAGE_LENGTH > NUM_SWITCHES
      else if (counter > 0 && order[counter - 1] != i) {
        on_switch_incorrect(i);
        counter = 0;
        state = INACTIVE;
      }
      #else
      else if (random_mode < 3) {
        if (counter > 0 && order[counter - 1] != i) {
          on_switch_incorrect(i);
          counter = 0;
          state = INACTIVE;
        }
      }
      else {
        bool switch_has_passed = false;
        for (int j = 0; j < counter; j++) {
          if (order[counter - 1] == i) {
            switch_has_passed = true;
          }
        }
        if (!switch_has_passed) {
          on_switch_incorrect(i);
          counter = 0;
          state = INACTIVE;
        }
      }
      #endif
    }
  }
}

#endif