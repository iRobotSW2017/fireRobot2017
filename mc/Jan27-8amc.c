#pragma config(I2C_Usage, I2C1, i2cSensors)
#pragma config(Sensor, dgtl3,  frontUltra,     sensorSONAR_cm)
#pragma config(Sensor, dgtl5,  leftUltra,      sensorSONAR_cm)
#pragma config(Sensor, dgtl7,  rightUltra,     sensorSONAR_cm)
#pragma config(Sensor, dgtl12, redLed,         sensorDigitalOut)
#pragma config(Sensor, I2C_1,  ,               sensorQuadEncoderOnI2CPort,    , AutoAssign )
#pragma config(Sensor, I2C_2,  ,               sensorQuadEncoderOnI2CPort,    , AutoAssign )
#pragma config(Motor,  port1,           rightMotor,    tmotorVex393_HBridge, encoderPort, I2C_1)
#pragma config(Motor,  port10,          leftMotor,     tmotorVex393_HBridge, reversed, encoderPort, I2C_2)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

void goStraight (int leftspeed, int rightspeed){
//this fuction walks straight and has a consistent speed
	motor[rightMotor] = rightspeed;
	motor[leftMotor] = leftspeed;
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

	int tickGoal = (70* degrees)/10;

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

	int tickGoal = (62* degrees)/10;

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

		int comSpd = 60;
		int leftcomSpd = 70;
		//int comAdjSpd = 31;
		//int delaySec = 450;
		//int frontSpace = 12; //(46-30)/2
		int rightSpace = 23;

		//wait to have a full stop
		wait1Msec(2000);

		//Right motor is better to use than fwd motor because of different interferences
		while(SensorValue[rightUltra]<rightSpace){
			goStraight(leftcomSpd, comSpd);	//speed is debateable
		}
		completeStop(1000); // have robot stop @ the middle of hallway

		//allow the robot to move a little more, position @ the center of intersection
		goStraight(leftcomSpd, comSpd);
		wait1Msec(500);
		completeStop(500);

		//make 90 turn, going to room#1 direction
		turnRight(90, 60);//turn to the room so we can almost enter.

		//allow the robot to move forward
		while(SensorValue[frontUltra]>10){
			goStraight(leftcomSpd, comSpd);
		}
		//move close to the wall, and stop, so the robot can stop on the middle of entry
			completeStop(1000);	//???

		turnRight(90, 60);//turn to the room so we can almost enter.
		completeStop(1000);

		//drive into the roiom
		while(SensorValue[frontUltra]>30){
			goStraight(leftcomSpd, comSpd);  //Walk straight
		}
		//stop close to 30cm
			completeStop(1000);


		//180 degree turn
		turnRight(180, 60);//turn to the room so we can almost enter.
		completeStop(1000);

		//leave the room
		while(SensorValue[frontUltra]>25){
			goStraight(leftcomSpd, comSpd);
		}
		if(SensorValue[frontUltra]<25){
			completeStop(1000);//exit the room
		}

		//make 90 left turn
		turnLeft(90, 60);//turn to the room so we can almost enter.
		completeStop(1000);

		goStraight(leftcomSpd, comSpd);// move to center of the hallways
		wait1Msec(1500);
		completeStop(1000);

		//Room 3

		turnRight(90,60);
		completeStop(1000);
		while(SensorValue[rightUltra]>20){
			goStraight(leftcomSpd, comSpd);
		}
		completeStop(1000);
		while(SensorValue[rightUltra]<20){
			goStraight(leftcomSpd, comSpd);
		}
		completeStop(1000);
		turnRight(90,60);
		completeStop(1000);
		while(SensorValue[frontUltra]>26){
			goStraight(leftcomSpd, comSpd);
		}
		completeStop(1000);
		turnRight(180,60);
		completeStop(1000);

		//Room 4

		while(SensorValue[frontUltra]>35){
			goStraight(leftcomSpd,comSpd);
		}
		completeStop(1000);
		turnLeft(180,60);
		completeStop(1000);
		turnRight(180,60);

		//Room 2




}