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
		int comSpd = 50;
		int lowSpd = 40;
		int rightTicks = 41; //60
		int leftTicks = 41;	//57
		int turnSpd = 50;
		//int comAdjSpd = 31;
		//int delaySec = 450;
		//int frontSpace = 12; //(46-30)/2
		int rightSpace = 23;
		int flameDetected = 120;	//250
		int flameTargetAdj = 0; // 8
		int flameOff = 100;
		bool isFlameDetected = false;
		bool isFlameOff = false;
		int senseDog1 = -1;
		int senseDog2 = -1;

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
			turnRight(90, turnSpd,0);
			completeStop(1000);
			close2wall();
			turnLeft(90, turnSpd, 0);
			completeStop(1000);
		}
}

bool isDogFound(){
		wait1Msec(500);
		senseDog1 = SensorValue(rightUltra);
		wait1Msec(100);
		senseDog2 = SensorValue(rightUltra);

		bool _open1 = false;
		bool _open2 = false;
		bool _open3 = false;

		if(senseDog1 >=60 && senseDog1 <= 90) _open1 = true;
		if(senseDog2 >=60 && senseDog2 <= 90) _open2 = true;

		if(_open1 && _open2) return false;
		if(_open1 || _open2){
			wait1Msec(100);
			int senseDog3 = SensorValue(rightUltra);
			if(senseDog3 >=60 && senseDog3 <= 90) _open3 = true;
			if (_open3) return false;
		}

	/*	if(senseDog1!= senseDog2) {
			return true;
		}else if(senseDog1<=40 && senseDog2<=40){
			return true;
		}
		*/
		/*if(senseDog1>=40 && senseDog2>=40&& senseDog1<90 && senseDog2<90){
			return false;
		}*/

		return true;

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

				resetEncoders();
				while(SensorValue(frontUltra)>15){
					walkStraight(lowSpd, comSpd);
				}
				completeStop(1000);




			//	if(SensorValue(rightUltra)<2*rightSpace){
			if(isDogFound()){ // determines if the doggo is blocking de wae
				//if the dog is there, turn right.
				turnRight(180, turnSpd, 0);

				completeStop(1000);

				//hop like a bonnie rabbit
				resetEncoders();
				walkStraight(lowSpd, comSpd);
				wait1Msec(500);
				completeStop(1000);

				//first leg of da daetour. this is so cool
				resetEncoders();
				while(SensorValue(leftUltra)<rightSpace){
					walkStraight(lowSpd, comSpd);
				}

				//hop like a bonnie wabbit v2
				resetEncoders();
				walkStraight(lowSpd, comSpd);
				wait1Msec(200);
				completeStop(500);

				//self explanitory
				turnLeft(90, turnSpd, 0);

				//hop like a wonnie babbit v3
				resetEncoders();
				walkStraight(lowSpd, comSpd);
				wait1Msec(500);
				completeStop(500);

				//second leg of da daetour. this is also so cool
				resetEncoders();
				while(SensorValue(leftUltra)<rightSpace){
					walkStraight(lowSpd, comSpd);
				}

				//vop hike l aonnie wabbit b4
				resetEncoders();
				walkStraight(lowSpd, comSpd);
				wait1Msec(400);
				completeStop(500);

				//solf explanitairy
				turnLeft(90, turnSpd, 0);

				resetEncoders();
				walkStraight(lowSpd, comSpd);
				wait1Msec(500);
				completeStop(500);

				//third leg of dae daetor. t  h  i  s     i  s     a  l  s  o     c  o  o  l
				resetEncoders();
				while(SensorValue(leftUltra)<rightSpace){
					walkStraight(lowSpd, comSpd);
				}

				//bop vike h lonnie aabbit w5
				resetEncoders();
				walkStraight(lowSpd, comSpd);
				wait1Msec(300);
				completeStop(500);

				//eoly sxplanitairf
				turnLeft(90, turnSpd, 0);

				//wop bike v honnie labbit a6
				resetEncoders();
				walkStraight(lowSpd, comSpd);
				wait1Msec(1100);
				completeStop(500);

				//fourth leg of dae daetor. this is also cool FLEX ON DEM HATRS
				//resetEncoders();
				//while(SensorValue(leftUltra)<rightSpace){
				//	walkStraight(lowSpd, comSpd);
				//}

				//solf explanitairy once agian begone
				turnLeft(90, turnSpd, 0);

				resetEncoders();
				while(SensorValue[frontUltra]>27){
					walkStraight(lowSpd, comSpd);//go into the room
				}
				completeStop(1000);
				turnRight(180, turnSpd, 0);						// put the flame sense code right here, DAD, my little 13 year old brain can't handle all of this.
				completeStop(1000);//180 degree scan
			}

			turnRight(90, turnSpd, 0);
			completeStop(1000);

			resetEncoders();
			walkStraight(lowSpd, comSpd);
			wait1Msec(300);
			completeStop(1000);

			resetEncoders();
			while(SensorValue(rightUltra)<rightSpace){
				walkStraight(lowSpd, comSpd);
			}
			completeStop(1000);

			resetEncoders();
			walkStraight(lowSpd, comSpd);
			wait1Msec(300);
			completeStop(1000);

			turnRight(90, turnSpd, 0);
			completeStop(1000);

			while(SensorValue(frontUltra)>30){
				walkStraight(lowSpd-20, comSpd-20);
			}


}
