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
	display.show();
}

void setup() {
	setTimer(12, 34);
	delay(3000);
}

void loop() {
	static unsigned long prevHundreths = 0;
	static long t = 90000L; // in hundreths of a second
	static bool timerOn = true;

	unsigned long hundreths = millis() / 10;

	// ticking
	if (timerOn && hundreths != prevHundreths) {

		// update timer
		t--;
		prevHundreths = hundreths;

		// update display
		if (t >= 6000) { // one minute
			int min = t / 6000;
			int sec = t % 6000;
			setTimer(min, sec);
		} else if (t > 0) {
			int sec = t / 100;
			int hund = t % 100;
			setTimer(sec, hund);
		} else { // zero
			timerOn = false; // stop
			setTimer(0, 0);
		}
	}

}
