#pragma config(Sensor, dgtl3,  ultrain,        sensorSONAR_cm)
#pragma config(Motor,  port1,           rightMotor,    tmotorVex393_HBridge, openLoop, encoderPort, None)
#pragma config(Motor,  port10,          leftMotor,     tmotorVex393_HBridge, openLoop, reversed, encoderPort, None)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

task main()
{
wait1Msec(1000);
while(SensorValue(ultrain)>10){
	motor(rightMotor)= 40;
	motor(leftMotor)= 50;
}
while(SensorValue(ultrain)<10){
	motor(rightMotor)= 0;
	motor(leftMotor)= 0;
}
}
