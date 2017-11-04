#pragma config(Motor,  port1,           leftmotor,     tmotorVex393_HBridge, openLoop, reversed)
#pragma config(Motor,  port10,          rightmotor,    tmotorVex393_HBridge, openLoop)
task main()
{
	motor[leftmotor]=127;
	motor[rightmotor]=127;
	wait1Msec(1500);
	motor[leftmotor]=-127;
	wait1Msec(2000);
	motor[leftmotor]=127;
	wait1Msec(1000);
	motor[leftmotor]=0;
	motor[rightmotor]=0;


}
