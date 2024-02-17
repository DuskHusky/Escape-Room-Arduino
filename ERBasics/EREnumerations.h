#ifndef ER_ENUM_H
#define ER_ENUM_H

enum PuzzleCommand {
	ACTIVATE,
	BYPASS,
	LOCK,
	SET
};

enum PuzzleState {
	PUZZLE_ACTIVE,
	PUZZLE_COMPLETE,
	PUZZLE_LOCKED,
	PUZZLE_SET
};

enum ButtonLEDMode {
	MB_NONE,
	MB_ON_WHEN_PRESSED,
	MB_OFF_WHEN_PRESSED,
	MB_ALTERNATE_WHEN_PRESSED
};

enum LEDState {
	LED_OFF,
	LED_ON
};

#endif