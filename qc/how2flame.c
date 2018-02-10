#pragma config(I2C_Usage, I2C1, i2cSensors)
#pragma config(Sensor, dgtl3,  frontUltra,     sensorSONAR_cm)
#pragma config(Sensor, dgtl5,  leftUltra,      sensorSONAR_cm)
#pragma config(Sensor, dgtl7,  rightUltra,     sensorSONAR_cm)
#pragma config(Sensor, dgtl9,  Bumpey,         sensorTouch)
#pragma config(Sensor, dgtl10, fan,            sensorDigitalOut)
#pragma config(Sensor, dgtl12, redLed,         sensorDigitalOut)
#pragma config(Sensor, I2C_1,  ,               sensorQuadEncoderOnI2CPort,    , AutoAssign )
#pragma config(Sensor, I2C_2,  ,               sensorQuadEncoderOnI2CPort,    , AutoAssign )
#pragma config(Motor,  port1,           rightMotor,    tmotorVex393_HBridge, openLoop, encoderPort, I2C_1)
#pragma config(Motor,  port10,          leftMotor,     tmotorVex393_HBridge, openLoop, reversed, encoderPort, I2C_2)
#pragma config(Sensor, in8,    IR_sensor,      sensorAnalog)

//global variables
		int comSpd = 90;
		int lowSpd = 70;
		int rightTicks = 60;
		int leftTicks = 57;
		//int comAdjSpd = 31;
		//int delaySec = 450;
		//int frontSpace = 12; //(46-30)/2
		int rightSpace = 23;
		int flameDetected = 200;
		int flameOff = 66;
		bool isFlameDetected = false;
		bool isFlameOff = false;

void walkStraight(int lowSpeed, int CommonSpeed){
//This function Walks straight and has a consistent speed
	if ((nMotorEncoder[rightMotor]) > (abs(nMotorEncoder[leftMotor]))){
		motor[rightMotor] = lowSpeed;
		motor[leftMotor] = CommonSpeed;
	}
	if (nMotorEncoder[rightMotor] < abs(nMotorEncoder[leftMotor])){
		motor[rightMotor] = CommonSpeed;
		motor[leftMotor] = lowSpeed;
	}
	if (nMotorEncoder[rightMotor] == abs(nMotorEncoder[leftMotor])){
		motor[rightMotor] = CommonSpeed;
		motor[leftMotor] = CommonSpeed;
	}
}

void completeStop (int delaytime){
	motor[rightMotor] = 0;
	motor[leftMotor] = 0;
	wait1Msec(delaytime); //this stops rabot for a delay of time
}

void resetEncoders(){
	nMotorEncoder[rightMotor]=0;
	nMotorEncoder[leftMotor]=0;
}

/*
 * move closer, put off flame, LED off, backward
 */
void putOffFlame(){
	int _moveClose = 0;
	int _slowCloser = lowSpd;
	//only do when flame is detected
	if(isFlameDetected){
		//move close if front > 30
		resetEncoders();
		while(SensorValue[frontUltra] > 30){
			walkStraight(_slowCloser, _slowCloser);
		}
		_moveClose = abs(nMotorEncoder[rightMotor]); //return encoder value for reverse purpose
		completeStop(500);
		//start fan until flame is off, & turn of LED
		while(true){
				writeDebugStreamLine("IR %d", SensorValue[IR_sensor]);
				if(SensorValue[IR_sensor] > flameOff){
					SensorValue[fan] = 1;	//start fan
				}else{
					SensorValue[fan] = 0;	//off fan
					wait1Msec(1000);
					SensorValue[redLed] = 1;	//off LED
					break;
				}
		}
		//back to the start point
		resetEncoders();
		while(abs(nMotorEncoder[rightMotor]) <= _moveClose){
				motor[rightMotor] = -1 * _slowCloser;
				motor[leftMotor] = -1 * _slowCloser;
		}
		completeStop(500);
	}

}

/*
 * scan clockwise, stop when flame is detected
 */
int turnRight(int degrees, int speed, int offset, bool isDetecting){
	resetEncoders();
	//you must reset the encoders

	int tickGoal = ((rightTicks * degrees)/10 - offset);	//increased the tick goal by 1 today by mattyboio == 74@matt

	motor[leftMotor]=speed;
	motor[rightMotor]=-1*speed;

	while(nMotorEncoder[leftMotor]<tickGoal||nMotorEncoder[rightMotor]>-1*tickGoal){
		if(isDetecting){
			// flame is detected, stop motor, turn on LED, return ticks-run so far
			if(SensorValue[IR_sensor] > flameDetected){
				motor[leftMotor]=0;
				motor[rightMotor]=0;
				SensorValue[redLed] = 0;
				isFlameDetected = true;
				return abs(nMotorEncoder[rightMotor]);
			}
		}
		if(nMotorEncoder[leftMotor]>tickGoal){
			motor[leftMotor]=0;
		}
		if(nMotorEncoder[rightMotor]<-1*tickGoal){
			motor[rightMotor]=0;
		}
	}
	return -1;	//meaning turn is completed
}

void turnLeft(int degrees, int speed){
	resetEncoders();
	//you must reset the encoders

	int tickGoal = (leftTicks * degrees)/10;	//72@Matt

	motor[leftMotor]=-1*speed;
	motor[rightMotor]=speed;

	while(nMotorEncoder[leftMotor]>-1*tickGoal||nMotorEncoder[rightMotor]<tickGoal){
		if(nMotorEncoder[leftMotor]<-1*tickGoal){
			motor[leftMotor]=0;
		}
		if(nMotorEncoder[rightMotor]>tickGoal){
			motor[rightMotor]=0;
			//this is quality control
		}
	}
}


task main()
{
		SensorValue[redLed] = 1; // turn off LED by default

		//wait to have a full stop
		while(SensorValue[Bumpey]==0){
			motor[leftMotor]=0;
			motor[rightMotor]=0;
		}
		wait1Msec(500);

		//start room#1 -----
		//Right motor is better to use than fwd motor because of different interferences
		resetEncoders();
		while(SensorValue[rightUltra]<rightSpace){
			walkStraight(lowSpd, comSpd);	//speed is debateable
		}
		completeStop(1000); // have robot stop @ the middle of hallway

		//allow the robot to move a little more, position @ the center of intersection
		resetEncoders();
		walkStraight(lowSpd, comSpd);
		wait1Msec(300);
		completeStop(500);

		//make 90 turn, going to room#1 direction
		turnRight(90, 60, 0, false);//turn to the room so we can almost enter.
		completeStop(1000);

		//allow the robot to move forward
		resetEncoders();
		walkStraight(lowSpd, comSpd);
		wait1Msec(1000);

		resetEncoders();
		while(SensorValue[rightUltra]<rightSpace){
			walkStraight(lowSpd, comSpd);
		}
		completeStop(0);
		resetEncoders();
		while(SensorValue[frontUltra]>15){
			walkStraight(lowSpd, comSpd);
		}
		completeStop(1500);

		turnRight(90, 60, 0, false);	//turn to the room so we can almost enter.
		completeStop(1000);

		//push robot into room#1 to reduce front ultrasonic false reading
		resetEncoders();
		walkStraight(lowSpd, comSpd);
		wait1Msec(1000);
		//drive into the room
		resetEncoders();
		while(SensorValue[frontUltra]>30){
			walkStraight(lowSpd, comSpd);  //Walk straight
		}
		completeStop(1000);	//stop close to 30cm

		// can we do it?!
		int _ticks1 = turnRight(180, 60, 0, true);
		if(isFlameDetected){
			putOffFlame(); // put off flame
			// how to finish the rest of turn
			turnRight(180, 60, _ticks1, false);
		}
		completeStop(1000);

		//leave the room
		//use close ultrasonic for navigation to reduce noise
		resetEncoders();
		while(SensorValue[leftUltra] < 80){
				walkStraight(lowSpd, comSpd);
		}
		completeStop(0);
		// @ hallway
		resetEncoders();
		while(SensorValue[frontUltra]>20){
				walkStraight(lowSpd, comSpd);
		}
		completeStop(1000);//exit the room


		//make 90 left turn
		turnLeft(90, 60);//turn to the room so we can almost enter.
		completeStop(1000);

		resetEncoders();
		while(SensorValue[rightUltra]<rightSpace){
			walkStraight(lowSpd, comSpd);
		}
		completeStop(0);

		//allow the robot to move a little more, position @ the center of intersection
		resetEncoders();
		walkStraight(lowSpd, comSpd);
		wait1Msec(250);
		completeStop(500);

		//end as finishing room#1
		//if candle off should return to start point
		if(isFlameOff){
			//make 90 left turn
			turnLeft(90, 60);//turn to the room so we can almost enter.
			completeStop(1000);
		}
}
