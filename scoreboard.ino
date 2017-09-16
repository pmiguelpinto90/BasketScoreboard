#include <ShiftDisplay.h>
#include "commands.h"

const int DISPLAY_TYPE = INDIVIDUAL_ANODE; // TODO in lib
const int COUNTER_DISPLAY_SIZE = 4;
const int SCORE_SECTION_COUNT = 5;
const int SCORE_SECTION_SIZES[] = {2, 1, 2, 1, 1};
enum sections { // TODO
	SECTION_POINTS_HOME,
	SECTION_PERIOD,
	SECTION_POINTS_VISIT,
	SECTION_FOULS_HOME,
	SECTION_FOULS_VISIT,
};

const int COUNTERS_COUNT = 2;
const int COUNTERS[] = {5, 8}; // in minutes
const int DEFAULT_COUNTER_ID = 0;

volatile bool counterOn;
volatile bool buttonCounterReset;
volatile bool possessionHome;
volatile bool possessionVisit;

ShiftDisplay counterDisplay(DISPLAY_TYPE, COUNTER_DISPLAY_SIZE);
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
		case TOGGLE_COUNTER:
			counterOn = !counterOn;
			break;
		case RESET_COUNTER:
			buttonCounterReset = true;
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
	counterOn, buttonCounterReset = false;
	possessionHome, possessionVisit = false;
	counterDisplay.set("0000"); // TODO
	//onReceive(receiveEvent); // TODO
}

void loop() {
	static int counterId = DEFAULT_COUNTER_ID;
	static long counter = COUNTERS[counterId] * 60 * 100; 
	static bool counterSet = true;
	static unsigned long lastTime = 0;

	unsigned long currTime = millis() / 10;

	if (counterOn && currTime != lastTime) {
		counter--;
		if (counter == 0) {
			counterDisplay.set("0000");
			counterOn = false;
		} else if (counter < 10) {
			//TODO
			counterDisplay.set();
		} else if (counter < 100) {
			//TODO
			counterDisplay.set();
		} else if (counter < 6000) {
			//TODO
			counterDisplay.set();
		} else {
			int seconds = counter / 100;
			int min = seconds / 60;
			int sec = seconds % 60;
			//TODO
			counterDisplay.set();
		}

		counterSet = false;
		lastTime = currTime;
	}

	if (buttonCounterReset) { // button pressed
		if (counterSet) {
			// iterate counter time
			counterId = (counterId + 1) % COUNTERS_COUNT;
			int minutes = COUNTERS[counterId];
			//TODO
			counterDisplay.set();
		} else {
			// stop and reset counter
			counterOn = false;
			counterSet = true;
			int minutes = COUNTERS[counterId];
			//TODO
			counterDisplay.set();
		}
		buttonCounterReset = false;
	}
}
