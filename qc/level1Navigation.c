#pragma config(I2C_Usage, I2C1, i2cSensors)
#pragma config(Sensor, dgtl3,  frontUltra,     sensorSONAR_cm)
#pragma config(Sensor, dgtl5,  leftUltra,      sensorSONAR_cm)
#pragma config(Sensor, dgtl7,  rightUltra,     sensorSONAR_cm)
#pragma config(Sensor, dgtl12, redLed,         sensorDigitalOut)
#pragma config(Sensor, I2C_1,  ,               sensorQuadEncoderOnI2CPort,    , AutoAssign )
#pragma config(Sensor, I2C_2,  ,               sensorQuadEncoderOnI2CPort,    , AutoAssign )
#pragma config(Motor,  port1,           rightMotor,    tmotorVex393_HBridge, openLoop, encoderPort, I2C_1)
#pragma config(Motor,  port10,          leftMotor,     tmotorVex393_HBridge, openLoop, reversed, encoderPort, I2C_2)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

void driveStraight(int lowSpeed, int commonSpeed){
	if (nMotorEncoder[rightMotor] > abs(nMotorEncoder[leftMotor])){
		motor[rightMotor] = lowSpeed;
		motor[leftMotor] = commonSpeed;
	}
	if (nMotorEncoder[rightMotor] < abs(nMotorEncoder[leftMotor])){
		motor[rightMotor] = commonSpeed;
		motor[leftMotor] = lowSpeed;
	}
	if (nMotorEncoder[rightMotor] == abs(nMotorEncoder[leftMotor])){
		motor[rightMotor] = commonSpeed;
		motor[leftMotor] = commonSpeed;
	}
}

void completeStop(int delay){
		wait1Msec(delay); // allow the car to move forward until the body face open
		motor[rightMotor] = 0;
		motor[leftMotor]  = 0;
}

void encoderReset(){
		//Clear the encoders associated with the left and right motors
		nMotorEncoder[rightMotor] = 0;
		nMotorEncoder[leftMotor] = 0;
}

void turnRight(int degrees, int speed){
	//fully stop engine
	completeStop(0);
	//Reset encoders
	encoderReset();

  //Determine tickGoal
  int tickGoal = (50 * degrees) / 10;

  //Perform a point turn to the left. We will use lower power values for more accuracy.
  //default 40
  motor[leftMotor] = speed;
  motor[rightMotor] = -1*speed; // turn right

  //Since the wheels may go at slightly different speeds due to manufacturing tolerances, etc.,
  //we need to test both encoders and control both motors separately. This may result in one motor
  //going for longer than another but it will ultimately result in a much more accurate turn.
  while(nMotorEncoder[leftMotor] < tickGoal || nMotorEncoder[rightMotor] > -1*tickGoal) {
    if(nMotorEncoder[leftMotor] > tickGoal) {motor[leftMotor] = 0;}
    if(nMotorEncoder[rightMotor] < -1*tickGoal) {motor[rightMotor] = 0;}
  }
	//turn completed, fully stop engine
	completeStop(0);
}

task main()
{
	int lowSpd = 50;
	int comSpd = 63;
	int delaySec = 350;

	SensorValue[redLed] = 1; //make sure RED-LED is off
	wait1Msec(2000);

	//move along with the wall, stop when it is open on right side
	encoderReset();
	while (SensorValue[rightUltra] <= 23){
		driveStraight(lowSpd, comSpd);
	}

	SensorValue[redLed] = 0; //indicate open turn
	completeStop(delaySec);

	// make a 90 degree right turn
	turnRight(90, 40);
	SensorValue[redLed] = 1; //indicate turn finish

	//wait1Msec(2000);	//wait for 2 secs to make sure ultrasonic sensor is free from noise
	// if @ open area, move
	//while (SensorValue[rightUltra] > 23 && SensorValue[frontUltra] > 23){
	encoderReset();
	SensorValue[redLed] = 0; //indicate dual sensors
	while ((SensorValue[rightUltra] > 23) && (SensorValue[frontUltra] > 23)){
	//while (SensorValue[frontUltra] > 23){
		driveStraight(lowSpd, comSpd);
	}

	//move along with the wall, stop when it is open on right side
	encoderReset();
	while (SensorValue[rightUltra] <= 23){
		driveStraight(lowSpd, comSpd);
	}

	SensorValue[redLed] = 0; //indicate open turn
	completeStop(130);

	// make a 90 degree right turn
	turnRight(90, 40);
	SensorValue[redLed] = 1; //indicate turn finish

	//robot comes into the room
	encoderReset();
	while (SensorValue[frontUltra] > 40){
		driveStraight(lowSpd, comSpd);
	}
	completeStop(0);

	//360 turn to detect detect candle
	wait1Msec(2000);
	turnRight(360, 40);

	//if found, stop & fan on
	//if not found, complete 360
	//backward to exit the room

}
