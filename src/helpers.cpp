#include <Arduino.h>


enum Direction{STOP=0, FORWARD=1, BACKWARD=2, LEFT=3, RIGHT=4};

struct SpeedDifference{
    int16_t left_forward_motor = 0;
    int16_t right_forward_motor = 0;
    int16_t left_backward_motor = 0;
    int16_t right_backward_motor = 0;
    void updateSpeedDifference(uint16_t deltaR1, uint16_t deltaR2, uint16_t deltaR3, uint16_t deltaR4){
        this->left_forward_motor -= deltaR1;
        this->right_forward_motor -= deltaR2;
        this->left_backward_motor -= deltaR3;
        this->right_backward_motor -= deltaR4;
        int16_t average = (this->left_forward_motor + this->right_forward_motor +
                this->left_backward_motor + this->right_backward_motor) / 4;
        int16_t deltaAverage = abs(0 - average);
        this->left_forward_motor += deltaAverage;
        this->right_forward_motor += deltaAverage;
        this->left_backward_motor += deltaAverage;
        this->right_backward_motor += deltaAverage;
    }
    void reset(){
        this->left_forward_motor = 0;
        this->right_forward_motor = 0;
        this->left_backward_motor = 0;
        this->right_backward_motor = 0;
    }
    void print(){
        Serial.print(this->left_forward_motor);
        Serial.print('\t');
        Serial.print(this->right_forward_motor);
        Serial.print('\t');
        Serial.print(this->left_backward_motor);
        Serial.print('\t');
        Serial.print(this->right_backward_motor);
        Serial.println();
    }
};