#ifndef DEFINITIONS_H
#define DEFINITIONS_H
#include <ERBasics.h>

enum PlayState {
  INACTIVE,
  FLASHING,
  ACTIVE,
  COMPLETED
};
PlayState state = INACTIVE;

int counter = 0;
int stage = 0;
unsigned long timer = 0;

extern Puzzle main_puzzle;
extern FastSerial fast_serial;

#endif