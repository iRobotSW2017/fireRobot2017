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

void resetEncoders(){
	nMotorEncoder[rightMotor]=0;
	nMotorEncoder[leftMotor]=0;
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

		/*
		SensorValue[redLed] = 0;
			motor[leftMotor]=50;
			motor[rightMotor]=50;
		wait1Msec(2000);
		SensorValue[redLed] = 1;
			motor[leftMotor]=0;
			motor[rightMotor]=0;

			//2sec 50speed 61.5cm
		*/

		//360 is how far
			resetEncoders();

			int tickGoal = 559; //588  627==36~38

			motor[rightMotor]=50;
			motor[leftMotor]=50;

			while(nMotorEncoder[rightMotor]<tickGoal){

			}

			motor[rightMotor]=0;
			motor[leftMotor]=0;

}
