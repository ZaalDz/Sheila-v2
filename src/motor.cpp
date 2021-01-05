#include <Arduino.h>


class Motor{
    const uint8_t ENABLE;
    const uint8_t INPUT1;
    const uint8_t INPUT2;
    const uint8_t WHEEL_ROTATION_COUNTER_PIN;
    uint16_t wheelRotationCounter = 0;
    uint8_t lastPulse = 0;
    uint8_t speed = 0;
public:
    Motor(const uint8_t enable, const uint8_t input1, const uint8_t input2, const uint8_t wheelRotationPin):
            ENABLE(enable), INPUT1(input1), INPUT2(input2), WHEEL_ROTATION_COUNTER_PIN(wheelRotationPin){}
    void forward(uint8_t speed){
        speed = max(0, min(speed, 255));
        this->speed = speed;
        digitalWrite(this->INPUT1, HIGH);
        digitalWrite(this->INPUT2, LOW);
        analogWrite(this->ENABLE, this->speed);
    }
    void backward(uint8_t 
    speed){
        speed = max(0, min(speed, 255));
        this->speed = speed;
        digitalWrite(this->INPUT1, LOW);
        digitalWrite(this->INPUT2, HIGH);
        analogWrite(this->ENABLE, this->speed);
    }
    void stop(){
        this->speed = 0;
        digitalWrite(this->INPUT1, LOW);
        digitalWrite(this->INPUT2, LOW);
        analogWrite(this->ENABLE, this->speed);
        this->wheelRotationCounter = 0;
    }
    void updateWheelRotationCounter(){
        uint8_t currentPulse = digitalRead(this->WHEEL_ROTATION_COUNTER_PIN);
        if (currentPulse != this->lastPulse){
            this->wheelRotationCounter++;
        }
        this->lastPulse = currentPulse;
    }
    void resetWheelRotationCounter(){
        this->wheelRotationCounter = 0;
    }
    uint16_t getWheelRotationCounter(){
        return this->wheelRotationCounter;
    }
};