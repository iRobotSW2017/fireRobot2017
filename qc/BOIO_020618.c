#pragma config(I2C_Usage, I2C1, i2cSensors)
#pragma config(Sensor, dgtl3,  frontUltra,     sensorSONAR_cm)
#pragma config(Sensor, dgtl5,  leftUltra,      sensorSONAR_cm)
#pragma config(Sensor, dgtl7,  rightUltra,     sensorSONAR_cm)
#pragma config(Sensor, dgtl9,  Bumpey,         sensorTouch)
#pragma config(Sensor, dgtl12, redLed,         sensorDigitalOut)
#pragma config(Sensor, I2C_1,  ,               sensorQuadEncoderOnI2CPort,    , AutoAssign )
#pragma config(Sensor, I2C_2,  ,               sensorQuadEncoderOnI2CPort,    , AutoAssign )
#pragma config(Motor,  port1,           rightMotor,    tmotorVex393_HBridge, openLoop, encoderPort, I2C_1)
#pragma config(Motor,  port10,          leftMotor,     tmotorVex393_HBridge, openLoop, reversed, encoderPort, I2C_2)
#pragma config(Sensor, in8,    IR_sensor,      sensorAnalog)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

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

void turnRight(int degrees, int speed){
	nMotorEncoder[rightMotor]=0;
	nMotorEncoder[leftMotor]=0;
	//you must reset the encoders

	int tickGoal = (60 * degrees)/10;	//increased the tick goal by 1 today by mattyboio == 74@matt

	motor[leftMotor]=speed;
	motor[rightMotor]=-1*speed;

	while(nMotorEncoder[leftMotor]<tickGoal||nMotorEncoder[rightMotor]>-1*tickGoal){
		if(nMotorEncoder[leftMotor]>tickGoal){
			motor[leftMotor]=0;
		}
		if(nMotorEncoder[rightMotor]<-1*tickGoal){
			motor[rightMotor]=0;
			//this is quality control
		}
	}
}

void turnLeft(int degrees, int speed){
	nMotorEncoder[rightMotor]=0;
	nMotorEncoder[leftMotor]=0;
	//you must reset the encoders

	int tickGoal = (57 * degrees)/10;	//72@Matt

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

void resetEncoders(){
	nMotorEncoder[rightMotor]=0;
	nMotorEncoder[leftMotor]=0;
}

task main()
{

		int comSpd = 90;
		int lowSpd = 70;
		//int comAdjSpd = 31;
		//int delaySec = 450;
		//int frontSpace = 12; //(46-30)/2
		int rightSpace = 23;

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
		turnRight(90, 60);//turn to the room so we can almost enter.
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

		turnRight(90, 60);	//turn to the room so we can almost enter.
		completeStop(1000);


		//push robot into room#1 to reduce front ultrasonic false reading
		resetEncoders();
		walkStraight(lowSpd, comSpd);
		wait1Msec(1000);
		//drive into the room
		resetEncoders();
		while(SensorValue[frontUltra]>20){
			walkStraight(lowSpd, comSpd);  //Walk straight
		}
		completeStop(1000);	//stop close to 30cm

		//180 degree turn
		turnRight(90, 60);//turn to the room so we can almost enter.
		turnRight(90, 60);//turn to the room so we can almost enter.
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

		//start room#3
		//make 90 turn, going to room#1 direction
		turnRight(90, 60);//turn to the room so we can almost enter.
		completeStop(1000);


		//allow the robot to move forward, to reduce ultrasonic noise
		resetEncoders();
		walkStraight(lowSpd, comSpd);
		wait1Msec(1000);
	//	completeStop(500);

		resetEncoders();
		while(SensorValue[rightUltra]<rightSpace){
			walkStraight(lowSpd, comSpd);
		}
		completeStop(1000);

		resetEncoders();
		while(SensorValue[frontUltra]>15){
			walkStraight(lowSpd, comSpd);
		}
		completeStop(1000);

		turnRight(90, 60);//turn to the room so we can almost enter.
		completeStop(1000);

		//drive into the room#3
		//allow the robot to move forward, to reduce ultrasonic noise
		resetEncoders();
		walkStraight(lowSpd, comSpd);
		wait1Msec(1000);
		// enter room#3
		resetEncoders();
		while(SensorValue[frontUltra]>20){
			walkStraight(lowSpd, comSpd);  //Walk straight
		}
		completeStop(1000);	//stop close to 30cm

		turnRight(90, 60);	//scan the room
		turnRight(100, 60);	//scan the room
		completeStop(1000);

		// return to the hallway
		resetEncoders();
		while(SensorValue[leftUltra]<100){
			walkStraight(lowSpd, comSpd);
		}
		completeStop(500);
		// exit room#3
		// stop @ hallway
		resetEncoders();
		walkStraight(lowSpd, comSpd);
		wait1Msec(200);
		completeStop(500);

		// end of room#3
		// if candle detected & off, go back

		// enter room#4
		//allow the robot to move forward, to reduce ultrasonic noise
		resetEncoders();
		walkStraight(lowSpd, comSpd);
		wait1Msec(1500);
		resetEncoders();
		while(SensorValue[frontUltra] > 50){
			walkStraight(lowSpd, comSpd);
		}
		completeStop(1000);

		turnLeft(90, 60);//turn away from the room so we can exit to main hallwaie.
		turnLeft(100, 60);//turn away from the room so we can exit to main hallwaie.
		completeStop(1000);

		resetEncoders();
		while(SensorValue[rightUltra]<100){
			walkStraight(lowSpd, comSpd);//get to main hallwaie
		}
		completeStop(0);

		//allow the robot to move forward, to reduce ultrasonic noise
		resetEncoders();
		walkStraight(lowSpd, comSpd);
		wait1Msec(450);
		completeStop(1000);

		turnRight(90, 60);	//turn to start pointe
		completeStop(1000);

		resetEncoders();
		while(SensorValue[frontUltra]>20){
			walkStraight(lowSpd, comSpd);//go to starte pointe
		}
		completeStop(1000);

		//turn to the room 2 hallway
		turnRight(90, 60);
		completeStop(1000);

		//hop a little
		resetEncoders();
		walkStraight(lowSpd, comSpd);
		wait1Msec(1000);
		completeStop(500);

		//usig right sensor to get to room2 gateway

		resetEncoders();
		while(SensorValue[rightUltra]<rightSpace){
			walkStraight(lowSpd, comSpd);// go to opening of the roooom dos
		}

		completeStop(1000);

		//hop a little to center in the room
		resetEncoders();
		walkStraight(lowSpd, comSpd);
		wait1Msec(200);
		completeStop(500);

		completeStop(1000);
		turnRight(90, 60);
		completeStop(1000);//turn to roooom dos

		while(SensorValue[frontUltra]>25){
			walkStraight(lowSpd, comSpd);//go into the room
		}
		completeStop(1000);
		turnRight(180, 60);
		completeStop(1000);//180 degree scan

		while(SensorValue[frontUltra]>25){
			walkStraight(lowSpd, comSpd);//go out of the room
		}
		completeStop(1000);
		turnRight(90, 60);//turn to final destination
		completeStop(1000);

		while(SensorValue[frontUltra]>25){
			walkStraight(lowSpd, comSpd);//go to final destination the room
		}

}
