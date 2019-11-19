#include "FTLib.hpp"
#include <stdexcept>


//TXT
TXT::TXT(){
    if(StartTxtDownloadProg() == KELIB_ERROR_NONE){
        pTArea = GetKeLibTransferAreaMainAddress();
    }
    else{
        throw std::runtime_error("KELIB_ERROR");
    }
}

TXT::~TXT(){
    StopTxtDownloadProg();
}

TXT::DigitalInput(uint8_t pin){
    if(pin > 15){
        throw std::invalid_argument("pin must be between 0 and 7 for master and 8 and 15 for extension");
    }
    return DigitalInput{(pTArea+pin/8),pin%8};
}

TXT::Output(uint8_t pin){
    if(pin > 15){
        throw std::invalid_argument("pin must be between 0 and 7 for master and 8 and 15 for extension");
    }
    return Output{(pTArea+pin/8),pin%8};
}


//Output
Output::Output(FISH_X1_TRANSFER* pTArea,uint8_t pin): pin(pin), pTArea(pTArea){}

void Output::on(){
    pTArea->ftX1out.duty[pin] = 512;
}

void Output::off(){
    pTArea->ftX1out.duty[pin] = 0;
}

void Output::setLevel(uint16_t level){
    pTArea->ftX1out.duty[pin] = level;
}

uint8_t Output::getPin(){
    return pin;
}

//DigitalInput
DigitalInput::DigitalInput(FISH_X1_TRANSFER* pTArea,uint8_t pin): pin(pin), pTArea(pTArea){
    pTArea->ftX1config.uni[pin].mode = MODE_R; 	    //  resistor
    pTArea->ftX1config.uni[pin].digital = 1;        //  digital Input
    pTArea->ftX1state.config_id ++; 
}

bool DigitalInput::value(){
    return pTArea->ftX1in.uni[pin];
}

uint8_t DigitalInput::getPin(){
    return pin;
}

AnalogInput::AnalogInput(FISH_X1_TRANSFER* pTArea,uint8_t pin): pin(pin), pTArea(pTArea){
    pTArea->ftX1config.uni[pin].mode = MODE_R; 	    //  resistor
    pTArea->ftX1config.uni[pin].digital = 0;        //  analog Input
    pTArea->ftX1state.config_id ++; 
}

uint16_t AnalogInput::value(){
    return pTArea->ftX1in.uni[pin];
}

uint8_t AnalogInput::getPin(){
    return pin;
}

//Colorsensor
ColorSensor::ColorSensor(FISH_X1_TRANSFER* pTArea,uint8_t pin): pin(pin), pTArea(pTArea){
    pTArea->ftX1config.uni[pin].mode = MODE_U; 	//	Spannung
    pTArea->ftX1config.uni[pin].digital = 0;    //  analog Input
    pTArea->ftX1state.config_id ++; 
}

uint16_t ColorSensor::value(){
    return pTArea->ftX1in.uni[pin];
}

Color ColorSensor::color(){
    if(value() < 500){
        return Color::WHITE;
    }
    else if(value() < 1300){
        return Color::RED;
    }
    else {
        return Color::BLUE;
    }
}

uint8_t ColorSensor::getPin(){
    return pin;
}

//Colorsensor
Voltage::Voltage(FISH_X1_TRANSFER* pTArea,uint8_t pin): pin(pin), pTArea(pTArea){
    pTArea->ftX1config.uni[pin].mode = MODE_U; 	//	Spannung
    pTArea->ftX1config.uni[pin].digital = 0;    //  analog Input
    pTArea->ftX1state.config_id ++; 
}

uint16_t Voltage::value(){
    return pTArea->ftX1in.uni[pin];
}

uint8_t Voltage::getPin(){
    return pin;
}

//Colorsensor
Ultrasonic::Ultrasonic(FISH_X1_TRANSFER* pTArea,uint8_t pin): pin(pin), pTArea(pTArea){
    pTArea->ftX1config.uni[pin].mode = MODE_ULTRASONIC; 
    pTArea->ftX1config.uni[pin].digital = 0;  
    pTArea->ftX1state.config_id ++; 
}

uint16_t Ultrasonic::value(){
    return pTArea->ftX1in.uni[pin];
}

uint8_t Ultrasonic::getPin(){
    return pin;
}

//Spurensensor
TrackSensor::TrackSensor(FISH_X1_TRANSFER* pTArea,uint8_t left, uint8_t right): left(left), right(right), pTArea(pTArea){
    pTArea->ftX1config.uni[left].mode = MODE_U; 
    pTArea->ftX1config.uni[left].digital = 1;  
    pTArea->ftX1config.uni[right].mode = MODE_U; 
    pTArea->ftX1config.uni[right].digital = 1;  
    pTArea->ftX1state.config_id ++; 
}

uint16_t TrackSensor::valueLeft(){
    return pTArea->ftX1in.uni[left];
}

uint16_t TrackSensor::valueRight(){
    return pTArea->ftX1in.uni[right];
}

uint8_t TrackSensor::getPinLeft(){
    return left;
}

uint8_t TrackSensor::getPinRight(){
    return left;
}


//Motor
Motor::Motor(FISH_X1_TRANSFER* pTArea,uint8_t pin): pin(pin), pTArea(pTArea){


void Motor::left(uint16_t level){
    pTArea->ftX1out.duty[pin*2] = level;
    pTArea->ftX1out.duty[pin*2+1] = 0;
}

void Motor::right(uint16_t level){
    pTArea->ftX1out.duty[pin*2] = 0;
    pTArea->ftX1out.duty[pin*2+1] = level;
}

void Motor::stop(){
    pTArea->ftX1out.duty[pin*2] = 0;
    pTArea->ftX1out.duty[pin*2+1] = 0;
}

uint8_t Motor::getPin(){
    return pin;
}

//EncoderMotor
EncoderMotor::EncoderMotor(uint8_t pin, uint8_t c_pin) : pin(pin), c_pin(c_pin) {}

void EncoderMotor::distanceLeft(uint16_t steps){
    pTArea->ftX1out.distance[pin] = steps;  // Distance to drive Motor 1 [0]
	pTArea->ftX1out.motor_ex_cmd_id[pin]++; // Set new Distance Value for Motor 1 [0]
    left();
}

