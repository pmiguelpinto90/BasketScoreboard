#include <ShiftDisplay.h>

const int TIMER_LATCH_PIN = 1;
const int TIMER_CLOCK_PIN = 1;
const int TIMER_DATA_PIN = 1;
const int SCORE_LATCH_PIN = 1;
const int SCORE_CLOCK_PIN = 1;
const int SCORE_DATA_PIN = 1;

const DisplayType DISPLAY_TYPE = INDIVIDUAL_CATHODE;
const int TIMER_DISPLAY_SIZE = 4;
const int SCORE_SECTION_COUNT = 5;
const int SCORE_SECTION_SIZES[] = {2, 1, 2, 1, 1};
enum sections {
	SECTION_POINTS_HOME,
	SECTION_PERIOD,
	SECTION_POINTS_VISIT,
	SECTION_FOULS_HOME,
	SECTION_FOULS_VISIT,
};

const int DEFAULT_POINTS = 0;
const int DEFAULT_FOULS = 0;
const int DEFAULT_PERIOD = 0;
const int DEFAULT_TIMER = 12; // in minutes
const int MIN_TIMER = 1; // in minutes
const int MAX_TIMER = 20; // in minutes

volatile bool possessionHome;
volatile bool possessionVisit;

ShiftDisplay timerDisplay(TIMER_LATCH_PIN, TIMER_CLOCK_PIN, TIMER_DATA_PIN, DISPLAY_TYPE, TIMER_DISPLAY_SIZE);
ShiftDisplay scoreDisplay(SCORE_LATCH_PIN, SCORE_CLOCK_PIN, SCORE_DATA_PIN, DISPLAY_TYPE, SCORE_SECTION_COUNT, SCORE_SECTION_SIZES);

void setPointsHome(int points) {
	scoreDisplay.setAt(SECTION_POINTS_HOME, points); // only the least 2 significant digits will be set
	scoreDisplay.setDotAt(SECTION_POINTS_HOME, 0, points > 99); // character '1' is connected as the dot in home points index 0
	scoreDisplay.setDotAt(SECTION_POINTS_HOME, 1, possessionHome); // home possession symbol is connected as the dot in home points index 1
	scoreDisplay.show();
}

void setPointsVisit(int points) {
	scoreDisplay.setAt(SECTION_POINTS_VISIT, points); // only the least 2 significant digits will be set
	scoreDisplay.setDotAt(SECTION_POINTS_VISIT, 0, points > 99); // character '1' is connected as the dot in visit points index 0
	scoreDisplay.setDotAt(SECTION_POINTS_VISIT, 1, possessionVisit); // visit possession symbol is connected as the dot in visit points index 1
	scoreDisplay.show();
}

void setFoulsHome(int fouls) {
	scoreDisplay.setAt(SECTION_FOULS_HOME, fouls);
	scoreDisplay.show();
}

void setFoulsVisit(int fouls) {
	scoreDisplay.setAt(SECTION_FOULS_VISIT, fouls);
	scoreDisplay.show();
}

void setPeriod(int period) {
	scoreDisplay.setAt(SECTION_PERIOD, period);
	scoreDisplay.show();
}

void setPossessionHome() {
	scoreDisplay.setDotAt(SECTION_POINTS_VISIT, 1, false);
	scoreDisplay.setDotAt(SECTION_POINTS_HOME, 1, true);
	scoreDisplay.show();
	possessionVisit = false;
	possessionHome = true;
}

void setPossessionVisit() {
	scoreDisplay.setDotAt(SECTION_POINTS_HOME, 1, false);
	scoreDisplay.setDotAt(SECTION_POINTS_VISIT, 1, true);
	scoreDisplay.show();
	possessionHome = false;
	possessionVisit = true;
}

void clearPossession() {
	scoreDisplay.setDotAt(SECTION_POINTS_HOME, 1, false);
	scoreDisplay.setDotAt(SECTION_POINTS_VISIT, 1, false);
	scoreDisplay.show();
	possessionHome = false;
	possessionVisit = false;
}

void setTimer(int left, int right) {
	int n = left * 100 + right;
	timerDisplay.set(n);
	timerDisplay.show();
}

void setTimer(int left) {
	setTimer(left, 0);
}

void setup() {
	// initialize display values
	setPointsHome(DEFAULT_POINTS);
	setPointsVisit(DEFAULT_POINTS);
	setFoulsHome(DEFAULT_FOULS);
	setFoulsVisit(DEFAULT_FOULS);
	setPeriod(DEFAULT_PERIOD);
	setTimer(DEFAULT_TIMER);
}

void loop() {
	static unsigned long prevHundreths = 0;
	static long t = (long) DEFAULT_TIMER * 6000; // in hundreths of a second
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
			setTimer(0);
		}
	}

}
