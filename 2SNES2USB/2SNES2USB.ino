#include "HID-Project.h"

/*
  Dual SNES to USB adapter
  Developed for and tested on Seeduino Xiao SAMD21
  
  Adapted from GadgetReboot's SNES_To_USB


  NES controller pinout:
               _________
             /          |
            /        O  | Ground
           /            |
     +VCC  |  O      O  | Clock
           |            |
     N.C.  |  O      O  | Latch
           |            |
     N.C.  |  O      O  | Data Out (To NES)
           |____________|

  SNES controller pinout:
               ________________
              | 0 0 0 0 | 0 0 0 ) 
               ----------------      
                + C L D   N N G
                V L A A   C C N
                  K T T       D
                    C A
                    H
*/

// bit positions of each controller button in the status register
const int B_BUTTON               = 0;
const int Y_BUTTON               = 1;
const int SELECT_BUTTON          = 2;
const int START_BUTTON           = 3;
const int UP_BUTTON              = 4;
const int DOWN_BUTTON            = 5;
const int LEFT_BUTTON            = 6;
const int RIGHT_BUTTON           = 7;
const int A_BUTTON               = 8;
const int X_BUTTON               = 9;
const int LSHOULDER_BUTTON       = 10;
const int RSHOULDER_BUTTON       = 11;

const int shiftDelay  = 12;  // clock/latch pulse width in microseconds (SNES spec = 12)

// NES/SNES controller pins to Arduino (board specific!)
const int snesClock      = 11;    // controller shared clock pin = CPX pin 11 is A2
const int snesLatch      = 12;    // controller shared latch pin = CPX pin 12 is A3
const int snesData       = 8;     // controller 1 data pin = CPX pin 8 is A6
const int snes2Data      = 7;     // controller 2 data pin = CPX pin 7 is A7

int snesRegister;            // SNES controller 1 button states. 0=pressed 1=released
int snes2Register;           // SNES controller 2 button states. 0=pressed 1=released

void setup(){
  // configure pins
  pinMode(snesData, INPUT);
  pinMode(snes2Data, INPUT);
  pinMode(snesClock, OUTPUT);
  pinMode(snesLatch, OUTPUT);

  digitalWrite(snesClock, LOW);      // SNES control lines idle low
  digitalWrite(snesLatch, LOW);

  //Initialize HID gamepads
  Gamepad1.begin();
  Gamepad2.begin();
  Gamepad1.releaseAll();
  Gamepad2.releaseAll();
}

void loop(){
  readSNES();
  writeSNES1();
  writeSNES2();
}

// Read the 12 SNES controller buttons into snesRegister
// States are asynchronously loaded into the shift register while snesLatch is high
// When snesLatch goes low, the first data bit is shifted to snesData
// Button data is shifted to snesData on each low to high transition of snesClock
void readSNES(){
  digitalWrite(snesLatch, HIGH);       // while the latch pin is high, 
  delayMicroseconds(shiftDelay);       // impose a delay to allow a setup and hold time
  digitalWrite(snesLatch, LOW);  
        
  for (int x = 0; x <= 11; x++) {                    // read in the 12 controller buttons that were latched
    bitWrite(snesRegister, x, digitalRead(snesData)); // store the current button state on the data input into the correct snesRegister position
    bitWrite(snes2Register, x, digitalRead(snes2Data));
    digitalWrite(snesClock, HIGH);                   // generate a clock pulse to bring the next button to the data input
    delayMicroseconds(shiftDelay);
    digitalWrite(snesClock, LOW);                    
  }
}

// Send commands to USB gamepad interface 1
void writeSNES1(){
  if (!bitRead(snesRegister, B_BUTTON)){
    Gamepad1.press(1);
  }
  else{
    Gamepad1.release(1);
  }
  if (!bitRead(snesRegister, Y_BUTTON)){
    Gamepad1.press(2);
  }
  else{
    Gamepad1.release(2);
  }
  if (!bitRead(snesRegister, SELECT_BUTTON)){
    Gamepad1.press(3);
  }
  else{
    Gamepad1.release(3);
  }
  if (!bitRead(snesRegister, START_BUTTON)){
    Gamepad1.press(4);
  }
  else{
    Gamepad1.release(4);
  }
  if (!bitRead(snesRegister, UP_BUTTON)){
    Gamepad1.yAxis(-32768);
  }
  else if (!bitRead(snesRegister, DOWN_BUTTON)){
    Gamepad1.yAxis(32767);
  }
  else{
    Gamepad1.yAxis(0);
  }
  if (!bitRead(snesRegister, LEFT_BUTTON)){
    Gamepad1.xAxis(-32768);
  }
  else if (!bitRead(snesRegister, RIGHT_BUTTON)){
    Gamepad1.xAxis(32767);
  }
  else{
    Gamepad1.xAxis(0);
  }
  if (!bitRead(snesRegister, A_BUTTON)){
    Gamepad1.press(5);
  }
  else{
    Gamepad1.release(5);
  }
  if (!bitRead(snesRegister, X_BUTTON)){
    Gamepad1.press(6);
  }
  else{
    Gamepad1.release(6);
  }
  if (!bitRead(snesRegister, LSHOULDER_BUTTON)){
    Gamepad1.press(7);
  }
  else{
    Gamepad1.release(7);
  }
  if (!bitRead(snesRegister, RSHOULDER_BUTTON)){
    Gamepad1.press(8);
  }
  else{
    Gamepad1.release(8);
  }

  Gamepad1.write();
}

// Send commands to USB gamepad interface 2
void writeSNES2(){
  if (!bitRead(snes2Register, B_BUTTON)){
    Gamepad2.press(1);
  }
  else{
    Gamepad2.release(1);
  }
  if (!bitRead(snes2Register, Y_BUTTON)){
    Gamepad2.press(2);
  }
  else{
    Gamepad2.release(2);
  }
  if (!bitRead(snes2Register, SELECT_BUTTON)){
    Gamepad2.press(3);
  }
  else{
    Gamepad2.release(3);
  }
  if (!bitRead(snes2Register, START_BUTTON)){
    Gamepad2.press(4);
  }
  else{
    Gamepad2.release(4);
  }
  if (!bitRead(snes2Register, UP_BUTTON)){
    Gamepad2.yAxis(-32768);
  }
  else if (!bitRead(snes2Register, DOWN_BUTTON)){
    Gamepad2.yAxis(32767);
  }
  else{
    Gamepad2.yAxis(0);
  }
  if (!bitRead(snes2Register, LEFT_BUTTON)){
    Gamepad2.xAxis(-32768);
  }
  else if (!bitRead(snes2Register, RIGHT_BUTTON)){
    Gamepad2.xAxis(32767);
  }
  else{
    Gamepad2.xAxis(0);
  }
  if (!bitRead(snes2Register, A_BUTTON)){
    Gamepad2.press(5);
  }
  else{
    Gamepad2.release(5);
  }
  if (!bitRead(snes2Register, X_BUTTON)){
    Gamepad2.press(6);
  }
  else{
    Gamepad2.release(6);
  }
  if (!bitRead(snes2Register, LSHOULDER_BUTTON)){
    Gamepad2.press(7);
  }
  else{
    Gamepad2.release(7);
  }
  if (!bitRead(snes2Register, RSHOULDER_BUTTON)){
    Gamepad2.press(8);
  }
  else{
    Gamepad2.release(8);
  }

  Gamepad2.write();
}
//steal this sketch
