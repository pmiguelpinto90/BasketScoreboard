#include <ShiftDisplay.h>
#include "commands.h"

const int TIMER_LATCH_PIN = 5;
const int TIMER_CLOCK_PIN = 6;
const int TIMER_DATA_PIN = 7;
const int SCORE_LATCH_PIN = 8;
const int SCORE_CLOCK_PIN = 9;
const int SCORE_DATA_PIN = 10;

const DisplayType DISPLAY_TYPE = COMMON_CATHODE;
const DisplayDrive DISPLAY_DRIVE = STATIC_DRIVE;

const int TIMER_SECTION_SIZES[] = {2, 2, 0};
enum timerSections {SECTION_LEFT, SECTION_RIGHT};
const int SEPARATOR_INDEX = 1;

const int SCORE_SECTION_SIZES[] = {2, 1, 2, 1, 1, 0};
enum scoreSections {
	SECTION_POINTS_HOME,
	SECTION_PERIOD,
	SECTION_POINTS_VISIT,
	SECTION_FOULS_HOME,
	SECTION_FOULS_VISIT,
};
const int POSSESSION_INDEX = 1; // inside SECTION_POINTS_*
const int HUNDRED_INDEX = 0; // inside SECTION_POINTS_*

const int DEFAULT_POINTS = 0;
const int DEFAULT_FOULS = 0;
const int DEFAULT_PERIOD = 0;
const int DEFAULT_TIMER = 0;

const int BUZZER_PIN = 2; // TODO
const int BUZZ_TIME = 3000; // in milliseconds // TODO

volatile bool possessionHome;
volatile bool possessionVisit;
volatile bool buzzReq;

ShiftDisplay timerDisplay(TIMER_LATCH_PIN, TIMER_CLOCK_PIN, TIMER_DATA_PIN, DISPLAY_TYPE, TIMER_SECTION_SIZES, DISPLAY_DRIVE);
ShiftDisplay scoreDisplay(SCORE_LATCH_PIN, SCORE_CLOCK_PIN, SCORE_DATA_PIN, DISPLAY_TYPE, SCORE_SECTION_SIZES, DISPLAY_DRIVE);

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

void receiveEvent(int size) {
	Command cmd = /*read()*/NULL; // TODO
	int param1 = size > 1 ? /*read()*/0 : 0; // TODO
	int param2 = size > 2 ? /*read()*/0 : 0; // TODO
	switch (cmd) {
		case SET_TIMER:
			setTimer(param1, param2, true); // TODO
			break;
		case SET_POINTS_HOME:
			setPointsHome(param1);
			break;
		case SET_POINTS_VISIT:
			setPointsVisit(param1);
			break;
		case SET_FOULS_HOME:
			setFoulsHome(param1);
			break;
		case SET_FOULS_VISIT:
			setFoulsVisit(param1);
			break;
		case SET_PERIOD:
			setPeriod(param1);
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
			buzzReq = true;
			break;
	}
}

void setup() {
	possessionHome, possessionVisit, buzzReq = false;

	// initialize display values
	setTimer(DEFAULT_TIMER, DEFAULT_TIMER, false); // TODO
	setPointsHome(DEFAULT_POINTS);
	setPointsVisit(DEFAULT_POINTS);
	setFoulsHome(DEFAULT_FOULS);
	setFoulsVisit(DEFAULT_FOULS);
	setPeriod(DEFAULT_PERIOD);
	clearPossession();

	//onReceive(receiveEvent); // TODO
}

void loop() {
	if (buzzReq) {
		digitalWrite(BUZZER_PIN, HIGH); // TODO
		delay(BUZZ_TIME);
		digitalWrite(BUZZER_PIN, LOW);
		buzzReq = false;
	}
}
