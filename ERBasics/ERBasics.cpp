#include "ERBasics.h"

// ---- Class: _Time -----------------------------------

void _Time::update() {
	d_time = millis() - prev_millis;
	prev_millis = millis();
	
	for (int i = 0; i < MAX_COROUTINES; i++) {
		if (coroutines[i] != nullptr) {
			coroutines[i](d_time);
			if (cr_ends[i] > 0 && millis() > cr_ends[i]) {
				end_coroutine(i);
			}
		}
	}
}

long _Time::delta_time() {
	return d_time;
}

void _Time::start_coroutine(Coroutine function, long duration = 0) {
	int n = -1;
	for (int i = MAX_COROUTINES - 1; i >= 0; i--) {
		if (coroutines[i] == function) {
			Serial.println("Couldn't start coroutine. Coroutine already exists.");
			return;
		}
		else if (coroutines[i] == nullptr) {
			n = i;
		}
	}
	if (n < 0) {
		Serial.println("Couldn't start coroutine. Coroutine limit exceeded.");
	}
	else {
		coroutines[n] = function;
		cr_ends[n] = (duration > 0) ? millis() + duration : 0;
	}
}

void _Time::end_coroutine(Coroutine function) {
	for (int i = 0; i < MAX_COROUTINES; i++) {
		if (coroutines[i] == function) {
			coroutines[i] = nullptr;
			cr_ends[i] = 0;
			if (on_coroutine_end != nullptr) {
				on_coroutine_end(function);
			}
			return;
		}
	}
}
void _Time::end_coroutine(int index) {
	Coroutine func = coroutines[index];
	coroutines[index] = nullptr;
	cr_ends[index] = 0;
	if (on_coroutine_end != nullptr) {
		on_coroutine_end(func);
	}
}

void _Time::attach_cr_end_func(void(*func)(Coroutine)) {
	on_coroutine_end = func;
}

_Time Time;

namespace ERBasics {
	unsigned int time_cycle(unsigned long t, unsigned long interval) {
		return t / interval;
	}
	unsigned int time_cycle(unsigned long t, unsigned long interval, unsigned int overflow) {
		return (t / interval) % overflow;
	}
}

// ---- Class: _Time -----------------------------------

// ---- Class: Button ----------------------------------

Button::Button(uint8_t pin, int interval) : pin(pin) {
	button.attach(pin);
	button.interval(interval);
}

Button::Button(uint8_t pin, int interval, VoidCallback _on_press) : pin(pin) {
	button.attach(pin);
	button.interval(interval);
	set_on_press_callback(_on_press);
}

Button::Button(uint8_t pin, int interval, VoidCallback _on_press, VoidCallback _on_release) : pin(pin) {
	button.attach(pin);
	button.interval(interval);
	set_on_press_callback(_on_press);
	set_on_release_callback(_on_release);
}

Button::Button(uint8_t pin, int interval, VoidCallback _on_press, VoidCallback _on_release, VoidCallback _on_hold) : pin(pin) {
	button.attach(pin);
	button.interval(interval);
	set_on_press_callback(_on_press);
	set_on_release_callback(_on_release);
	set_on_hold_callback(_on_hold);
}

void Button::set_on_press_callback(VoidCallback callback) {
	this->on_press = callback;
}
void Button::set_on_release_callback(VoidCallback callback) {
	this->on_release = callback;
}
void Button::set_on_hold_callback(VoidCallback callback) {
	this->on_hold = callback;
}

void Button::check_button() {
	button.update();
	if (button.fell()) {
		if (on_press != nullptr) on_press();
		update_led_state(true);
	}
	else if (button.rose()) {
		if (on_release != nullptr) on_release();
		update_led_state(false);
	}
	if (button.read() == 0) {
		if (on_hold != nullptr) on_hold();
	}
}

void Button::update_led_state(bool fell) {}
void Button::set_led_state(LEDState led_state) {}

// ---- Class: Button ----------------------------------

// ---- Class: MonoButton ------------------------------



// ---- Class: MonoButton ------------------------------

// ---- Class: LEDStrip --------------------------------



// ---- Class: LEDStrip --------------------------------

// ---- Class: Puzzle ----------------------------------

Puzzle::Puzzle() {}

Puzzle::Puzzle(VoidCallback on_lock) {
	set_lock_callback(on_lock);
}

Puzzle::Puzzle(VoidCallback on_activate, VoidCallback on_complete, VoidCallback on_lock) {
	set_activate_callback(on_activate);
	set_complete_callback(on_complete);
	set_lock_callback(on_lock);
}

Puzzle::Puzzle(VoidCallback on_activate, VoidCallback on_complete, VoidCallback on_lock, VoidCallback on_set) {
	set_activate_callback(on_activate);
	set_complete_callback(on_complete);
	set_lock_callback(on_lock);
	set_set_callback(on_set);
}

void Puzzle::set_activate_callback(VoidCallback callback) {
	this->activate = callback;
}
void Puzzle::set_complete_callback(VoidCallback callback) {
	this->complete = callback;
}
void Puzzle::set_lock_callback(VoidCallback callback) {
	this->lock = callback;
}
void Puzzle::set_set_callback(VoidCallback callback) {
	this->set = callback;
}

void Puzzle::attach_active_loop(VoidCallback callback) {
	this->active_loop = callback;
}

void Puzzle::set_state(PuzzleState _state) {
	state = _state;
	switch (_state) {
	case PUZZLE_ACTIVE:
		if (activate != nullptr) {
			Serial.println("Puzzle Activated");
			activate();
		}
		break;
	case PUZZLE_COMPLETE:
		if (complete != nullptr) {
			Serial.println("Puzzle Completed");
			complete();
		}
		break;
	case PUZZLE_LOCKED:
		if (lock != nullptr) {
			Serial.println("Puzzle Locked");
			lock();
		}
		break;
	case PUZZLE_SET:
		if (set != nullptr) {
			Serial.println("Puzzle Set");
			set();
		}
		break;
	}
}
PuzzleState Puzzle::get_state() {
	return state;
}

void Puzzle::run() {
	if (state == PUZZLE_ACTIVE) {
		if (active_loop != nullptr) {
			active_loop();
		}
	}
}

// ---- Class: Puzzle ----------------------------------
