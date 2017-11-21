#include <ShiftDisplay.h>
#include "commands.h"

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

const int BUZZER_PIN = 2; // TODO
const int BUZZ_TIME = 3000; // in milliseconds // TODO

volatile bool timerOn;
volatile bool resetTimer;
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

void buzz() { // TODO
	digitalWrite(BUZZER_PIN, HIGH);
	delay(BUZZ_TIME);
	digitalWrite(BUZZER_PIN, LOW);
}

void receiveEvent(int size) {
	Command cmd = /*read()*/NULL; // TODO
	int param = size > 1 ? /*read()*/0 : 0; // TODO
	switch (cmd) {
		case TOGGLE_TIMER:
			timerOn = !timerOn;
			break;
		case RESET_TIMER:
			resetTimer = true;
			break;
		case SET_POINTS_HOME:
			setPointsHome(param);
			break;
		case SET_POINTS_VISIT:
			setPointsVisit(param);
			break;
		case SET_FOULS_HOME:
			setFoulsHome(param);
			break;
		case SET_FOULS_VISIT:
			setFoulsVisit(param);
			break;
		case SET_PERIOD:
			setPeriod(param);
			break;
		case SET_POSSESSION_HOME:
			setPossessionHome();
			break;
		case SET_POSSESSION_VISIT:
			setPossessionVisit();
			break;
		case CLEAR_POSSESSION:
			clearPossession();
			break;
		case BUZZ:
			buzz();
			break;
	}
}

void setup() {
	// initialize display values
	setPointsHome(DEFAULT_POINTS);
	setPointsVisit(DEFAULT_POINTS);
	setFoulsHome(DEFAULT_FOULS);
	setFoulsVisit(DEFAULT_FOULS);
	setPeriod(DEFAULT_PERIOD);
	setTimer(DEFAULT_TIMER);

	//onReceive(receiveEvent); // TODO
}

void loop() {
	static int timerSetting = DEFAULT_TIMER;
	static bool timerSet = true;
	static unsigned long prevHundreths = 0;
	static long t = timerSetting * 6000; // in hundreths of a second

	unsigned long hundreths = millis() / 10;

	// timer on and ticking
	if (timerOn && hundreths != prevHundreths) {

		// update timer
		t--;
		prevHundreths = hundreths;
		timerSet = false;

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
			buzz();
		}
	}

	// button pressed
	if (resetTimer) {
		if (timerSet) {

			// iterate timer setting
			timerSetting++;
			if (timerSetting == MAX_TIMER)
				timerSetting = MIN_TIMER;
		} else {

			// stop and reset timer
			timerOn = false;
			timerSet = true;
		}

		// update timer and display
		t = timerSetting * 6000;
		setTimer(timerSetting);

		// clear button pressed
		resetTimer = false;
	}
}
