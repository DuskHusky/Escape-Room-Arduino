#ifndef ER_FAST_SERIAL_H
#define ER_FAST_SERIAL_H

#ifndef SoftwareSerial_h

class FastSerial {
public:
	typedef void (*ReadCallback)(char*, int);

	FastSerial(int max_message_length, HardwareSerial& _serial, ReadCallback callback) :
		max_msg_length(max_message_length), serial(_serial), read_callback(callback) {
		message = (char*)malloc(max_message_length + 1);
	}

#ifdef ER_BASICS_H
	FastSerial(int max_message_length, HardwareSerial& _serial, Puzzle& _puzzle) :
		max_msg_length(max_message_length), serial(_serial), puzzle(&_puzzle) {
		message = (char*)malloc(max_message_length + 1);
	}
	void set_puzzle_state(PuzzleState _state) {
		puzzle->set_state(_state);
	}
#endif

	template <size_t length>
	void emulate(char(&message)[length]) {
#ifdef ER_BASICS_H
		if (puzzle != nullptr) {
			set_puzzle_state(PuzzleState(int(message[0]) - '0'));
		}
#endif
		if (read_callback != nullptr) {
			read_callback(message, length);
		}
	}

	void test_serial() {
		serial.println("Ready");
	}
	void read_serial() {
		while (serial.available()) {
			static int message_pos = 0;

			char in_byte = serial.read();

			if (in_byte != '\n' && (message_pos < max_msg_length)) {
				message[message_pos] = in_byte;
				message_pos++;
			}
			else {
				message[message_pos] = '\0';

#ifdef ER_BASICS_H
				if (puzzle != nullptr) {
					set_puzzle_state(int(message[0]) - '0');
				}
#endif
				if (read_callback != nullptr) {
					read_callback(message, message_pos);
				}

				message_pos = 0;
			}
		}
	}

private:
	char* message; // The most recent serial read.
	int max_msg_length;
	HardwareSerial& serial;
	ReadCallback read_callback;
#ifdef ER_BASICS_H
	Puzzle* puzzle;
#endif
};

#else

template <typename T>
class FastSerial {
	static_assert (
		arx::stdx::is_same<T, HardwareSerial>::value ||
		arx::stdx::is_same<T, SoftwareSerial>::value,
		"FastSerial requires either a HardwareSerial or SoftwareSerial."
		);
public:
	typedef void (*ReadCallback)(char*, int);

	FastSerial(int max_message_length, T& _serial, ReadCallback callback) :
		max_msg_length(max_message_length), serial(_serial), read_callback(callback) {
		message = (char*)malloc(max_message_length + 1);
	}

#ifdef ER_BASICS_H
	FastSerial(int max_message_length, T& _serial, Puzzle& _puzzle) :
		max_msg_length(max_message_length), serial(_serial), puzzle(&_puzzle) {
		message = (char*)malloc(max_message_length + 1);
	}
	void set_puzzle_state(PuzzleState _state) {
		puzzle->set_state(_state);
	}
#endif

	template <size_t length>
	void emulate(char(&message)[length]) {
#ifdef ER_BASICS_H
		if (puzzle != nullptr) {
			set_puzzle_state(int(message[0]) - '0');
		}
#endif
		if (read_callback != nullptr) {
			read_callback(message, length);
		}
	}

	void test_serial() {
		serial.println("Ready");
	}
	void read_serial() {
		while (serial.available()) {
			static int message_pos = 0;

			char in_byte = serial.read();

			if (in_byte != '\n' && (message_pos < max_msg_length)) {
				message[message_pos] = in_byte;
				message_pos++;
			}
			else {
				message[message_pos] = '\0';
#ifdef ER_BASICS_H
				if (puzzle != nullptr) {
					set_puzzle_state(int(message[0]) - '0');
				}
#endif
				if (read_callback != nullptr) {
					read_callback(message, message_pos);
				}

				message_pos = 0;
			}
		}
	}

private:
	char* message; // The most recent serial read.
	int max_msg_length;
	T& serial;
	ReadCallback read_callback;
#ifdef ER_BASICS_H
	Puzzle* puzzle;
#endif
};

#endif

#endif