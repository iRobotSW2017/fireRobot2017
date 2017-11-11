#pragma config(Sensor, dgtl1,  sonarIn,        sensorSONAR_inch)
#pragma config(Motor,  port1,           leftmotor,     tmotorVex393_HBridge, openLoop, reversed)
#pragma config(Motor,  port10,          rightmotor,    tmotorVex393_HBridge, openLoop)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

task main()
{
	wait1Msec(1000);
	int x =  SensorValue(sonarIn);
	while(x > 10 || x < -1){
		//writeDebugStream("SonarIn is %d", x);
		motor[leftmotor]=122;
		motor[rightmotor]=127;
		x =  SensorValue(sonarIn);
//		wait1Msec(2000);
	}
		motor[leftmotor]=0;
		motor[rightmotor]=0;
		wait1Msec(2000);
    // right motor stronger than left motor
		motor[rightmotor]=-120;
		motor[leftmotor]=115;
		wait1Msec(460);
		motor[rightmotor]=0;
		motor[leftmotor]=0;
	  wait1Msec(1000);
	  x =  SensorValue(sonarIn);
	while(x >15 || x < -1){
		//writeDebugStream("SonarIn is %d", x);
		motor[leftmotor]=122;
		motor[rightmotor]=127;
		x =  SensorValue(sonarIn);
//		wait1Msec(2000);
	}
		motor[rightmotor]=0;
		motor[leftmotor]=0;
		}
