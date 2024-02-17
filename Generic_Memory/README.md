# Generic Memory Puzzle

The **only** files that need to ba changed are *Configuration.h* and *Custom_Functions.h*.

# Configuration

## Optional Settings

Certain `#define` lines have been commented out (the lines start with //) near the top of *Configuration.h*. These lines represent certain optional settings which can be enabled by removing the '//' at the start of the line (only the '//' before `#define` needs to be removed).

 - `#define TRIGGER_ON_STATE_CHANGE` - Uncommenting this line will make the arduino register switch state changes as presses. If left commented, the switches/buttons will only register as a 'press' when they fall. This will also remove the requirement for all handles to be in the correct position to start. This should be used if you are using levers and are using either a non 'bag draw' randomisation mode or if MAX_STAGE_LENGTH > NUM_SWITCHES.

 - `#define ARGB` - Uncommenting this line will make the arduino use an led strip to show the order instead of individually wired lights.

 - `#define RANDOMISE_ON_ACTIVATION` - Uncommenting this line will make the sequence randomise only after activating. This means that if the player fails, the sequence will not change.

## Variables

The following variables need to be correctly defined for the program to function correctly.

 - `NUM_STAGES` - Defines how many stages need to be completed.

 - `NUM_SWITCHES` - Defines how many switches are in the puzzle (not including the start button).

 - `MAX_STAGE_LENGTH` - Defines the length of the longest stage.

 - `START_BUTTON_PIN` - Defines the pin the start button is connected to.

 - `MILLIS_PER_FLASH` - Defines how long between wach flash and how long the lights stay on for while flashing the sequence.

 - `random_mode` - Defines the mode of randomisation to be used when randomising the sequence. (More information in the *Randomisation* section below)

 - `switch_pins` - A list (of length NUM_SWITCHES) of the pins that each of the switches are connected to.

 - `stage_lengths` - A list (of length NUM_STAGES) of the stage lengths in order. Each value in the list must be less than MAX_STAGE_LENGTH to function properly.

### Conditional Variables

Some of the variables in *Configuration.h* are conditionally required. These variables, their descriptions, and the conditions in which you need to define them are as follows.

 - `order` - The pre-defined sequence of the puzzle. if not set, it will default to a list starting at 0 and incrementing by 1 (e.g. [0,1,2,3,4]). Does not need to be defined if using randomisation.

#### If not using ARGB

 - `light_pins` - A list (of length NUM_SWITCHES) of the pins that each of the lights are connected to.

#### If using ARGB

This program uses the FastLED library to control the LEDs. For more information see the [FastLED Doumentation](https://fastled.io/docs/).

 - `NUM_LEDS` - The number of LEDs in the strip.

 - `DATA_PIN` - The data pin being used to control the leds.

 - `LED_TYPE` - The LED type.

 - `COLOR_ORDER` - The colour order of the led type.

 - `BRIGHTNESS` - Defines the brightness of the led_strip.

 - `LEDS_PER_SWITCH` - Defines how many LEDs should light up for each switch. If you have multiple LEDs in, above, or around each switch, this should be set to however many there are.

 - `main_color` - The main colour for the leds to light up. Any other desired colours or behaviours can be programmed into the custom functions.

### Randomisation

The `random_mode` variable defines the style of randomisation, if any, for the puzzle. If no randomisation mode is chosen, then the order must be manually defined, otherwise it will default to an array of numbers (e.g. [0, 1, 2, 3, 4]). The randomisation modes that are available are as follows:

| mode | description |
|:---:|:---|
| RANDOM_NONE | Does not randomise the puzzle, will use the user defined order if defined, otherwise wil increment from zero to the number of switches. |
| RANDOM_NORMAL | Completely random. can repeat a switch multiple times in a row |
| RANDOM_NO_REPEAT | Completely random but will not repeat a number multiple times in a row. A switch could still appear multiple times in the sequence.
| RANDOM_BD | Random bag draw. Draws switches from a "bag". Once a switch has been drawn, it will be removed from the bag and thus can't be chosen again. After the bag runs out, it will be refilled. |
| RANDOM_BD_NO_REPEAT | Random bag draw. Will not draw the same switch twice in a row. |

---

# Custom Functions

In the file *Custom_Functions.h*, numerous functions have been defined which can be used to customise the puzzle. Below is a list of the functions and their descriptions.

 - `on_setup` - Calls once at the very end of the setup funciton.

 - `on_loop` - Calls once at the end of every loop.

 - `on_light_flash` - Calls twice for each light while flashing the sequence (once when the light turns on and once when the light turns off. The `id` parameter is either the position of the lights pin in `light_pins` or the section of LEDs as they are connected.

 - `on_switch_correct`/`on_switch_incorrect` - Both of these functions are called once when a switch is turned (the function called depends on whether it wa the correct switch or not). The `id` parameter is the position of correlated pin in switch_pins.

 - `before_sequence_start` - Calls every loop before the sequence is started only if the puzzle is active.

 - `on_sequence_start` - Calls once whenever a sequence is started (just after the start button is pressed).

 - `on_stage_complete` - Calls once after a stage has been completed (excluding the final stage).

 - `on_puzzle_activated`/`on_puzzle_completed`/`on_puzzle_locked`/`on_puzzle_set` - Calls once when the puzzle is activated/completed/locked/set.

## Useful Things for Customisation

### Variables

 - `counter` (int) - Starts at 0 and increments by 1 each time the correct switch is turned.

 - `stage` (int) - Starts at 0 and increments by 1 each time a stage is completed.

### Functions

 - `set_light(int position, uint8_t power)` - Sets the power state of the light/LED for the given position. The position follows the same rules as the id in the custom functions.

 - `fill_lights(uint8_t power)` - Sets the power state of every light to the given value.

 - `main_puzzle.set_state(PuzzleState state)` - Sets the puzzles state. The `state` parameter can be one of the following: `PUZZLE_ACTIVE`, `PUZZLE_COMPLETE`, `PUZZLE_LOCKED`, or `PUZZLE_SET`. Setting the puzzles state will call the relevant functions.

 - `main_puzzle.get_state()` - Returns the current state of the puzzle. The states values are as follows:
|state   |value |
|:---:   |:---- |
|Active  |0     |
|Complete|1     |
|Locked  |2     |
|Set     |3     |

 - `led_strip.set_led(uint_16 pos, CRGB color)` - Sets the colour of an LED at a certain position to the given colour.

 - `led_strip.set_section(uint16_t section, CRGB color)` - Sets the colour of every LED in sections of size `LEDS_PER_SWITCH` (e.g. section 0 is the set of leds around the first switch).

 - `led_strip.fill(CRGB color)` - Sets every LED to the given colour.

 - `led_strip.clear()` - Turns off every LED.

 - `Time.delta_time()` - Returns the number of milliseconds between the previous and current loop.
