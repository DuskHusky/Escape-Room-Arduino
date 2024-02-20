# ERFastSerial

*ERFastSerial.h* is a sub library from ERBasics. It can be used by itself but is automatically included with the *ERBasics.h* library.

## FastSerial Class

### Class Constructors

All class constructors require an integer as their first parameter. This parameter defines the maximum length of the message that can be received. 

Additionally, all constructors require a serial object as the second parameter to read from and print to. This can be either the arduinos hardware serial port or a software serial object.

Finally, The third parameter must be one of the following:

 - A ReadCallback function (see the [ReadCallback](####readcallback) section for more information).
 - A Puzzle object (if used in conjunction with the rest of the ERBasics Library).

### Functions

 - `void FastSerial.test_serial()` - Prints "Ready" to the provided serial object.

 - `void FastSerial.read_serial()` - Reads bytes from the serial stream until either the message object is filled, or until it reaches a line break character ('\n').

 - `void FastSerial.emulate(char[])` - Takes a c-string and processes it like it is an input from the serial. Useful for interfacing with unsupported methods of communication.

#### read_serial()

the `read_serial()` function will take the bytes read from teh serial and process them into a message. This message is dealt with depending on how the FastSerial object was set up.

 - If the FastSerial object was set up with a ReadCallback, then the message and its length will be sent through to the ReadCallback function.

 - If the FastSerial object was set up with a Puzzle object, then the FastSerial will set the puzzles state based on the first character in the message. The states are as follows:

    |Character|State    |
    |:-------:|:--------|
    |'0'      |Activated|
    |'1'      |Completed|
    |'2'      |Locked   |
    |'3'      |Set      |

`FastSerial.read_serial()` should be called as often as possible so as to not miss a message from the serial.

### ReadCallback

A ReadCallback function is a function which can be used by a FastSerial object to process serial input. It must have a void return type and take a `char*` and an `int` as its parameters. The `char*` is the container for the message received by the serial and the `int` is the length of the message.
e.g.
`void read_message(char* message, int length)`.

# ERTime

*ERTime.h* is another sub library of ERBasics. It can be used by itself but is automatically included with the *ERBasics.h* library.

## _Time Class

*ERTime.h* comes with a _Time class. A global _Time object (aptly named `Time`) is automatically created when including the *ERBasics.h* library (This can be disabled if used with *ERBasics.h* by adding the line `#define ER_NO_TIME` before `#include ERBasics.h`).

### Basic Functions

 - `void Time.update()` - The most important function in the _Time class which makes all other features work. This function should be called at least once every loop.

 - `long Time.delta_time()` - This function returns the time (in milliseconds) between the last `Time.update()` call and the call before that. If `Time.update()` is called every loop, this function will return the time between the previous and current loop.

### Coroutines

Coroutines are functions that can be attached to a _Time object to be called every time `_Time.update()` is called. Additionally, a duration can be defined alongside the coroutine so that it stops after a certain amount of time. A Coroutine function is a function with a `void` return type and a `long` as its parameter (e.g. `void coroutine(long dt)`).

To attach a coroutine function to a _Time object, the `_Time.setup()` function must first be called. Once the _Time object is setup, you can use the function `_Time.start_coroutine(Coroutine function, long duration = 0)`. If a value less than 0 is passed as the duration, the coroutine will only end if it is manually removed.

***Limitations**
- Only one of each function can be added as a coroutine at a time. If you try to add the same function more than once, an error will be thrown.
- Only a limited number of coroutines can exist at one time. By default, this maximum is 1, but it can be increased by adding the line `#define MAX_COROUTINES 4` (replace 4 with however many you need). If this limit is exceeded, an error will be thrown.

#### Ending a coroutine

Coroutines will end automatically after their duration is over. However, they can also be manually ended before that. The `_Time` class comes with the `_Time.end_coroutine` function with two options for parameters:

 - `_Time.end_coroutine(Coroutine function)` will end the coroutine that is passed in as a parameter.

 - `_Time.end_coroutine(int index)` will end the coroutine based on its index in the coroutine list. This is arguably less useful but still exists.

Additionally, a function can be attached to the `_Time` object to be called whenever a coroutine ends (either manually or automatically) using `_Time.attach_cr_end_func(void(*func)(Coroutine))`. This function takes in another function as a parameter.

The function that is passed in must have a void return type and take a coroutine as a parameter (e.g. `void on_coroutine_end(Coroutine function)`). When a coroutine ends, this function will be called and the coroutine will be passed into it.

#### Errors

There are three Errors that can be returned when starting a coroutine (The error will be output to the arduinos hardware serial port).

 - `Couldn't start coroutine. _Time object has not been setup.` - This error means that the `_Time.setup()` function has not been called before trying to start the coroutine.

 - `Couldn't start coroutine. Coroutine already exists.` - This error means that the function is still running as a coroutine on this _Time object.*

 - `Couldn't start coroutine. Coroutine limit exceeded.` - This error means that you've tried to run too many coroutines on the same _Time object.*

Some errors (The errors followed by *) will stop the program from running by default when encountered. To disable this, use the `_Time.set_cr_overload(bool stop)` function (false will let the program continue to run).

The `_Time.start_coroutine(Coroutine function, long duration = 0)` returns an integer based on the error thrown (if any).
|Error value|Error|
|:---:|:---|
|0|None (Coroutine Succesfully started).|
|1|_Time object has not been setup.|
|2|Coroutine already exists.|
|3|Coroutine limit exceeded.|
