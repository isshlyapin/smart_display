#pragma once

#include <DHT.h>

#include "config.h"

class TemperatureController {
public:
    static TemperatureController& getInstance() {
        static TemperatureController instance;
        return instance;
    }
    
    float getTemperature() { return dht.readTemperature();       }
    float getHumidity()    { return dht.readHumidity();          }
    float getHeatIndex()   { return dht.computeHeatIndex(false); }

private:
    TemperatureController() : dht(DHT_PIN, DHT_TYPE) {
        dht.begin();
    }

    TemperatureController(TemperatureController const&) = delete;
    void operator=(TemperatureController const&) = delete;

private:
    DHT dht;
};
    