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

	int tickGoal = (52* degrees)/10;

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

	int tickGoal = (60* degrees)/10;

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

		int comSpd = 63;
		int lowSpd = 40;
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

		//Right motor is better to use than fwd motor because of different interferences
		resetEncoders();
		while(SensorValue[rightUltra]<rightSpace){
			walkStraight(lowSpd, comSpd);	//speed is debateable
		}
		completeStop(1000); // have robot stop @ the middle of hallway


		//allow the robot to move a little more, position @ the center of intersection
		resetEncoders();
		//walkStraight(lowSpd, comSpd);
		//wait1Msec(500);
		completeStop(500);

		//make 90 turn, going to room#1 direction
		turnRight(90, 60);//turn to the room so we can almost enter.
		completeStop(1000);

		//allow the robot to move forward
		resetEncoders();
		walkStraight(lowSpd, comSpd);
		wait1Msec(1000);
		completeStop(500);

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

		//drive into the roiom
		resetEncoders();
		while(SensorValue[frontUltra]>28){
			walkStraight(lowSpd, comSpd);  //Walk straight
		}
		completeStop(1000);	//stop close to 30cm

		//180 degree turn
		turnRight(170, 60);//turn to the room so we can almost enter.
		completeStop(1000);


		//leave the room
		resetEncoders();
		while(SensorValue[frontUltra]>15){
				walkStraight(lowSpd, comSpd);
		}
		completeStop(1000);//exit the room


		//make 90 left turn
		turnLeft(90, 60);//turn to the room so we can almost enter.
		completeStop(1000);

		resetEncoders();
		walkStraight(lowSpd, comSpd);// move to center of the hallways
		wait1Msec(1500);

		completeStop(1000);

}
