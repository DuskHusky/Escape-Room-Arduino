#ifndef ER_TIME_H
#define ER_TIME_H

#include "Arduino.h"

#ifndef MAX_COROUTINES
#define MAX_COROUTINES 1
#endif

typedef void(*Coroutine) (long);

template <unsigned int max_coroutines>
class _Time {
public:
	void setup();

	void update();

	long delta_time();

	int start_coroutine(Coroutine function, long duration = 0);

	void end_coroutine(Coroutine function);

	void end_coroutine(int index);

	void attach_cr_end_func(void(*func)(Coroutine));

	void set_cr_overload(bool stop);

private:
	long prev_millis = 0, d_time = 0;
	bool stop_on_cr_overload = true;
	unsigned long cr_ends[max_coroutines];
	Coroutine* coroutines;
	void(*on_coroutine_end)(Coroutine);
};

template <unsigned int max_coroutines>
void _Time<max_coroutines>::setup() {
	coroutines = (Coroutine*)calloc(max_coroutines, sizeof(Coroutine));
}

template <unsigned int max_coroutines>
void _Time<max_coroutines>::update() {
	d_time = millis() - prev_millis;
	prev_millis = millis();

	if (coroutines == nullptr)
		return;

	for (int i = 0; i < max_coroutines; i++) {
		if (coroutines[i] != nullptr) {
			coroutines[i](d_time);
			if (cr_ends[i] > 0 && millis() > cr_ends[i]) {
				end_coroutine(i);
			}
		}
	}
}

template <unsigned int max_coroutines>
long _Time<max_coroutines>::delta_time() {
	return d_time;
}

template <unsigned int max_coroutines>
int _Time<max_coroutines>::start_coroutine(Coroutine function, long duration = 0) {
	if (coroutines == nullptr) {
		Serial.println("Couldn't start coroutine. _Time object has not been setup.");
		return 1;
	}

	int n = -1;
	for (int i = max_coroutines - 1; i >= 0; i--) {
		if (coroutines[i] == function) {
			Serial.println("Couldn't start coroutine. Coroutine already exists.");
			while (stop_on_cr_overload);
			return 2;
		}
		else if (coroutines[i] == nullptr) {
			n = i;
		}
	}
	if (n < 0) {
		Serial.println("Couldn't start coroutine. Coroutine limit exceeded.");
		while (stop_on_cr_overload);
		return 3;
	}
	else {
		coroutines[n] = function;
		cr_ends[n] = (duration > 0) ? millis() + duration : 0;
	}
	return 0;
}

template <unsigned int max_coroutines>
void _Time<max_coroutines>::end_coroutine(Coroutine function) {
	for (int i = 0; i < max_coroutines; i++) {
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

template <unsigned int max_coroutines>
void _Time<max_coroutines>::end_coroutine(int index) {
	Coroutine func = coroutines[index];
	coroutines[index] = nullptr;
	cr_ends[index] = 0;
	if (on_coroutine_end != nullptr) {
		on_coroutine_end(func);
	}
}

template <unsigned int max_coroutines>
void _Time<max_coroutines>::attach_cr_end_func(void(*func)(Coroutine)) {
	on_coroutine_end = func;
}

template <unsigned int max_coroutines>
void _Time<max_coroutines>::set_cr_overload(bool stop) {
	stop_on_cr_overload = stop;
}

extern _Time<MAX_COROUTINES> Time;

#endif