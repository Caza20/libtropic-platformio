#include <Arduino.h>
#include "main.hpp"


Libtropic tropic;

// The Pico W LED is GPIO25, same for the regular Pico.
#define LED_PIN 25

extern "C" {
    #include "pico/rand.h"
    #include <stdint.h>

    uint32_t random32(void) {
        return get_rand_32();
    }
}

void setup() {
    Serial.begin(115200);

    pinMode(LED_PIN, OUTPUT);

    delay(5000);
    Serial.println("=== Raspberry Pi Pico + libtropic ejemplo ===");

    tropic.begin();
}

void loop() {
    digitalWrite(LED_PIN, 1);
    delay(100);
    digitalWrite(LED_PIN, 0);
    delay(500);
}