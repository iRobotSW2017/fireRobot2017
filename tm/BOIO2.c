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

void goStraight (int lowSpeed, int commonSpeed){
//this fuction walks straight and has a consistent speed
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

void completeStop (int delaytime){
	motor[rightMotor] = 0;
	motor[leftMotor] = 0;
	wait1Msec(delaytime); //this stops rabot for a delay of time
}

/*added by Qian for more precise turn */
void runStop (int delaytime){
	wait1Msec(delaytime); //this stops rabot for a delay of time
	motor[rightMotor] = 0;
	motor[leftMotor] = 0;
}

void encoderReset(){
		//Clear the encoders associated with the left and right motors
		resetMotorEncoder(leftMotor);
		resetMotorEncoder(rightMotor);
}

void turnRight(int degrees, int speed){
	//fully stop engine
	runStop(0);
	//Reset encoders
	encoderReset();

  //Determine tickGoal
  int tickGoal = (49 * degrees) / 10;

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
	runStop(0);
}
/*added by Qian for more precise turn */

task main()
{

		int comSpd = 90;
		int lowSpd = 75;
		int lowAdjSpd = 25; //25
		int comAdjSpd = 31;
		//int comAdjSpd = 31;
		//int delaySec = 450;
		int frontSpace = 30; //(46-30)/2
		int rightSpace = 30;

		int nBatteryLevel = nImmediateBatteryLevel; //read battery level
		SensorValue[redLed] = 1; //make sure RED-LED is off
		//delay to start
		wait1Msec(2000);

		//Right motor is better to use than fwd motor because of different interferences
		encoderReset();
		while(SensorValue[rightUltra] < rightSpace){
			goStraight(lowSpd, comSpd);	//speed is debatable
		}
		SensorValue[redLed] = 0; //make sure RED-LED is off
		//completeStop(1000); // have robot stop @ the middle of hallway

		//allow the robot to move a little more, position @ the center of intersection
		encoderReset();
		goStraight(lowAdjSpd, comAdjSpd);
		runStop(500);
		SensorValue[redLed] = 1; //make sure RED-LED is off
		completeStop(1500); // to make sure a full stop

		//make 90 turn, going to room#1 direction
		turnRight(90, 40);

		//allow te robot to move forward
		encoderReset();
		while(SensorValue[frontUltra] > frontSpace){
			goStraight(lowSpd, comSpd);
		}
		completeStop(0);

		//move close to the wall, and stop, so the robot can stop on the middle of entry
		encoderReset();
		goStraight(lowAdjSpd, comAdjSpd);
		runStop(400);	//???
		completeStop(1500); // to make sure a full stop

		turnRight(93, 40);//turn into the room 90@Rright turn
		completeStop(1000);

		//drive into the room
		encoderReset();
		while(SensorValue[frontUltra] > (frontSpace+5)){
			goStraight((lowAdjSpd+10), (comAdjSpd+10));
		}
		runStop(500);	//stop close to 30cm

		//180 degree turn
		turnRight(180, 40);//turn into the room 90@Rright turn
		completeStop(1000);

		//leave the room
		while(SensorValue[frontUltra] > 20){
			goStraight((lowAdjSpd+10), (comAdjSpd+10));
		}
		runStop(500);	//stop close to wall
		completeStop(1000);

/*		//make 90 left turn
		pointTurn(-comSpd, comSpd, 860);
		completeStop(1000);

		goStraight(comSpd, comSpd);// move to center of the hallways
		wait1Msec(1500);
*/

}
