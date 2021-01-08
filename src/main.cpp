#include <Arduino.h>
#include <car.cpp>

Car car = Car();

void setup() {
    Serial.begin(9600);
    pinMode(LED_BUILTIN, OUTPUT);
    pinMode(9, OUTPUT);
    pinMode(10, OUTPUT);
    pinMode(11, OUTPUT);
    pinMode(12, OUTPUT);
    pinMode(30, OUTPUT);
    pinMode(31, OUTPUT);
    pinMode(32, OUTPUT);
    pinMode(33, OUTPUT);
    pinMode(36, OUTPUT);
    pinMode(37, OUTPUT);
    pinMode(38, OUTPUT);
    pinMode(39, OUTPUT);
    Serial.println("start");
    pinMode(7, INPUT);
    pinMode(6, INPUT);
    pinMode(5, INPUT);
    pinMode(4, INPUT);
}
void loop() {

    if (Serial.available() > 0) {
        String request = Serial.readStringUntil('\n');

        int delimiterIndex = request.indexOf("#");

        String direction = request.substring(0, delimiterIndex);

        uint8_t speed = request.substring(delimiterIndex + 1).toInt();

        Serial.println("received command:\t"  + direction + " " + speed);

        car.move(direction, speed);
    }

}