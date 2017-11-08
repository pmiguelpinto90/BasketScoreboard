#include <ShiftDisplay.h>

const int LATCH_PIN = 6;
const int CLOCK_PIN = 7;
const int DATA_PIN = 5;

const DisplayType DISPLAY_TYPE = INDIVIDUAL_CATHODE;
const int DISPLAY_SIZE = 4;

ShiftDisplay display(LATCH_PIN, CLOCK_PIN, DATA_PIN, DISPLAY_TYPE, DISPLAY_SIZE);

void setTimer(int left, int right) {
	int n = left * 100 + right;
	display.set(n);
	display.show(); // display.update();
}

void setup() {
	setTimer(12, 34);
	delay(2000);
}

void loop() {
	static unsigned long prevTick = 0;
	static long prevSeconds = 0;
	static long hundreths = (2*60 + 30) * 100; // timer value in hundreths of a second
	static bool timerOn = true;

	unsigned long tick = millis() / 10; // in hundreths of a second

	// ticking
	if (timerOn && tick != prevTick) {

		// update timer
		hundreths--;
		int seconds = hundreths / 100;

		// update display
		if (seconds >= 60 && seconds != prevSeconds) { // greater or equal one minute
			int min = seconds / 60;
			int sec = seconds % 60;
			setTimer(min, sec);
			prevSeconds = seconds;
		} else if (hundreths > 0) { // less than one minute
			int sec = seconds;
			int hund = hundreths % 100;
			setTimer(sec, hund);
		} else { // zero
			timerOn = false; // stop
			setTimer(0, 0);
		}

		prevTick = tick;
	}

}
