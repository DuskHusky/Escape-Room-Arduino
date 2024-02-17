// General use "Memory" game.

#include "Custom_Functions.h"

void lock() {
  state = INACTIVE;
  stage = 0;
  counter = 0;
  on_puzzle_locked();
}
void set() {
  on_puzzle_set();
}
void activate() {
  state = INACTIVE;
  #ifdef RANDOMISE_ON_ACTIVATION
  randomise();
  #endif
  on_puzzle_activated();
}
void complete() {
  state = COMPLETED;
  on_puzzle_completed();
}

Puzzle main_puzzle = Puzzle(activate, complete, lock, set);
FastSerial fast_serial = FastSerial(8, Serial, main_puzzle);

void randomise() {
  ERBasics::fill_array(order, bag, random_mode);
  ERTesting::print_array(Serial, order);
}

void start() {
  on_sequence_start();
  #ifndef RANDOMISE_ON_ACTIVATION
  randomise();
  #endif
  fill_lights(LOW);
  state = FLASHING;
  counter = 0;
  timer = 0;
}

void main_loop() {
  switch(state) {
    case INACTIVE:
      #ifndef TRIGGER_ON_STATE_CHANGE

      for (int i = 0; i < NUM_SWITCHES; i++) {
        switches[i].update();
        if (switches[i].read() == LOW) return;
      }

      #endif
      
      before_sequence_start();
      if (digitalRead(START_BUTTON_PIN) == LOW) start();

      break;
    case FLASHING:
      static int n = 0;
      static bool power = true;
      timer += Time.delta_time();
      if (n >= 2 * stage_lengths[stage]) {
        state = ACTIVE;
        fill_lights(LOW);
        timer = 0;
        n = 0;
        power = true;
        return;
      }
      if (timer > MILLIS_PER_FLASH) {
        timer = 0;
        fill_lights(LOW);
        set_light(order[n/2], power);
        on_light_flash(n/2);
        power = !power;
        n++;
      }      
      break;
    case ACTIVE:
      check_switches();
      if (counter >= stage_lengths[stage]) {
        counter = 0;
        stage++;
        if (stage >= NUM_STAGES) {
          stage = 0;
          main_puzzle.set_state(PUZZLE_COMPLETE);
        }
        else {
          on_stage_complete();
          state = INACTIVE;
        }
      }
      break;
  }
}

void setup() {
  Serial.begin(9600);
  fast_serial.test_serial();
  initialisation();

  ERBasics::gen_sequence_array(bag);

  for (int i = 0; i < NUM_SWITCHES; i++) {
    pinMode(switch_pins[i], INPUT_PULLUP);
    switches[i].attach(switch_pins[i]);
    switches[i].interval(25);
  }

  pinMode(START_BUTTON_PIN, INPUT_PULLUP);
  main_puzzle.attach_active_loop(main_loop);
  on_setup();
}

void loop() {
  Time.update();
  main_puzzle.run();
  fast_serial.read_serial();
  on_loop();
}
