#pragma config(Sensor, dgtl1,  UltraIn,        sensorSONAR_inch)
#pragma config(Motor,  port1,           leftmotor,     tmotorVex393_HBridge, openLoop, reversed)
#pragma config(Motor,  port10,          rightmotor,    tmotorVex393_HBridge, openLoop)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

task main()
{

	//while	(SensorValue[UltraIn]>3)
		while	(true)
{
	motor[port1]=63;
	motor[port10]=63;
}
	motor[port1]=0;
	motor[port10]=0;
}