#ifndef CUSTOM_FUNCTIONS_H
#define CUSTOM_FUNCTIONS_H
#include "Setup.h"

void on_setup() {

}

void on_loop() {

}

void on_light_flash(int id) {
  
}

void on_switch_correct(int id) {
  set_light(id, HIGH);
}

void on_switch_incorrect(int id) {
  unsigned long temp_timer = 0;
  while (temp_timer < 1000) {
    fill_lights(ERBasics::time_cycle(temp_timer, 125, 2));
    temp_timer += Time.delta_time();
  }
  fill_lights(HIGH);
}

void before_sequence_start() {
  // called every loop until the sequence is started while puzzle is active.

}

void on_sequence_start() {
  // called once when the sequence is started.

}

void on_stage_complete() {
  fill_lights(LOW);
}

// Any special events to happen when the puzzle is locked, set, activated, or completed can be added to
// the relevent function. (Do not remove the 'state' setting lines)
void on_puzzle_activated() {
  fill_lights(HIGH);
}
void on_puzzle_completed() {
  Serial.println("Puzzle Complete");
  led_strip.fill(CRGB::Green);
}
void on_puzzle_locked() {
  fill_lights(LOW);
}
void on_puzzle_set() {

}
//

#endif