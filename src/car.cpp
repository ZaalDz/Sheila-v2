#include <Arduino.h>
#include <motor.cpp>
#include <helpers.cpp>


class Car{
private:
    Motor* left_forward_motor = new Motor(9, 30, 31, 7);
    Motor* right_forward_motor = new Motor(11, 37, 36, 6);
    Motor* left_backward_motor = new Motor(10, 33, 32, 5);
    Motor* right_backward_motor = new Motor(12, 38, 39, 4);
    const uint8_t MOTOR_FIX_UP_ROTATE_THRESHOLD = 30;
    SpeedDifference* speedDifference = new SpeedDifference();
    uint8_t speed = 0;
    Direction carDirection = Direction::STOP;
    void fixUpMotorSpeed(uint16_t r1, uint16_t r2, uint16_t r3, uint16_t r4){
        uint16_t min_r = min(min(r1, r2), min(r3, r4));
        uint16_t deltaR1 = r1 - min_r;
        uint16_t deltaR2 = r2 - min_r;
        uint16_t deltaR3 = r3 - min_r;
        uint16_t deltaR4 = r4 - min_r;
        speedDifference->updateSpeedDifference(deltaR1, deltaR2, deltaR3, deltaR4);
        speedDifference->print();
        left_forward_motor->resetWheelRotationCounter();
        right_forward_motor->resetWheelRotationCounter();
        left_backward_motor->resetWheelRotationCounter();
        right_backward_motor->resetWheelRotationCounter();
    }
    void updateWheelRotationCounters(){
        left_forward_motor->updateWheelRotationCounter();
        right_forward_motor->updateWheelRotationCounter();
        left_backward_motor->updateWheelRotationCounter();
        right_backward_motor->updateWheelRotationCounter();
        uint16_t r1 = left_forward_motor->getWheelRotationCounter();
        uint16_t r2 = right_forward_motor->getWheelRotationCounter();
        uint16_t r3 = left_backward_motor->getWheelRotationCounter();
        uint16_t r4 = right_backward_motor->getWheelRotationCounter();
        uint16_t max_r = max(max(r1, r2), max(r3, r4));
        if (max_r >= MOTOR_FIX_UP_ROTATE_THRESHOLD){
            this->fixUpMotorSpeed(r1, r2, r3, r4);
        }
    }
public:
    void forward(const uint8_t speed){
        this->speed = speed;
        if (this->carDirection != Direction::FORWARD){
            this->stop();
        } else {
            this->updateWheelRotationCounters();
        }
        left_forward_motor->forward(speed + speedDifference->left_forward_motor);
        right_forward_motor->forward(speed + speedDifference->right_forward_motor);
        left_backward_motor->forward(speed + speedDifference->left_backward_motor);
        right_backward_motor->forward(speed + speedDifference->right_backward_motor);
        this->carDirection = Direction::FORWARD;
    }
    void backward(const uint8_t speed){
        this->speed = speed;
        if (this->carDirection != Direction::BACKWARD){
            this->stop();
        } else {
            this->updateWheelRotationCounters();
        }
        left_forward_motor->backward(speed + speedDifference->left_forward_motor);
        right_forward_motor->backward(speed + speedDifference->right_forward_motor);
        left_backward_motor->backward(speed + speedDifference->left_backward_motor);
        right_backward_motor->backward(speed + speedDifference->right_backward_motor);
        this->carDirection = Direction::BACKWARD;
    }
    void left(const uint8_t speed){
        this->speed = speed;
        if (this->carDirection != Direction::LEFT){
            this->stop();
        } else {
            this->updateWheelRotationCounters();
        }
        left_forward_motor->backward(speed + speedDifference->left_forward_motor);
        right_forward_motor->forward(speed + speedDifference->right_forward_motor);
        left_backward_motor->backward(speed + speedDifference->left_backward_motor);
        right_backward_motor->forward(speed + speedDifference->right_backward_motor);
        this->carDirection = Direction::LEFT;
    }
    void right(const uint8_t speed){
        this->speed = speed;
        if (this->carDirection != Direction::RIGHT){
            this->stop();
        } else {
            this->updateWheelRotationCounters();
        }
        left_forward_motor->forward(speed + speedDifference->left_forward_motor);
        right_forward_motor->backward(speed + speedDifference->right_forward_motor);
        left_backward_motor->forward(speed + speedDifference->left_backward_motor);
        right_backward_motor->backward(speed + speedDifference->right_backward_motor);
        this->carDirection = Direction::RIGHT;
    }
    void stop(){
        left_forward_motor->stop();
        right_forward_motor->stop();
        left_backward_motor->stop();
        right_backward_motor->stop();
        this->carDirection = Direction::STOP;
        this->speedDifference->reset();
        this->speed = 0;
    }
    ~Car(){
        delete left_forward_motor;
        delete left_backward_motor;
        delete right_backward_motor;
        delete right_forward_motor;
        delete speedDifference;
    }
};