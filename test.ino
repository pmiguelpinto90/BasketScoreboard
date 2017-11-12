#include <ShiftDisplay.h>

const int TIMER_LATCH_PIN = 1;
const int TIMER_CLOCK_PIN = 1;
const int TIMER_DATA_PIN = 1;
const int SCORE_LATCH_PIN = 1;
const int SCORE_CLOCK_PIN = 1;
const int SCORE_DATA_PIN = 1;

const DisplayType DISPLAY_TYPE = COMMON_CATHODE;
const DisplayDrive DISPLAY_DRIVE = STATIC_DRIVE;

const int TIMER_SECTION_COUNT = 2;
const int TIMER_SECTION_SIZES[] = {2, 2};
enum timerSections {SECTION_LEFT, SECTION_RIGHT};
const int SEPARATOR_INDEX = 1;

const int SCORE_SECTION_COUNT = 5;
const int SCORE_SECTION_SIZES[] = {2, 1, 2, 1, 1};
enum scoreSections {
	SECTION_POINTS_HOME,
	SECTION_PERIOD,
	SECTION_POINTS_VISIT,
	SECTION_FOULS_HOME,
	SECTION_FOULS_VISIT
};
const int POSSESSION_INDEX = 1; // inside SECTION_POINTS_*
const int HUNDRED_INDEX = 0; // inside SECTION_POINTS_*

const int DEFAULT_POINTS = 0;
const int DEFAULT_FOULS = 0;
const int DEFAULT_PERIOD = 0;
const int DEFAULT_TIMER = 2; // in minutes

volatile bool possessionHome;
volatile bool possessionVisit;

ShiftDisplay timerDisplay(TIMER_LATCH_PIN, TIMER_CLOCK_PIN, TIMER_DATA_PIN, DISPLAY_TYPE, TIMER_SECTION_COUNT, TIMER_SECTION_SIZES, DISPLAY_DRIVE);
ShiftDisplay scoreDisplay(SCORE_LATCH_PIN, SCORE_CLOCK_PIN, SCORE_DATA_PIN, DISPLAY_TYPE, SCORE_SECTION_COUNT, SCORE_SECTION_SIZES, DISPLAY_DRIVE);

void setPointsHome(int points) {
	scoreDisplay.setAt(SECTION_POINTS_HOME, points); // only the 2 least significant digits will be set
	if (points > 99)
		scoreDisplay.setDotAt(SECTION_POINTS_HOME, HUNDRED_INDEX); // character '1' is connected as dot
	if (possessionHome)
		scoreDisplay.setDotAt(SECTION_POINTS_HOME, POSSESSION_INDEX); // set home possession symbol because it was cleared
	scoreDisplay.update();
}

void setPointsVisit(int points) {
	scoreDisplay.setAt(SECTION_POINTS_VISIT, points); // only the 2 least significant digits will be set
	if (points > 99)
		scoreDisplay.setDotAt(SECTION_POINTS_VISIT, HUNDRED_INDEX); // character '1' is connected as dot
	if (possessionVisit)
		scoreDisplay.setDotAt(SECTION_POINTS_VISIT, POSSESSION_INDEX); // set visit possession symbol because it was cleared
	scoreDisplay.update();
}

void setFoulsHome(int fouls) {
	scoreDisplay.setAt(SECTION_FOULS_HOME, fouls);
	scoreDisplay.update();
}

void setFoulsVisit(int fouls) {
	scoreDisplay.setAt(SECTION_FOULS_VISIT, fouls);
	scoreDisplay.update();
}

void setPeriod(int period) {
	scoreDisplay.setAt(SECTION_PERIOD, period);
	scoreDisplay.update();
}

void setPossessionHome() {
	scoreDisplay.setDotAt(SECTION_POINTS_HOME, POSSESSION_INDEX); // possession symbols are connected as dots
	scoreDisplay.setDotAt(SECTION_POINTS_VISIT, POSSESSION_INDEX, false);
	scoreDisplay.update();
	possessionHome = true;
	possessionVisit = false;
}

void setPossessionVisit() {
	scoreDisplay.setDotAt(SECTION_POINTS_VISIT, POSSESSION_INDEX); // possession symbols are connected as dots
	scoreDisplay.setDotAt(SECTION_POINTS_HOME, POSSESSION_INDEX, false);
	scoreDisplay.update();
	possessionVisit = true;
	possessionHome = false;
}

void clearPossession() {
	scoreDisplay.setDotAt(SECTION_POINTS_HOME, POSSESSION_INDEX, false); // possession symbols are connected as dots
	scoreDisplay.setDotAt(SECTION_POINTS_VISIT, POSSESSION_INDEX, false);
	scoreDisplay.update();
	possessionHome = false;
	possessionVisit = false;
}

void setTimer(int left, int right, bool sep) {
	timerDisplay.setAt(SECTION_LEFT, left);
	timerDisplay.setAt(SECTION_RIGHT, right, true); // show leading zeros
	if (sep)
		timerDisplay.setDot(SEPARATOR_INDEX); // separator is connected as dot
	timerDisplay.update();
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
