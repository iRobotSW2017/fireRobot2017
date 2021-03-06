#pragma config(I2C_Usage, I2C1, i2cSensors)
#pragma config(Sensor, in8,    IR_sensor,      sensorAnalog)
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
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//
//Matt and dyliboyo
//global variables
		//int comSpd = 90;
		//int lowSpd = 70;
		int comSpd = 50;
		int lowSpd = 40;
		int rightTicks = 45; //60
		int leftTicks = 43;	//57
		//int comAdjSpd = 31;
		//int delaySec = 450;
		//int frontSpace = 12; //(46-30)/2
		int rightSpace = 23;
		int flameDetected = 120;	//250
		int flameTargetAdj = 13; //8
		int flameOff = 100;
		bool isFlameDetected = false;
		bool isFlameOff = false;

void _walkStraight(int lowSpeed, int CommonSpeed){
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

void walkStraight(int lowSpeed, int CommonSpeed){
//This function Walks straight and has a consistent speed
	int _right = SensorValue[rightUltra];
	int _left = SensorValue[leftUltra];

	//try to get robot run in the center line of hall way

	if(_left < 10){
			motor[leftMotor] = CommonSpeed + 10;
			motor[rightMotor] = lowSpeed;
	//		wait1Msec(500);
	}else if(_right < 10){
			motor[rightMotor] = CommonSpeed + 10;
			motor[leftMotor] = lowSpeed;
	//		wait1Msec(500);
	}else if((_right < rightSpace) && (_left < rightSpace) && (abs(_left - _right) >= 3)){
		if(_right < _left){
			motor[rightMotor] = CommonSpeed + 10;
			motor[leftMotor] = lowSpeed;
		}else if(_right > _left){
			motor[leftMotor] = CommonSpeed + 10;
			motor[rightMotor] = lowSpeed;
		}else{
			_walkStraight(lowSpeed, CommonSpeed);
		}
	}else{
		_walkStraight(lowSpeed, CommonSpeed);
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
		while(SensorValue[frontUltra] > 20){
			walkStraight(_slowCloser, _slowCloser);
		}
		_moveClose = abs(nMotorEncoder[rightMotor]); //return encoder value for reverse purpose
		completeStop(500);
		//start fan until flame is off, & turn of LED
		while(true){
				//writeDebugStreamLine("IR %d", SensorValue[IR_sensor]);
				if(SensorValue[IR_sensor] > flameOff){
					SensorValue[fan] = 1;	//start fan
					wait1Msec(3000);
				}else{
					SensorValue[fan] = 0;	//off fan
					wait1Msec(1000);
					SensorValue[redLed] = 1;	//off LED
					isFlameOff = true;  // Means that flame is off so that the robot goes back to the start
					isFlameDetected = false; // reset to prevent duplicate scan on reverse direction
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
 * Scan flame on clockwise
 */
int right4flame(int degrees, int speed){
	int _flame0 = 0;
	int _flameTicks = 0;

	//you must reset the encoders
	resetEncoders();

	int tickGoal = ((rightTicks * degrees)/10);	//increased the tick goal by 1 today by mattyboio == 74@matt

	motor[leftMotor]=speed;
	motor[rightMotor]=-1*speed;

	while(nMotorEncoder[leftMotor]<tickGoal||nMotorEncoder[rightMotor]>-1*tickGoal){
			// flame is detected, stop motor, turn on LED, return ticks-run so far
			int _flameCurr = SensorValue[IR_sensor];
			if(_flameCurr > _flame0){	// if hear is increasing, not facing straight yet
				//writeDebugStreamLine("flame level %d %d %d", _flame0, _flameCurr, _flameTicks);
				_flame0 = _flameCurr;
				_flameTicks = abs(nMotorEncoder[rightMotor]);
				writeDebugStreamLine("flame level %d %d", _flameCurr, _flameTicks);
			}

		if(nMotorEncoder[leftMotor]>tickGoal){
			motor[leftMotor]=0;
		}
		if(nMotorEncoder[rightMotor]<-1*tickGoal){
			motor[rightMotor]=0;
		}
	}

	if(_flame0 > flameDetected){
		isFlameDetected = true;
		return _flameTicks;
	}

	return -1;	//meaning turn is completed
}

int left4flame(int degrees, int speed){
	int _flame0 = 0;
	int _flameTicks = 0;

	//you must reset the encoders
	resetEncoders();

	int tickGoal = ((leftTicks * degrees)/10);	//increased the tick goal by 1 today by mattyboio == 74@matt

	motor[rightMotor]=speed;
	motor[leftMotor]=-1*speed;

	while(nMotorEncoder[rightMotor]<tickGoal||nMotorEncoder[leftMotor]>-1*tickGoal){
			// flame is detected, stop motor, turn on LED, return ticks-run so far
			int _flameCurr = SensorValue[IR_sensor];
			if(_flameCurr > _flame0){	// if hear is increasing, not facing straight yet
				writeDebugStreamLine("flame level %d %d %d", _flame0, _flameCurr, _flameTicks);
				_flame0 = _flameCurr;
				_flameTicks = abs(nMotorEncoder[leftMotor]);
			}

		if(nMotorEncoder[rightMotor]>tickGoal){
			motor[rightMotor]=0;
		}
		if(nMotorEncoder[leftMotor]<-1*tickGoal){
			motor[leftMotor]=0;
		}
	}

	if(_flame0 > flameDetected){
		isFlameDetected = true;
		return _flameTicks;
	}

	return -1;	//meaning turn is completed
}

/*
 * scan clockwise, stop when flame is detected
 */
int turnRight(int degrees, int speed, int offset){
	//you must reset the encoders
	resetEncoders();

	int tickGoal = ((rightTicks * degrees)/10 - offset);	//increased the tick goal by 1 today by mattyboio == 74@matt

	motor[leftMotor]=speed;
	motor[rightMotor]=-1*speed;

	while(nMotorEncoder[leftMotor]<tickGoal||nMotorEncoder[rightMotor]>-1*tickGoal){
		if(nMotorEncoder[leftMotor]>tickGoal){
			motor[leftMotor]=0;
		}
		if(nMotorEncoder[rightMotor]<-1*tickGoal){
			motor[rightMotor]=0;
		}
	}
	return abs(nMotorEncoder[rightMotor]);	//meaning turn is completed
}

int turnLeft(int degrees, int speed, int offset){
	//you must reset the encoders
	resetEncoders();

	int tickGoal = ((leftTicks * degrees)/10 - offset);	//increased the tick goal by 1 today by mattyboio == 74@matt

	motor[rightMotor]=speed;
	motor[leftMotor]=-1*speed;

	while(nMotorEncoder[rightMotor]<tickGoal||nMotorEncoder[leftMotor]>-1*tickGoal){
		if(nMotorEncoder[rightMotor]>tickGoal){
			motor[rightMotor]=0;
		}
		if(nMotorEncoder[leftMotor]<-1*tickGoal){
			motor[leftMotor]=0;
		}
	}
	return abs(nMotorEncoder[leftMotor]);	//meaning turn is
}

void left4startDone(){
		turnLeft(90,60,0);
		completeStop(1000);
		while(SensorValue[frontUltra]>16){
				walkStraight(lowSpd,comSpd);
		}
		completeStop(1000);
		turnRight(180,60,0);
		completeStop(0);
}

void close2wall(){
		resetEncoders();
		while(SensorValue[frontUltra]>10){
			walkStraight(lowSpd-20, comSpd-20);
		}
		completeStop(500); // increase to 1500 to get more close to wall -- updated by Qian on 03/05
}

void positionAdjByRightUltra(){
		wait1Msec(1000);
		if(SensorValue[rightUltra] > 20){
			turnRight(90, 45, 0);
			completeStop(1000);
			close2wall();
			turnLeft(90, 45, 0);
			completeStop(1000);
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

		// check whether robot is facing room#2
		if(SensorValue[rightUltra]>2*rightSpace){
			turnRight(90,45,0);
			completeStop(500);
		}

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
		turnRight(90, 45, 0);//turn to the room so we can almost enter.
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
		while(SensorValue[frontUltra]>20){
			walkStraight(lowSpd-20, comSpd-20);
		}
		completeStop(1500);

		turnRight(90, 45, 0);	//turn to the room so we can almost enter.
		completeStop(1000);

		//push robot into room#1 to reduce front ultrasonic false reading
		resetEncoders();
		walkStraight(lowSpd, comSpd);
		wait1Msec(1000);
		//drive into the room
		resetEncoders();
		while(SensorValue[frontUltra]>40){
			walkStraight(lowSpd, comSpd);  //Walk straight
		}
		completeStop(1000);	//stop close to 30cm

		// updated by Qian on 03/04/18
		turnLeft(30, 45, 0); // to cover all directions
		int _ticks1 = right4flame((180+30), 45);
		completeStop(1000);
		if(isFlameDetected){
			turnLeft((180+30), 45, 0); //back to start point
			completeStop(1000);

			int _ticks3 = turnRight(((_ticks1*10/rightTicks) - flameTargetAdj), 45, 0);
			SensorValue[redLed] = 0; // turn on LED
			putOffFlame(); // put off flame
			// how to finish the rest of turn
			turnRight((180+30), 45, _ticks3);
			completeStop(1000);
		}

		//leave the room
		//use close ultrasonic for navigation to reduce noise
		resetEncoders();
		while(SensorValue[leftUltra] < 80){
				walkStraight(30, 40);
		}
		completeStop(0);
		// @ hallway
		resetEncoders();
		while(SensorValue[frontUltra]>15){
				walkStraight(lowSpd-20, comSpd-20);
		}
		completeStop(1000);//exit the room

		//make 90 left turn
		turnLeft(90, 45, 0);//turn to the room so we can almost enter.
		completeStop(1000);

		resetEncoders();
		while(SensorValue[rightUltra]<rightSpace){
			walkStraight(lowSpd, comSpd);
		}
		completeStop(0);

		//allow the robot to move a little more, position @ the center of intersection
		resetEncoders();
		walkStraight(lowSpd, comSpd);
		wait1Msec(150);
		completeStop(500);

		//end as finishing room#1
		//if candle off should return to start point
		if(isFlameOff){
			left4startDone();
		}else{
			// continue room#3
			resetEncoders();
			walkStraight(lowSpd-20, comSpd-20);
			wait1Msec(200);
			completeStop(50);
			//start room#3
			turnRight(90, 45, 0);//turn to the room so we can almost enter.
			completeStop(1000);
			//allow the robot to move forward, to reduce ultrasonic noise
			resetEncoders();
			walkStraight(lowSpd, comSpd);
			wait1Msec(1000);

			resetEncoders();
			while(SensorValue[rightUltra]<rightSpace){
				walkStraight(lowSpd, comSpd);
			}
			completeStop(1000);

			close2wall();

			turnRight(90, 45, 0);//turn to the room so we can almost enter.
			completeStop(500);

			//drive into the room#3
			//allow the robot to move forward, to reduce ultrasonic noise
			resetEncoders();
			walkStraight(lowSpd-20, comSpd-20);
			/*wait1Msec(10); // just get into room#3 gateway
			resetEncoders();
			while(SensorValue[rightUltra]>80){
				walkStraight(lowSpd-20, comSpd-20);
			}*/
			wait1Msec(1000); // just get into room#3 gateway
			int _delay = 0;

			// updated by Qian on 03/06/18 -- scan @ entry
			turnLeft(60, 45, 0); // to cover all directions
			int _ticks1_3 = right4flame((60+60), 45);
			completeStop(1000);
			if(isFlameDetected){
				if(_ticks1_3 < (90*rightTicks/10)){ // meaning flame is on the left 60+30 angles range
					turnLeft((60+60), 45, 0); //back to start point
					completeStop(1000);
					int _ticks3_3 = turnRight(((_ticks1_3*10/rightTicks) - flameTargetAdj), 45, 0);
					SensorValue[redLed] = 0; // turn on LED
					putOffFlame(); // put off flame
					// how to finish the rest of turn
					turnRight((60+180), 45, _ticks3_3);
					completeStop(1000);
					_delay = 200;
				}else{ // flame is on right side more than 30 angles
					isFlameDetected = false; //reset for moving forward on right side scan only
				}
			}
			if(!isFlameOff){
					// move deep into the room
					turnLeft(60, 45, 0); //back to start point
					completeStop(1000);
					resetEncoders();
					while(SensorValue[frontUltra]>30){
						walkStraight(lowSpd, comSpd);  //Walk straight
					}
					completeStop(1000);	//stop close to 30cm
					int _ticks1_3b = right4flame(180, 45);
					completeStop(1000);
					if(isFlameDetected){
						turnLeft(180, 45, 0); //back to start point
						completeStop(1000);
						int _ticks3_3b = turnRight(((_ticks1_3b*10/rightTicks) - flameTargetAdj), 45, 0);
						SensorValue[redLed] = 0; // turn on LED
						putOffFlame(); // put off flame
						// how to finish the rest of turn
						turnRight(180, 45, _ticks3_3b);
						completeStop(1000);
						_delay = 0;
					}
			}
			//adjust robot position to have a better exit way
			positionAdjByRightUltra();
			resetEncoders();
			while(SensorValue[leftUltra]<80){
				walkStraight(lowSpd-20, comSpd-20);
			}
			completeStop(500);

			//how to position the robot @ the center of the hallway ?? still not good @ case#1
			positionAdjByRightUltra();
			walkStraight(lowSpd-20, comSpd-20);
			wait1Msec(200 + _delay);
			completeStop(500);

			if(isFlameOff){
				left4startDone();
			}else{
				// start room#4
				resetEncoders();
				walkStraight(lowSpd-20, comSpd-20);
				wait1Msec(1000);
				resetEncoders();
				while(SensorValue[leftUltra] > 70){
					walkStraight(lowSpd-20, comSpd-20);
				}
				completeStop(100); // just get into room#4 gateway

				// updated by Qian on 03/06/18 -- scan @ entry
				turnRight(60, 45, 0); // to cover all directions
				int _ticks1_4 = left4flame((60+60), 45);
				completeStop(1000);
				if(isFlameDetected){
					if(_ticks1_4 < (90*leftTicks/10)){ // meaning flame is on the right 60+30 angles range
						turnRight((60+60), 45, 0); //back to start point
						completeStop(1000);
						int _ticks3_4 = turnLeft(((_ticks1_4*10/leftTicks)), 45, 0);
						SensorValue[redLed] = 0; // turn on LED
						putOffFlame(); // put off flame
						// how to finish the rest of turn
						turnLeft((60+180), 45, _ticks3_4);
						completeStop(1000);
						_delay = 200;
					}else{ // flame is on right side more than 30 angles
						isFlameDetected = false; //reset for moving forward on right side scan only
					}
				}
				if(!isFlameOff){
					// move deep into the room
					turnRight(60, 45, 0); //back to start point
					completeStop(1000);
				}

			}

		}
}
