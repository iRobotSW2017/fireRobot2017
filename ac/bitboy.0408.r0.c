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

//global variables
		//int comSpd = 90;
		//int lowSpd = 70;
		int comSpd = 60;//was 50
		int lowSpd = 50;//40
		int rightTicks = 45; //40
		int leftTicks = 43;	//41
		int turnSpd = 60;//50
		//float halfSec50dist = 15.4;
		float stdLength = 7.7; //cm
		int minDistant = 12;
		int ticks360As33cm = 559;
		int rightSpace = 23;
		int flameDetected = 250;	//150, 120
		int flameTargetAdj = 10; // 8
		int flameTargetLeftAdj = -10;
		int flameOff = 80; // 100
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
			motor[leftMotor] = CommonSpeed +7;
			motor[rightMotor] = lowSpeed;
	//		wait1Msec(500);
	}else if(_right < 10){
			motor[rightMotor] = CommonSpeed +7;
			motor[leftMotor] = lowSpeed;
	//		wait1Msec(500);
	}else if((_right < rightSpace) && (_left < rightSpace) && (abs(_left - _right) >= 3)){
		if(_right < _left){
			motor[rightMotor] = CommonSpeed +7;
			motor[leftMotor] = lowSpeed;
		}else if(_right > _left){
			motor[leftMotor] = CommonSpeed +7;
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
				//writeDebugStreamLine("flame level %d %d", _flameCurr, _flameTicks);
			}

		if(nMotorEncoder[leftMotor]>tickGoal){
			motor[leftMotor]=0;
		}
		if(nMotorEncoder[rightMotor]<-1*tickGoal){
			motor[rightMotor]=0;
		}
	}
	motor[rightMotor]=0;
	motor[leftMotor]=0;

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
				//writeDebugStreamLine("flame level %d %d %d", _flame0, _flameCurr, _flameTicks);
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
	motor[rightMotor]=0;
	motor[leftMotor]=0;

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
	motor[rightMotor]=0;
	motor[leftMotor]=0;
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
	motor[rightMotor]=0;
	motor[leftMotor]=0;
	return abs(nMotorEncoder[leftMotor]);	//meaning turn is
}

/*
 * move closer, put off flame, LED off, backward
 */
void putOffFlame(){
	int _moveClose = 0;
	int _slowCloser = lowSpd - 10;
	//only do when flame is detected
	if(isFlameDetected){
		//move close if front > 30
		resetEncoders();
		while(SensorValue[frontUltra] > 20){ //20=>15
			walkStraight(_slowCloser, _slowCloser);
		}
		_moveClose = abs(nMotorEncoder[rightMotor]); //return encoder value for reverse purpose
		completeStop(500);
		//start fan until flame is off, & turn off LED
		bool _first = true;
		while(true){
				wait1Msec(100);
				writeDebugStreamLine("IR %d", SensorValue[IR_sensor]);
				if(SensorValue[IR_sensor] > flameOff){
					if(_first){
						SensorValue[fan] = 1;	//start fan
						wait1Msec(3000);
						_first = false;
						SensorValue[fan] = 0;	//stop fan
					}else{
						//-30~+30
						turnLeft(60, turnSpd, 0);
						int _extraTicks = right4flame((60+60), lowSpd); //changed from r40 to r60 for equal amount scan
						int _extraTicks2 = turnLeft((60+60-_extraTicks*10/rightTicks), lowSpd, 0);
						SensorValue[fan] = 1;	//start fan
						wait1Msec(3000);
						SensorValue[fan] = 0;	//stop fan
						int _leftdeg = _extraTicks2*10/leftTicks - 60;
						if(_leftdeg < 0){
							turnLeft(-1*_leftdeg, turnSpd, 0);
						}else{
							turnRight(_leftdeg, turnSpd, 0);
						}
					}
				}else{
					SensorValue[fan] = 0;	//off fan
					wait1Msec(1000);
					if(SensorValue[IR_sensor] <= flameOff){
						SensorValue[redLed] = 1;	//off LED
						isFlameOff = true;  // Means that flame is off so that the robot goes back to the start
						isFlameDetected = false; // reset to prevent duplicate scan on reverse direction
						break;
					}
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

void close2wall(){
		resetEncoders();
		while(SensorValue[frontUltra]>minDistant){
			walkStraight(lowSpd-20, comSpd-20);
		}
		completeStop(500); // increase to 1500 to get more close to wall -- updated by Qian on 03/05
}

/*
 * Use right ultrasound scanner to move robot middle of hallway position
 */
void positionAdjByRightUltra(){
		wait1Msec(1000);
		if(SensorValue[rightUltra] > minDistant){
			turnRight(90, turnSpd,0);
			completeStop(1000);
			close2wall();
			turnLeft(90, turnSpd, 0);
			completeStop(1000);
		}
}

/*
 * Use left ultrasound scanner to move robot middle of hallway position
 */
void positionAdjByLeftUltra(){
		wait1Msec(1000);
		if(SensorValue[leftUltra] > minDistant){
			turnLeft(90, turnSpd,0);
			completeStop(1000);
			close2wall();
			turnRight(90, turnSpd, 0);
			completeStop(1000);
		}
}

/*
 * move forward as measuring by 1 revolution of the wheel's rotation
 */
void moveforward(int distanceCM, int speed){
			motor[leftMotor]=0;
			motor[rightMotor]=0;
			resetEncoders();

			int tickGoal = ticks360As33cm/33 * distanceCM; //588  627==36~38
			motor[rightMotor]=speed;
			motor[leftMotor]=speed;
			while(nMotorEncoder[rightMotor]<tickGoal){
					// keep running
			}
			motor[rightMotor]=0;
			motor[leftMotor]=0;
}

/*
 * try to adj the robot's body by half sec ultrasonic
 */
void adjustRobotByRightUltra(){
		//SensorValue[redLed] = 0;
		motor[rightMotor]=0;
		motor[leftMotor]=0;
		int _right0 = SensorValue[rightUltra];
		//motor[rightMotor]=turnSpd;
		//motor[leftMotor]=turnSpd;
		//wait1Msec(250);
		moveforward(stdLength, turnSpd);
		int _right1 = SensorValue[rightUltra];
		// calculate degree adj
		//int _results = asin(abs(_right0-_right1)/(halfSec50dist/2)) * 180 / 3.1415;
		int _results = asin(abs(_right0-_right1)/stdLength) * 180 / 3.1415;

		if(_right0<_right1){
			//turn right a little
			turnRight(_results, turnSpd, 0);
		}else if(_right0>_right1){
			//turn right a little
			turnLeft(_results, turnSpd, 0);
		}
		//SensorValue[redLed] = 1;
}

/*
 * try to adj the robot's body by half sec ultrasonic
 */
void adjustRobotByLeftUltra(){
		//SensorValue[redLed] = 0;
		motor[rightMotor]=0;
		motor[leftMotor]=0;
		int _left0 = SensorValue[leftUltra];
		//motor[rightMotor]=turnSpd;
		//motor[leftMotor]=turnSpd;
		//wait1Msec(250);
		moveforward(stdLength, turnSpd);
		int _left1 = SensorValue[leftUltra];
		// calculate degree adj
		//int _results = asin(abs(_left0-_left1)/(halfSec50dist/2)) * 180 / 3.1415;
		int _results = asin(abs(_left0-_left1)/stdLength) * 180 / 3.1415;

		if(_left0<_left1){
			//turn right a little
			turnLeft(_results, turnSpd, 0);
		}else if(_left0>_left1){
			//turn right a little
			turnRight(_results, turnSpd, 0);
		}
		//SensorValue[redLed] = 1;
}

void jobWellDone(int LorR){
		switch(LorR){
			case 2:
				turnRight(90, turnSpd,0);
			break;
			default:
				turnLeft(90, turnSpd,0);
		}
		completeStop(1000);
		resetEncoders();

		while(SensorValue[frontUltra]>minDistant){
				walkStraight(lowSpd,comSpd);
		}
		completeStop(1000);
	//	turnRight(180, turnSpd,0);
	//	completeStop(0);
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
			turnRight(90, turnSpd,0);
			completeStop(500);
			adjustRobotByRightUltra();
			adjustRobotByRightUltra();
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
		clearTimer(T1);
		while(time1(T1)<350){
			walkStraight(lowSpd, comSpd);
		}
		completeStop(500);

		//make 90 turn, going to room#1 direction
		turnRight(90, turnSpd, 0);//turn to the room so we can almost enter.
		completeStop(1000);

		//allow the robot to move forward
		resetEncoders();
		clearTimer(T1);
		while(time1(T1)<1000){
			walkStraight(lowSpd, comSpd);
		}

		resetEncoders();
		while(SensorValue[rightUltra]<rightSpace){
			walkStraight(lowSpd, comSpd);
		}
		completeStop(0);

		close2wall();

		turnRight(90, turnSpd, 0);	//turn to the room so we can almost enter.
		completeStop(1000);

		adjustRobotByLeftUltra();
		//push robot into room#1 to reduce front ultrasonic false reading
		resetEncoders();
		clearTimer(T1);
		while(time1(T1)<1000){
			walkStraight(lowSpd, comSpd);
		}
		adjustRobotByLeftUltra();

		//drive into the room
		resetEncoders();
		while(SensorValue[frontUltra]>30){
			walkStraight(lowSpd, comSpd);  //Walk straight
		}
		completeStop(1000);	//stop close to 30cm

		// updated by Qian on 03/04/18
		turnLeft(60, turnSpd, 0); // to cover all directions
		int _ticks1 = right4flame((180+60), turnSpd);
		completeStop(1000);
		if(isFlameDetected){
			int _frameAdj = flameTargetLeftAdj;
			if(_ticks1*10/rightTicks > 60){
				//if +180 havig adj
				_frameAdj = flameTargetAdj;
			}
			int _ticks3 = turnLeft(((180+60) - _ticks1*10/rightTicks - _frameAdj), turnSpd, 0);
			completeStop(1000);
			SensorValue[redLed] = 0; // turn on LED
			putOffFlame(); // put off flame
			// how to finish the rest of turn
			turnRight((_ticks3*10/leftTicks), turnSpd, 0);
			completeStop(1000);

		}

		// .5 sec adj robot
		positionAdjByRightUltra();
		adjustRobotByRightUltra();
		adjustRobotByRightUltra();

		//leave the room
		//use close ultrasonic for navigation to reduce noise
		resetEncoders();
		while(SensorValue[leftUltra] < 40){
				walkStraight(lowSpd -10, comSpd -10);
		}
		completeStop(0);
		// @ hallway
		close2wall();

		//make 90 left turn
		turnLeft(90, turnSpd, 0);//turn to the room so we can almost enter.
		completeStop(1000);
		adjustRobotByRightUltra();
		adjustRobotByRightUltra();

		resetEncoders();
		while(SensorValue[rightUltra]<rightSpace){
			walkStraight(lowSpd, comSpd);
		}
		completeStop(0);

		//allow the robot to move a little more, position @ the center of intersection
		resetEncoders();
		clearTimer(T1);
		while(time1(T1)<202){
			walkStraight(lowSpd, comSpd);
		}
		completeStop(500);

		//end as finishing room#1
		//if candle off should return to start point
		if(isFlameOff){
			moveforward(3,lowSpd);
			jobWellDone(1);
		}else{
			// continue room#3
			moveforward(5, lowSpd);
			//start room#3
			turnRight(90, turnSpd, 0);//turn to the room so we can almost enter.
			completeStop(1000);
			//allow the robot to move forward, to reduce ultrasonic noise
			moveforward(40, lowSpd);

			completeStop(0);
			positionAdjByRightUltra();
			positionAdjByLeftUltra();

			resetEncoders();
			while(SensorValue[rightUltra]<rightSpace){
				walkStraight(lowSpd-10, comSpd-10);
			}
			completeStop(1000);
			close2wall();

			turnRight(90, turnSpd, 0);//turn to the room so we can almost enter.
			completeStop(500);
			// adj by leftUltra
			adjustRobotByLeftUltra();
			adjustRobotByLeftUltra();

			//drive into the room#3
			moveforward(30, lowSpd-10);

			// updated by Qian on 03/06/18 -- scan @ entry
			turnLeft(60, turnSpd, 0); // to cover all directions
			int _ticks1_3 = right4flame((60+60), turnSpd);
			completeStop(1000);
			if(isFlameDetected){
				if((_ticks1_3*10/rightTicks) < 90){ // meaning flame is on the left 60+30 angles range

					int _frameAdj = flameTargetLeftAdj;
					if(_ticks1_3*10/rightTicks > 60){
						//if +180 havig adj
						//_frameAdj = flameTargetAdj;
					}

					int _ticks3_3 = turnLeft((60+60-(_ticks1_3*10/rightTicks) - _frameAdj), turnSpd, 0); //back to start point
					completeStop(1000);
					SensorValue[redLed] = 0; // turn on LED
					putOffFlame(); // put off flame
					// how to finish the rest of turn
					turnRight((_ticks3_3*10/leftTicks + 120), turnSpd, 0);
					completeStop(1000);

				}else{ // flame is on right side more than 30 angles
					isFlameDetected = false; //reset for moving forward on right side scan only
				}
			}
			if(!isFlameOff){
					// move deep into the room
					turnLeft(60, turnSpd, 0); //back to start point
					completeStop(1000);
					resetEncoders();
					while(SensorValue[frontUltra]>(25+stdLength)){
						walkStraight(lowSpd, comSpd);  //Walk straight
					}
					completeStop(100);
					//adjust robot body to be staright
					adjustRobotByLeftUltra();
					completeStop(500);
					turnRight(90, turnSpd, 0);
					completeStop(100);
					while(SensorValue[frontUltra] > 40){
						walkStraight(lowSpd, comSpd);
					}
					completeStop(100);
					turnLeft(90,turnSpd, 0);
					completeStop(100);

					int _ticks1_3b = right4flame(180, turnSpd);
					writeDebugStreamLine("degrees %d", _ticks1_3b*10/rightTicks);
					completeStop(1000);
					if(isFlameDetected){

						int _frameAdj = 1.5*flameTargetLeftAdj;
						if(_ticks1_3b*10/rightTicks > 60){
							//if +180 havig adj
							_frameAdj = flameTargetAdj;
						}
						int _ticks3_3b = turnLeft((180 - (_ticks1_3b*10/rightTicks) - _frameAdj), turnSpd, 0);
						writeDebugStreamLine("degrees %d", _ticks3_3b*10/leftTicks);
						SensorValue[redLed] = 0; // turn on LED
						putOffFlame(); // put off flame
						// how to finish the rest of turn
						turnRight(_ticks3_3b*10/leftTicks, turnSpd, 0);
						completeStop(1000);
					}
			}
			//adjust robot position to have a better exit way
			positionAdjByRightUltra();
			adjustRobotByRightUltra();

			resetEncoders();
			while(SensorValue[leftUltra]<80){
				walkStraight(lowSpd-30, comSpd-30);
			}
			completeStop(0);

			//how to position the robot @ the center of the hallway ?? still not good @ case#1
			positionAdjByRightUltra();
			adjustRobotByRightUltra();

			moveforward(15, lowSpd-20);
			//completeStop(1000);

			if(isFlameOff){
				jobWellDone(1);
			}else{
				// start room#4
				//positionAdjByRightUltra();
				adjustRobotByRightUltra();

				moveforward((38+8), lowSpd-20);
				turnLeft(90, turnSpd, 0);
				moveforward(8, lowSpd-10);
				turnRight(90, turnSpd, 0);

				// updated by Qian on 03/06/18 -- scan @ entry
				turnRight(60, turnSpd, 0); // to cover all directions
				int _ticks1_4 = left4flame((60+60), turnSpd);
				completeStop(1000);
				if(isFlameDetected){
					if((_ticks1_4*10/leftTicks) < 90){ // meaning flame is on the right 60+30 angles range
						int _ticks3_4 = turnRight((60+60-(_ticks1_4*10/leftTicks)- 1.5*flameTargetLeftAdj), turnSpd, 0);
						if(SensorValue[frontUltra] > (30+20-8)){
							//move forward
							int _deg4 = (_ticks3_4*10/rightTicks - 30);
							if(_deg4<0){
								turnRight(-1*_deg4, turnSpd, 0);
							}else{
								turnLeft(_deg4, turnSpd, 0);
							}
							adjustRobotByRightUltra();
							moveforward(25, lowSpd);
							turnRight(30, turnSpd, 0); // to cover all directions
							int _ticks1_4c = left4flame((30+30), turnSpd);
							//completeStop(1000);
							int _ticks3_4c = turnRight((30+30-(_ticks1_4c*10/leftTicks)- 1.5*flameTargetLeftAdj), turnSpd, 0);
							SensorValue[redLed] = 0; // turn on LED
							putOffFlame(); // put off flame
							turnLeft((150+_ticks3_4c*10/rightTicks), turnSpd, 0);
							adjustRobotByLeftUltra();
							moveforward(23, lowSpd);
						}else{
							SensorValue[redLed] = 0; // turn on LED
							putOffFlame(); // put off flame
							// how to finish the rest of turn
							turnLeft((120+_ticks3_4*10/rightTicks), turnSpd, 0);
						}
						completeStop(1000);
					}else{ // flame is on right side more than 30 angles
						isFlameDetected = false; //reset for moving forward on right side scan only
					}
				}
				if(!isFlameOff){
					// move deep into the room
					turnRight(60, turnSpd, 0); //back to start point
					completeStop(1000);
					positionAdjByRightUltra();
					adjustRobotByRightUltra();
					adjustRobotByRightUltra();
					//if(SensorValue[frontUltra]> (122-35-30)){
						moveforward((12-8), lowSpd-10);
					//}
					int _ticks1_4b = left4flame(180, turnSpd);
					completeStop(1000);
					if(isFlameDetected){
						int _ticks3_4b = turnRight((180 - (_ticks1_4b*10/leftTicks)), turnSpd, 0);
						SensorValue[redLed] = 0; // turn on LED
						putOffFlame(); // put off flame
						// how to finish the rest of turn
						turnLeft(_ticks3_4b*10/rightTicks, turnSpd, 0);
						completeStop(1000);
					}
				}

				positionAdjByLeftUltra();
				adjustRobotByLeftUltra();
				adjustRobotByLeftUltra();
				//adjustRobotByLeftUltra();

				resetEncoders();
				while(SensorValue[rightUltra]<65){
					walkStraight(lowSpd-30, comSpd-30);
				}
				completeStop(0);

				//how to position the robot @ the center of the hallway ?? still not good @ case#1
				positionAdjByLeftUltra();
				adjustRobotByLeftUltra();

				moveforward(20, lowSpd-20);//10, lowspd-10

				if(isFlameOff){
					jobWellDone(2);
				}else{
					// start room#2
					turnRight(90, turnSpd, 0);
					completeStop(1000);

					moveforward(40, comSpd);
					positionAdjByRightUltra();
					adjustRobotByRightUltra();
					resetEncoders();
					while(SensorValue(rightUltra)< rightSpace){
							walkStraight(lowSpd, comSpd);
					}
					moveforward(65, comSpd);
					//start position adjust
					positionAdjByRightUltra();
					positionAdjByLeftUltra();
					adjustRobotByRightUltra();
					//end position adjust
					resetEncoders();
					while(SensorValue(frontUltra)>15){
							walkStraight(lowSpd, comSpd);
					}
					completeStop(1000);

					turnRight(90, turnSpd, 0);
					completeStop(1000);

					bool _isContinueR2 = true; //in case detect dog before getting into room#2
					bool _inWallR2 = false;
					resetEncoders();
					int _right2 = 100;
					while(SensorValue(frontUltra) > 20){
							walkStraight(lowSpd-20, comSpd-20);
							_right2 = SensorValue(rightUltra);
							//writeDebugStreamLine("right sensor %d", _right2);
							if(!_inWallR2){
									if(_right2 < 16) _inWallR2 = true;
							}else{ // start to hit wall
									if(_right2 > 50){
										// detect the room#2 open
										_isContinueR2 = false;
										moveforward(12, turnSpd); //20
										positionAdjByLeftUltra();
										turnRight(90, turnSpd, 0);
										break;
									}
							}

					}
					completeStop(0);

					if(_isContinueR2){
						resetEncoders();
						turnRight(90, turnSpd, 0);
						//completeStop(1000);
						//adjustRobotByRightUltra();
					//	close2wall();
				//		resetEncoders();
				//		turnLeft(90, (turnSpd-10), 0);
						adjustRobotByRightUltra();
						adjustRobotByRightUltra();
						//hop like a bonnie rabbit
						moveforward(15, comSpd);
						//first leg of da daetour. this is so cool
						resetEncoders();
						while(!(SensorValue(leftUltra) > (2*rightSpace))){
							walkStraight(lowSpd, comSpd);
						}
						//completeStop(1000);

						//hop like a bonnie wabbit v2
						moveforward(22, turnSpd);
						//self explanitory
						turnLeft(90, turnSpd, 0);
						//hop like a wonnie babbit v3
						moveforward(40, comSpd);

						//second leg of da daetour. this is also so cool
						/*resetEncoders();
						while(SensorValue(rightUltra)<rightSpace){
							walkStraight(lowSpd, comSpd);
						}*/
						positionAdjByLeftUltra();
						positionAdjByRightUltra();
						adjustRobotByLeftUltra();
						moveforward(30, comSpd);

						//vop hike l aonnie wabbit b4
						close2wall();
						turnLeft(90, turnSpd, 0);
						adjustRobotByRightUltra();
						adjustRobotByRightUltra();
						moveforward(20, comSpd);
						//third leg of dae daetor. t  h  i  s     i  s     a  l  s  o     c  o  o  l
						resetEncoders();
						while(SensorValue(leftUltra)<rightSpace){
							walkStraight(lowSpd, comSpd);
						}
						completeStop(1000);

						//bop vike h lonnie aabbit w5
						close2wall();
						//eoly sxplanitairf
						turnLeft(90, turnSpd, 0);
						adjustRobotByRightUltra();
						adjustRobotByRightUltra();

						//wop bike v honnie labbit a6
						resetEncoders();
						while(true){
							walkStraight(lowSpd-10, comSpd-10);
							if(SensorValue(leftUltra) < 90) break;
						}
						completeStop(0);
						moveforward(30, lowSpd-10);

						turnLeft(80, turnSpd, 0);
						completeStop(0);
					}

					moveforward(15, turnSpd);
					completeStop(0);
					if(SensorValue(leftUltra) < rightSpace){
						positionAdjByLeftUltra();
					}
					adjustRobotByLeftUltra();
					adjustRobotByLeftUltra();
					//completeStop(0);
					//moveforward(4, turnSpd);
					//completeStop(0);
					while(SensorValue(frontUltra) > 15){
						walkStraight(lowSpd-10, lowSpd-10);
					}
					//return to start, all done
					// start flame detecting
					turnLeft(30, turnSpd, 0);
					int _ticks1_2b = right4flame((180+30), turnSpd);
					completeStop(1000);
					if(isFlameDetected){
							int _ticks3_2b = turnLeft(((180+30) - (_ticks1_2b*10/rightTicks) - flameTargetAdj), turnSpd, 0);
							//writeDebugStreamLine("room2 %d %d", (_ticks1_2b*10/rightTicks), ((180+30) - (_ticks1_2b*10/rightTicks) - flameTargetAdj));
							SensorValue[redLed] = 0; // turn on LED
							putOffFlame(); // put off flame
							// how to finish the rest of turn
							turnRight(_ticks3_2b*10/leftTicks, turnSpd, 0);
							completeStop(1000);
					}

					//return to start, all done
					positionAdjByRightUltra();
					adjustRobotByRightUltra();
					close2wall();
					if(!_isContinueR2){
						jobWellDone(1);
					}else{
						turnRight(90, turnSpd, 0);
						adjustRobotByLeftUltra();
						close2wall();
						turnRight(90, turnSpd, 0);
						adjustRobotByLeftUltra();
						moveforward(20, comSpd);
						resetEncoders();
						while(!(SensorValue(rightUltra) > (2*rightSpace))){
							walkStraight(lowSpd, comSpd);
						}
						moveforward(22, turnSpd);
						//self explanitory
						turnRight(90, turnSpd, 0);
						//hop like a wonnie babbit v3
						moveforward(45, comSpd);
						//second leg of da daetour. this is also so cool
						/*resetEncoders();
						while(SensorValue(rightUltra)<rightSpace){
							walkStraight(lowSpd, comSpd);
						}*/
						positionAdjByRightUltra();
						adjustRobotByRightUltra();
						moveforward(10, lowSpd);
						resetEncoders();
						while(!(SensorValue(rightUltra) > (2*rightSpace))){
							walkStraight(lowSpd, comSpd);
						}
						moveforward(20, lowSpd);
						jobWellDone(2);

					}

				 }
			}
		}
}
