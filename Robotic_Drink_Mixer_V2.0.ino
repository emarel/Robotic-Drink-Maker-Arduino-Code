#include <AccelStepper.h>
#include <Wire.h>
#include <Adafruit_MotorShield.h>
#include "utility/Adafruit_PWMServoDriver.h"

//FV = Function Variable
//Global Variables 
unsigned long dispense_delay = 250;  	//MUST STAY unsigned long, Time to dispense one shot
int position_size = 278;            	//MUST STAY int, Number of steps to move one position
int dispense_size = 3000;           	//MUST STAY int, number of steps to move to dispense
int dispense_size_2 = 2800;
int current_position = 1;           	//Strongly Prefer int
char INBYTE;                        	//For reading bluetooth input
double ingredients[26];             	//Ingredients Array

Adafruit_MotorShield AFMS = Adafruit_MotorShield();        // Create the motor shield object with the default I2C address
Adafruit_StepperMotor *myMotor1 = AFMS.getStepper(200, 1); // Connect a stepper motor with 200 steps per revolution (1.8 degree) to motor port #2 (M3 and M4)
Adafruit_StepperMotor *myMotor2 = AFMS.getStepper(200, 2); // Connect a stepper motor with 200 steps per revolution (1.8 degree) to motor port #2 (M3 and M4)

void setup() {//=========================================================================== SETUP
  
  Serial.begin(9600);     	//Initialize Serial
  
  AFMS.begin();       		// create with the default frequency 1.6KHz
  //AFMS.begin(1000);   	// OR with a different frequency, say 1KHz
  
  myMotor1->setSpeed(3000); // 10 rpm
  myMotor2->setSpeed(3000); // 10 rpm
  
}

//Move tray to appropriate position for dispense
void move(int position) {//=========================================================================== MOVE
    
  int steps = 0;                          //FV for calculating steps
  steps = position - current_position;    //Calculate number of steps and direction
  //Serial.println("I am moving steps:");
  //Serial.println(steps);
  
  //Move right(Forward) if positive, left(Backward) if negative
  if(steps > 0){
    myMotor2->step(steps*position_size, FORWARD, DOUBLE);
	//Serial.println("I would move right to position");
	//Serial.println(position);
  }
  else if(steps < 0){
    steps = -1 * steps;
    myMotor2->step(steps*position_size, BACKWARD, DOUBLE);
	//Serial.println("I would move left to position");
	//Serial.println(position);
  }
  
  current_position = position;            //Update current position
}

void dispense(double quantity) {//=========================================================================== DISPENSE
  
  //Dispense whole shots
  for(int i = 1; i <= quantity; i++){
    myMotor1->step(dispense_size,FORWARD, DOUBLE);    //Start Dispense
    delay(dispense_delay);                            //Hold Dispense
    myMotor1->step(dispense_size,BACKWARD, DOUBLE);   //End Dispense
	//Serial.println("I would have dispensed");
	//Serial.println(quantity);
  }
  if(quantity == 0.50 || quantity == 1.50 || quantity == 2.50 || quantity == 3.50 || quantity == 4.50 || quantity == 5.50){
	myMotor1->step(dispense_size,FORWARD, DOUBLE);    //Start Dispense
    delay(dispense_delay);                          	//Hold Dispense
    myMotor1->step(dispense_size,BACKWARD, DOUBLE);   //End Dispense
	//Serial.println("I would have dispensed");
	//Serial.println(quantity);
  }
}

//Iterate through ingredients array and call move and dispense until done
void decode() {//=========================================================================== DECODE

  int position = 0;                     //FV to hold position and pass to move
  double quantity = 0;                  //FV to hold quantity and pass to move
  for(int i = 0; i < 26; i=i+2){        //Iterate through array
	position = ingredients[i];			//Read position data
    quantity = ingredients[i+1];        //Read quantity data
	if(quantity == 50){
		move(position);
	}
	else if(quantity != 0){
		move(position);                     //Call move using position
		dispense(quantity);                 //Call dispense with quantity
	}
  }
}

void translate() {//=========================================================================== TRANSLATE - BLUETOOTH
   
  for(int i = 0; i < 26; i++){ 
    //Serial.println("I am in iteration:");
	//Serial.println(i);
	INBYTE = Serial.read();        // read next available byte
    Serial.println(INBYTE);        // echo output for testing
    
    if(INBYTE == 'A'){             //Translate Positions
       ingredients[i] = 1;
    }
    else if(INBYTE == 'B'){
       ingredients[i] = 2;
    }
    else if(INBYTE == 'C'){
       ingredients[i] = 3;
    }
    else if(INBYTE == 'D'){
       ingredients[i] = 4;
    }
    else if(INBYTE == 'E'){
       ingredients[i] = 5;
    }
    else if(INBYTE == 'F'){
       ingredients[i] = 6;
    }
    else if(INBYTE == 'G'){
       ingredients[i] = 7;
    }
    else if(INBYTE == 'H'){
       ingredients[i] = 8;
    }
    else if(INBYTE == 'I'){
       ingredients[i] = 9;
    }
    else if(INBYTE == 'J'){
       ingredients[i] = 10;
    }
    else if(INBYTE == 'K'){
       ingredients[i] = 11;
    }
    else if(INBYTE == 'L'){
       ingredients[i] = 12;
    }
    else if(INBYTE == 'Y'){
      ingredients[i] = 1;
    }
    
    else if(INBYTE == 'M'){    		//Translate Quantities
       ingredients[i] = 0;
    }
	else if(INBYTE == 'N'){
       ingredients[i] = 0.5;
    }
	else if(INBYTE == 'O'){
       ingredients[i] = 1;
    }
	else if(INBYTE == 'P'){
       ingredients[i] = 1.5;
    }
    else if(INBYTE == 'Q'){
       ingredients[i] = 2;
    }
	else if(INBYTE == 'R'){
       ingredients[i] = 2.5;
    }
    else if(INBYTE == 'S'){
       ingredients[i] = 3;
    }
	else if(INBYTE == 'T'){
       ingredients[i] = 3.5;
    }
    else if(INBYTE == 'U'){
       ingredients[i] = 4;
    }
	else if(INBYTE == 'V'){
       ingredients[i] = 4.5;
    }
    else if(INBYTE == 'W'){
       ingredients[i] = 5;
    }
	else if(INBYTE == 'X'){
       ingredients[i] = 5.5;
    }
	else if(INBYTE == 'Z'){
		ingredients[i] = 50;
	}
    
    else{
      Serial.println("You got some errors here Jimbo");
    }
    
    delay(50);
  }

}

void test_debug(){
  INBYTE = Serial.read();        // read next available byte
  Serial.println(INBYTE);        // echo output for testing
  if(INBYTE == 'c'){
	myMotor1->step(3000,FORWARD, DOUBLE);    //Start Dispense
    delay(250);                            //Hold Dispense
    myMotor1->step(3000,BACKWARD, DOUBLE);   //End Dispense
  }
  if(INBYTE == 'u'){
	myMotor1->step(dispense_size,FORWARD, DOUBLE);    //Start Dispense
  }
  if(INBYTE == 'd'){
	myMotor1->step(dispense_size,BACKWARD, DOUBLE);   //End Dispense
  }
  if(INBYTE == 'l'){
	  myMotor2->step(position_size, BACKWARD, DOUBLE);
  }
  if(INBYTE == 'r'){
	  myMotor2->step(position_size, FORWARD, DOUBLE);
  }
}
	
void loop() {//=========================================================================== LOOP
  while (!Serial.available());   // stay here so long as COM port is empty
  //test_debug();
  translate();
  for(int i = 0; i < 26; i++){
	  Serial.println(ingredients[i]);
  }
  decode();
}
