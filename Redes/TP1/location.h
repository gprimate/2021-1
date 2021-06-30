#ifndef LOCATION
#define LOCATION

#include <math.h>
#include <stdio.h>
#include "constants.h"

typedef struct {
    int x;
    int y;
} Location;


int checkIfLocationIsValid(Location location);

int isEmpty(Location locations[MAX_NUMBER_OF_LOCATIONS]);

int checkIfLocationIsSaved(Location location, 
    Location locations[MAX_NUMBER_OF_LOCATIONS]);

double getDistanceBetweenLocations(Location locationA, Location locationB);

void initializeLocations(Location locations[MAX_NUMBER_OF_LOCATIONS]);

int getEmptyLocationIndex(Location locarions[MAX_NUMBER_OF_LOCATIONS]);

int getFirstEmptyPlace(Location location,Location locations[MAX_NUMBER_OF_LOCATIONS]);

int getLocationIndex(Location location, 
    Location locations[MAX_NUMBER_OF_LOCATIONS]);

void addNewLocation(Location location, Location locations[MAX_NUMBER_OF_LOCATIONS]);

#endif