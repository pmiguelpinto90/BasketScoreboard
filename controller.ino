#include "commands.h"

const int MIN_POINTS = 0;
const int MAX_POINTS = 99;
const int DEFAULT_POINTS = 0;
const int MIN_FOULS = 0;
const int MAX_FOULS = 19;
const int DEFAULT_FOULS = 0;
const int MIN_PERIOD = 0;
const int MAX_PERIOD = 9;
const int DEFAULT_PERIOD = 1;
const bool DEFAULT_POSSESSION = false;

volatile int pointsHome, pointsVisit;
volatile int foulsHome, foulsVisit;
volatile bool possessionHome, possessionVisit;
volatile int period;

volatile bool foobarButtonPressed;

void updateDisplay() {
	// TODO
}

void sendCommand(int cmd, int arg = NULL) {
	// TODO
}

void incrementPointsHome() {
	if (pointsHome < MAX_POINTS) {
		pointsHome++;
		updateDisplay();
		sendCommand(SET_POINTS_HOME, pointsHome);
	}
}

void incrementPointsVisit() {
	if (pointsVisit < MAX_POINTS) {
		pointsVisit++;
		updateDisplay();
		sendCommand(SET_POINTS_VISIT , pointsVisit);
	}
}

void decrementPointsHome() {
	if (pointsHome > MIN_POINTS) {
		pointsHome--;
		updateDisplay();
		sendCommand(SET_POINTS_HOME, pointsHome);
	}
}

void decrementPointsVisit() {
	if (pointsVisit > MIN_POINTS) {
		pointsVisit--;
		updateDisplay();
		sendCommand(SET_POINTS_VISIT, pointsVisit);
	}
}

void incrementFoulsHome() {
	if (foulsHome < MAX_FOULS) {
		foulsHome++;
		updateDisplay();
		sendCommand(SET_FOULS_HOME, foulsHome);
	}
}

void incrementFoulsVisit() {
	if (foulsVisit < MAX_FOULS) {
		foulsVisit++;
		updateDisplay();
		sendCommand(SET_FOULS_VISIT, foulsVisit);
	}
}

void decrementFoulsHome() {
	if (foulsHome > MIN_FOULS) {
		foulsHome--;
		updateDisplay();
		sendCommand(SET_FOULS_HOME, foulsHome);
	}
}

void decrementFoulsVisit() {
	if (foulsVisit > MIN_FOULS) {
		foulsVisit--;
		updateDisplay();
		sendCommand(SET_FOULS_VISIT, foulsVisit);
	}
}

void incrementPeriod() {
	if (period < MAX_PERIOD) {
		period++;
		updateDisplay();
		sendCommand(SET_PERIOD, period);
	}
}

void decrementPeriod() {
	if (period > MIN_PERIOD) {
		period--;
		updateDisplay();
		sendCommand(SET_PERIOD, period);
	}
}

void clearPossession() {
	possessionHome = false;
	possessionVisit = false;
	updateDisplay();
	sendCommand(CLEAR_POSSESSION);
}

void setPossessionHome() {
	possessionVisit = false;
	possessionHome = true;
	updateDisplay();
	sendCommand(SET_POSSESSION_HOME);
}

void setPossessionVisit() {
	possessionHome = false;
	possessionVisit = true;
	updateDisplay();
	sendCommand(SET_POSSESSION_VISIT);
}

void setup() {
	pointsHome, pointsVisit = DEFAULT_POINTS;
	foulsHome, foulsVisit = DEFAULT_FOULS;
	period = DEFAULT_PERIOD;
	possessionHome, possessionVisit = DEFAULT_POSSESSION;

	foobarButtonPressed = false;
	// attachInterrupt();
}

void loop() {
	//static int pointsHome = DEFAULT_POINTS;
	
	if (foobarButtonPressed) {
		incrementPointsHome();
		foobarButtonPressed = false;
	}
}
