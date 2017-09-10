#include "commands.h"
#include <ShiftDisplay.h>

const int COUNTER_DISPLAY_SIZE = 4;
const int SCORE_DISPLAY_QUANTITY = 5;
const int SCORE_DISPLAY_SIZES[] = {2, 1, 2, 1, 1};
const enum SCORE_DISPLAY_IDS {
	POINTS_HOME,
	PERIOD,
	POINTS_GUEST,
	FOULS_HOME,
	FOULS_GUEST,
}
const int TEN_HOME_INDEX = 5;
const int TEN_GUEST_INDEX = 6;
const int POSSESSION_HOME_INDEX = 0;
const int POSSESSION_GUEST_INDEX = 1;

const int COUNTERS[] = {5, 8}; // in minutes

volatile bool counterOn;
volatile bool counterReset;

const int DISPLAY_TYPE = SINGULAR_CATHODE || UNIQUE_CATHODE; // TODO

ShiftDisplay counterDisplay(DISPLAY_TYPE, COUNTER_DISPLAY_SIZE);
ShiftDisplay scoreDisplay(DISPLAY_TYPE, SCORE_DISPLAY_QUANTITY, SCORE_DISPLAY_SIZES);

void setPointsHome(points) {
	scoreDisplay.setAt(SCORE_DISPLAY_IDS.POINTS_HOME, points);
}

void setPointsGuest(points) {
	scoreDisplay.setAt(POINTS_GUEST, points);
}

void setFoulsHome(fouls) {
	scoreDisplay.setAt(FOULS_HOME, fouls);
	if (fouls > 9)
		scoreDisplay.insertDot(TEN_HOME_INDEX);
	else
		scoreDisplay.removeDot(TEN_HOME_INDEX);
}

void setFoulsGuest(fouls) {
	scoreDisplay.setAt(FOULS_GUEST, fouls);
	if (fouls > 9)
		scoreDisplay.insertDot(TEN_GUEST_INDEX);
	else
		scoreDisplay.removeDot(TEN_GUEST_INDEX);
}

void setPeriod(period) {
	scoreDisplay.setAt(PERIOD, period);
}

void setPossessionHome() {
	scoreDisplay.removeDot(POSSESSION_GUEST_INDEX);
	scoreDisplay.insertDot(POSSESSION_HOME_INDEX);
}

void setPossessionGuest() {
	scoreDisplay.removeDot(POSSESSION_HOME_INDEX);
	scoreDisplay.insertDot(POSSESSION_GUEST_INDEX);
}

void receiveEvent(int size) {
	int cmd = read();
	int arg = size > 1 ? read() : 0;
	swicth (cmd) {
		case TOGGLE_COUNTER:
			counterOn = !counterOn;
			break;
		case RESET_COUNTER:
			counterReset = true;
			break;
		case SET_POINTS_HOME:
			setPointsHome(arg);
			break;
		case SET_POINTS_GUEST:
			setPointsGuest(arg);
			break;
		case SET_FOULS_HOME:
			setFoulsHome(arg);
			break;
		case SET_FOULS_GUEST:
			setFoulsGuest(arg);
			break;
		case SET_PERIOD:
			setPeriod(arg);
			break;
		case SET_POSSESSION_HOME:
			setPossessionHome();
			break;
		case SET_POSSESSION_GUEST:
			setPossessionGuest();
			break;
	}
}

void setup() {
	counterOn = false;
	counterReset = true;
	counterDisplay.set(); // TODO
	onReceive(receiveEvent);
}

void loop() {
	const unsigned long lastTime = 0;

	unsigned long currTime = millis() / 10;

	if (counterOn && currTime != lastTime) {
		lastTime = currTime;

	}

	if (counterReset) {

		/*if (counterOn) {
			counterOn = false;
			int minutes = COUNTERS[counter];
			counterDisplay.set();
		} else {
			counter = (counter + 1) % sizeof(COUNTERS); //TODO sizeof

		}*/
	}
}
