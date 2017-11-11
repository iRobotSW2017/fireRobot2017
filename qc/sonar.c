#pragma config(Sensor, dgtl1,  sonarIn,        sensorSONAR_inch)
#pragma config(Motor,  port1,           leftmotor,     tmotorVex393_HBridge, openLoop, reversed)
#pragma config(Motor,  port10,          rightmotor,    tmotorVex393_HBridge, openLoop)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

task main()
{
	wait1Msec(1000);
	int x =  SensorValue(sonarIn);
	while(x >20 || x < -1){
		//writeDebugStream("SonarIn is %d", x);
		motor[leftmotor]=127;
		motor[rightmotor]=127;
		x =  SensorValue(sonarIn);
//		wait1Msec(2000);
	}
		motor[leftmotor]=0;
		motor[rightmotor]=0;
}
//At speed of 120, it hit wall. At speed of 30, it stopped because of sensing
//127 working fine with 20 inch stop
