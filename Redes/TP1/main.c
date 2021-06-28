#include "location.h"

int main(int argc, char const *argv[]) {

    Location locations[MAX_NUMBER_OF_LOCATIONS];

    initializeLocations(locations);


    printf("%d pra ver se inicializou\n", locations[25].y);

    locations[0].x = 0;
    locations[0].y = 0;

    locations[1].x = 4;
    locations[1].y = 3;


    printf("%d é igual a 2?\n", getEmptyLocationIndex(locations));

    printf("%d é igual a 1?\n", getLocationIndex(locations[1], locations));

    printf("Distance = %f \n", getDistanceBetweenLocations(locations[0], locations[1]));

    Location a;
    a.x = 0;
    a.y = 0;

    Location b;
    b.x = -1;
    b.y = 0;

    if(checkIfLocationIsSaved(a, locations)) {
        printf("tá salva, certinho \n");
    }

    
    Location c;
    c.x = 0;
    c.y = 99999;

    Location d;
    d.x = -5;
    d.y = 99999;

    if(checkIfLocationIsValid(a)) {
        printf("check certo \n");
    }

    
    
    

    return 0;
}
