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
const int DEFAULT_TIMER = 2; // in minutes

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

void setTimer(int left, int right, bool sep) {
	int n = left * 100 + right;
	timerDisplay.set(n);
	timerDisplay.setDot(1, sep); // separator connected as dot at index 1 of timer display
	timerDisplay.show(); // display.update();
}

void setTimer(int left) {
	setTimer(left, 0, true);
}

void setup() {

	for (int i = 0; i <= 9; i++) {
		setPointsHome(i);
		delay(400);
	}
	for (int i = 19; i <= 99; i += 10) {
		setPointsHome(i);
		delay(400);
	}
	setPointsHome(199);
	delay(400);
	setPointsHome(DEFAULT_POINTS);

	for (int i = 0; i <= 9; i++) {
		setPointsVisit(i);
		delay(400);
	}
	for (int i = 19; i <= 99; i += 10) {
		setPointsVisit(i);
		delay(400);
	}
	setPointsVisit(199);
	delay(400);
	setPointsVisit(DEFAULT_POINTS);

	for (int i = 0; i <= 9; i++) {
		setFoulsHome(i);
		delay(400);
	}
	setFoulsHome(DEFAULT_FOULS);

	for (int i = 0; i <= 9; i++) {
		setFoulsVisit(i);
		delay(400);
	}
	setFoulsVisit(DEFAULT_FOULS);

	for (int i = 0; i <= 9; i++) {
		setPeriod(i);
		delay(400);
	}
	setPeriod(DEFAULT_FOULS);

	for (int i = 0; i <= 9; i++) {
		setTimer(0, i, true);
		delay(400);
	}
	for (int i = 19; i <= 99; i += 10) {
		setTimer(0, i, true);
		delay(400);
	}
	for (int i = 0; i <= 9; i++) {
		setTimer(i, 99, true);
		delay(400);
	}
	for (int i = 19; i <= 99; i += 10) {
		setTimer(i, 99, true);
		delay(400);
	}
	setTimer(DEFAULT_TIMER);

	setPossessionHome();
	delay(400);
	setPossessionVisit();
	delay(400);
	clearPossession();
	delay(400);
	setPossessionVisit();
	delay(400);
	setPossessionHome();
	delay(400);
	clearPossession();
	delay(400);
}

void loop() {
	static unsigned long prevTick = 0;
	static long prevSeconds = 0;
	static long hundreths = DEFAULT_TIMER * 60 * 100; // timer value in hundreths of a second
	static bool timerOn = true;

	unsigned long tick = millis() / 10; // in hundreths of a second

	// ticking
	if (timerOn && tick != prevTick) {

		// update timer
		hundreths--;
		int seconds = hundreths / 100;

		// update display
		if (seconds >= 60) { // greater or equal one minute
			if (seconds != prevSeconds) {
				int mins = seconds / 60;
				int secs = seconds % 60;
				setTimer(mins, secs, true);
				prevSeconds = seconds;
			}
		} else if (hundreths > 0) { // less than one minute
			int secs = seconds;
			int hunds = hundreths % 100;
			setTimer(secs, hunds, true);
		} else { // zero
			timerOn = false; // stop
			setTimer(0);
		}

		prevTick = tick;
	}

}
