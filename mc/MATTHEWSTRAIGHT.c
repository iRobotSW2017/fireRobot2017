#pragma config(Sensor, dgtl3,  ultrain,        sensorSONAR_cm)
#pragma config(Motor,  port1,           RightMotor,    tmotorVex393_HBridge, openLoop)
#pragma config(Motor,  port10,          LeftMotor,     tmotorVex393_HBridge, openLoop, reversed)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

task main()
{
{
while (SensorValue(ultrain)>25)
	motor(LeftMotor)=60;
	motor(RightMotor)=45;
}
{
while (SensorValue(ultrain)<25)
	motor(LeftMotor)=0;
	motor(RightMotor)=0;
	wait1Msec(2000);
	motor(LeftMotor)=60;
	motor(RightMotor)=-45;
	wait1Msec(980);
}
}
