#include "location.h"
#include <math.h>

int checkIfLocationIsValid(Location location) {

    if (location.x < MIN_XY_VALUE || location.y < MIN_XY_VALUE ||
        location.x > MAX_XY_VALUE || location.y > MAX_XY_VALUE) {

        return 0;
    }
    return 1;
}


int checkIfLocationIsSaved(Location location, 
    Location locations[MAX_NUMBER_OF_LOCATIONS]) {

    for (size_t i = 0; i < MAX_NUMBER_OF_LOCATIONS; i++) {
        if (location.x == locations[i].x) {

            if (location.y == locations[i].y) {
                return 1;
            }
        }
    }
    return 0;
}


double getDistanceBetweenLocations(Location locationA, Location locationB) {

    return sqrt(pow((double) locationA.x + locationB.x, POWER_OF_2) + 
                pow((double) locationA.y + locationB.y, POWER_OF_2));
}


void initializeLocations(Location locations[MAX_NUMBER_OF_LOCATIONS]) {

    for(size_t i = 0; i < MAX_NUMBER_OF_LOCATIONS; i++) {
        locations[i].x = -1;
        locations[i].y = -1;
    }
}


int getEmptyLocationIndex(Location locations[MAX_NUMBER_OF_LOCATIONS]) {

    for (size_t i = 0; i < MAX_NUMBER_OF_LOCATIONS; i++) {
        if (locations[i].x == -1) {
            return i;
        }
    }
    return -1;
}


int getLocationIndex(Location location, 
    Location locations[MAX_NUMBER_OF_LOCATIONS]) {

    for (size_t i = 0; i < MAX_NUMBER_OF_LOCATIONS; i++) {
        if (location.x == locations[i].x) {

            if (location.y == locations[i].y) {
                return i;
            }
        }
    }
    return -1;
}

int isEmpty(Location locations[MAX_NUMBER_OF_LOCATIONS]) {

    for (size_t i = 0; i < MAX_NUMBER_OF_LOCATIONS; i++) {
        if (checkIfLocationIsValid(locations[i])) {
            return 0;
        }
    }
    return 1;
}