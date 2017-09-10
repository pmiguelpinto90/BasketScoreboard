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

volatile int pointsHome, pointsGuest;
volatile int foulsHome, foulsGuest;
volatile int period;
volatile bool possessionHome;

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

void incrementPointsGuest() {
	if (pointsGuest < MAX_POINTS)
		pointsGuest++;
}

void decrementPointsHome() {
	if (pointsHome > MIN_POINTS)
		pointsHome--;
}

void decrementPointsGuest() {
	if (pointsGuest > MIN_POINTS)
		pointsGuest--;
}

void incrementFoulsHome() {
	if (foulsHome < MAX_FOULS)
		foulsHome++;
}

void incrementFoulsGuest() {
	if (foulsGuest < MAX_FOULS)
		foulsGuest++;
}

void decrementFoulsHome() {
	if (foulsHome > MIN_FOULS)
		foulsHome--;
}

void decrementFoulsGuest() {
	if (foulsGuest > MIN_FOULS)
		foulsGuest--;
}

void incrementPeriod() {
	if (period < MAX_PERIOD)
		period++;
}

void decrementPeriod() {
	if (period > MIN_PERIOD)
		period--;
}

void togglePossession() {
	possessionHome = !possessionHome;
}

void setPossessionHome() {
	possessionHome = true;
}

void setPossessionGuest() {
	possessionHome = false;
}

void setup() {
	pointsHome, pointsGuest = DEFAULT_POINTS;
	foulsHome, foulsGuest = DEFAULT_FOULS;
	period = DEFAULT_PERIOD;
	possessionHome = true;
	// attachInterrupt();
}

void loop() {
	// static int pointsHome = DEFAULT_POINTS;
	//if button
	//incrementfoo
}
