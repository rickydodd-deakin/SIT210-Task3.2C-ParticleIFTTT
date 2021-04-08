#include <stdlib.h>
#include <math.h>
#include <BH1750.h>

BH1750 BH(0x23);

// Checks if there's a difference between current state and the new state. Acts on the difference if there is. Regardless of difference or not, returns the "new" state, even if it's the same state.
bool isThereDifference(bool previousInSunlight, bool newInSunlight) {
    if (previousInSunlight != newInSunlight) {
        if (newInSunlight) {
            Particle.publish("sunlight", "entered", PRIVATE); // publish the event, along with the data that it has entered the sunlight.
        } else {
            Particle.publish("sunlight", "left", PRIVATE); // publish the event, along with the data that it has left the sunlight.
        }
    }
    return newInSunlight;
}

// Checks if the lux is within the adequate amount of sunlight range or not.
bool checkSunlight(uint16_t lux) {
    if(lux >= 20000) {
        // The terrarium is in an adequate amount of sunlight
        return true;
    } else {
        // The terrarium is not in an adequate amount of sunlight
        return false;
    }
}

void setup() {
    BH.begin();
}

void loop() {
    // unsigned 16-bit integer because intended range is 1 ~ 65535, per the datasheet of the sensor
    uint16_t lux;
    bool inSunlight = false;
    
    delay(300000);
    
    // Sunlight through a window pane should be between 20,000 ~ 50,000 lux.
    // If my sensor were working, I'd instead write lux = BH.readLightLevel();
    lux = 25000;
    
    // Prevents continuing if lux is nan, included this as I would do so if I had a working sensor for the task.
    if (isnan(lux)) {
        return;
    }
    
    inSunlight = isThereDifference(inSunlight, checkSunlight(lux));

    delay(300000); // five minutes until state change

    lux = 500; // value that puts the terrarium out of the adequate amount of sunlight
    inSunlight = isThereDifference(inSunlight, checkSunlight(lux));
}
