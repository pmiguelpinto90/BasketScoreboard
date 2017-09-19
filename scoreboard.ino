#include <ShiftDisplay.h>
#include "commands.h"

const int DISPLAY_TYPE = INDIVIDUAL_ANODE;
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

const int MIN_TIMER = 1; // in minutes
const int MAX_TIMER = 20; // in minutes
const int DEFAULT_TIMER = 12; // in minutes

volatile bool timerOn;
volatile bool buttonTimerReset;
volatile bool possessionHome;
volatile bool possessionVisit;

ShiftDisplay timerDisplay(DISPLAY_TYPE, TIMER_DISPLAY_SIZE);
ShiftDisplay scoreDisplay(DISPLAY_TYPE, SCORE_SECTION_COUNT, SCORE_SECTION_SIZES);

void setPointsHome(int points) {
	scoreDisplay.setAt(SECTION_POINTS_HOME, points); // only the least 2 significant digits will be set
	scoreDisplay.setDotAt(SECTION_POINTS_HOME, 1, points > 99); // character '1' is connected as the dot in home points index 1
	scoreDisplay.setDotAt(SECTION_POINTS_HOME, 0, possessionHome); // home possession symbol is connected as the dot in home points index 0
}

void setPointsVisit(int points) {
	scoreDisplay.setAt(SECTION_POINTS_VISIT, points); // only the least 2 significant digits will be set
	scoreDisplay.setDotAt(SECTION_POINTS_VISIT, 1, points > 99); // character '1' is connected as the dot in visit points index 1
	scoreDisplay.setDotAt(SECTION_POINTS_VISIT, 0, possessionVisit); // visit possession symbol is connected as the dot in visit points index 0
}

void setFoulsHome(int fouls) {
	scoreDisplay.setAt(SECTION_FOULS_HOME, fouls);
}

void setFoulsVisit(int fouls) {
	scoreDisplay.setAt(SECTION_FOULS_VISIT, fouls);
}

void setPeriod(int period) {
	scoreDisplay.setAt(SECTION_PERIOD, period);
}

void setPossessionHome() {
	scoreDisplay.setDotAt(SECTION_POINTS_VISIT, 0, false);
	scoreDisplay.setDotAt(SECTION_POINTS_HOME, 0, true);
	possessionVisit = false;
	possessionHome = true;
}

void setPossessionVisit() {
	scoreDisplay.setDotAt(SECTION_POINTS_HOME, 0, false);
	scoreDisplay.setDotAt(SECTION_POINTS_VISIT, 0, true);
	possessionHome = false;
	possessionVisit = true;
}

void clearPossession() {
	scoreDisplay.setDotAt(SECTION_POINTS_HOME, 0, false);
	scoreDisplay.setDotAt(SECTION_POINTS_VISIT, 0, false);
	possessionHome = false;
	possessionVisit = false;
}

void receiveEvent(int size) {
	int cmd = 0; //read(); // TODO
	int arg = 0; //size > 1 ? read() : 0; // TODO
	switch (cmd) {
		case TOGGLE_TIMER:
			timerOn = !timerOn;
			break;
		case RESET_TIMER:
			buttonTimerReset = true;
			break;
		case SET_POINTS_HOME:
			setPointsHome(arg);
			break;
		case SET_POINTS_VISIT:
			setPointsVisit(arg);
			break;
		case SET_FOULS_HOME:
			setFoulsHome(arg);
			break;
		case SET_FOULS_VISIT:
			setFoulsVisit(arg);
			break;
		case SET_PERIOD:
			setPeriod(arg);
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
	}
}

void setup() {
	timerOn, buttonTimerReset = false;
	possessionHome, possessionVisit = false;
	timerDisplay.set("0000"); // TODO
	//onReceive(receiveEvent); // TODO
}

void loop() {
	static long timer = DEFAULT_TIMER * 60 * 100; // in hundreths of a second
	static bool timerSet = true;
	static unsigned long lastUpdate = 0;

	unsigned long thisUpdate = millis() / 10;

	if (timerOn && thisUpdate != lastUpdate) {
		timer--;
		if (timer == 0) {
			timerDisplay.set("0000");
			timerOn = false;
		} else if (timer < 10) {
			//TODO
			timerDisplay.set();
		} else if (timer < 100) {
			//TODO
			timerDisplay.set();
		} else if (timer < 6000) {
			//TODO
			timerDisplay.set();
		} else {
			int seconds = timer / 100;
			int min = seconds / 60;
			int sec = seconds % 60;
			//TODO
			timerDisplay.set();
		}

		timerSet = false;
		lastUpdate = thisUpdate;
	}

	if (buttonTimerReset) { // button pressed
		if (timerSet) {
			// iterate timer setting
			timer++;
			if (timer == MAX_TIMER)
				timer = MIN_TIMER;
			//TODO
			timerDisplay.set();
		} else {
			// stop and reset timer
			timerOn = false;
			timerSet = true;
			int minutes = TIMERS[timerId];
			//TODO
			timerDisplay.set();
		}
		buttonTimerReset = false;
	}
}
