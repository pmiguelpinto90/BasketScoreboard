#include <ShiftDisplay.h>
#include "commands.h"

const int COUNTER_DISPLAY_SIZE = 4;
const int SCORE_DISPLAY_QUANTITY = 5; // TODO 
const int SCORE_DISPLAY_SIZES[] = {2, 1, 2, 1, 1}; // TODO 
enum ScoreDisplayId { // TODO 
	POINTS_HOME,
	PERIOD,
	POINTS_VISIT,
	FOULS_HOME,
	FOULS_VISIT,
};
const int TEN_HOME_INDEX = 5; // digit index where the one from home fouls is connected
const int TEN_VISIT_INDEX = 6; // digit index where the one from visit fouls is connected
const int POSSESSION_HOME_INDEX = 0; // digit index where home possession symbol is connected
const int POSSESSION_VISIT_INDEX = 1; // digit index where visit possession symbol is connected

const int COUNTERS[] = {5, 8}; // in minutes TODO 

volatile bool counterOn;
volatile bool counterReset;

const int DISPLAY_TYPE = 0; // TODO in lib shiftdisplay SINGULAR_CATHODE UNIQUE_CATHODE

ShiftDisplay counterDisplay(DISPLAY_TYPE, COUNTER_DISPLAY_SIZE);
ShiftDisplay scoreDisplay(DISPLAY_TYPE, SCORE_DISPLAY_QUANTITY, SCORE_DISPLAY_SIZES);

void setPointsHome(int points) {
	scoreDisplay.setAt(POINTS_HOME, points);
}

void setPointsVisit(int points) {
	scoreDisplay.setAt(POINTS_VISIT, points);
}

void setFoulsHome(int fouls) {
	scoreDisplay.setAt(FOULS_HOME, fouls);
	if (fouls > 9)
		scoreDisplay.insertDot(TEN_HOME_INDEX);
	else
		scoreDisplay.removeDot(TEN_HOME_INDEX);
}

void setFoulsVisit(int fouls) {
	scoreDisplay.setAt(FOULS_VISIT, fouls);
	if (fouls > 9)
		scoreDisplay.insertDot(TEN_VISIT_INDEX); // TODO func setDot at lib shiftdisplay
	else
		scoreDisplay.removeDot(TEN_VISIT_INDEX);
}

void setPeriod(int period) {
	scoreDisplay.setAt(PERIOD, period);
}

void setPossessionHome() {
	scoreDisplay.removeDot(POSSESSION_VISIT_INDEX);
	scoreDisplay.insertDot(POSSESSION_HOME_INDEX);
}

void setPossessionVisit() {
	scoreDisplay.removeDot(POSSESSION_HOME_INDEX);
	scoreDisplay.insertDot(POSSESSION_VISIT_INDEX);
}

void clearPossession() {
	scoreDisplay.removeDot(POSSESSION_HOME_INDEX);
	scoreDisplay.removeDot(POSSESSION_VISIT_INDEX);
}

void receiveEvent(int size) {
	int cmd = 0; //read(); // TODO
	int arg = 0; //size > 1 ? read() : 0; // TODO
	switch (cmd) {
		case TOGGLE_COUNTER:
			counterOn = !counterOn;
			break;
		case RESET_COUNTER:
			counterReset = true;
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
	counterOn, counterReset = false;
	counterDisplay.set("0000"); // TODO
	//onReceive(receiveEvent); // TODO
}

void loop() {
	static unsigned long lastTime = 0;
	static bool counterSet = true;

	unsigned long currTime = millis() / 10;

	if (counterOn && currTime != lastTime) {
		lastTime = currTime;
		counterSet = false;
		// TODO update counterDisplay
	}

	if (counterReset) { // button pressed
		if (counterSet) {
			// iterate counter time
			/*counter = (counter + 1) % sizeof(COUNTERS); //TODO sizeof
			int minutes = COUNTERS[counter];
			counterDisplay.set();*/
		} else {
			// stop and reset counter
			counterOn = false;
			counterSet = true;
			/*int minutes = COUNTERS[counter];
			counterDisplay.set();*/
		}
		counterReset = false;
	}
}
