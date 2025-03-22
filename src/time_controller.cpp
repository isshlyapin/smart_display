#include <ArduinoNvs.h>

#include "time_controller.h"

void timeSetup() {
    if (NVS.begin()) {
        Serial.println("timeSetup: NVS opened");
    } else {
        Serial.println("timeSetup: Error opening NVS");
    }

    String timezone = NVS.getString("timezone");
    if (timezone) {
        // configTime(atoi(timezone.c_str()) * 3600, 0, "pool.ntp.org", "time.nist.gov");
        configTime(0, 0, "pool.ntp.org", "time.nist.gov");
    } else {
        configTime(0, 0, "pool.ntp.org", "time.nist.gov");
    }
    NVS.close();
}
