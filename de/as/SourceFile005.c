#pragma config(Sensor, dgtl3,  sonarin,        sensorSONAR_cm)
#pragma config(Sensor, dgtl5,  sonarleft,      sensorSONAR_cm)
#pragma config(Sensor, dgtl7,  sonarright,     sensorSONAR_cm)
#pragma config(Motor,  port1,           rightmotor,    tmotorVex393_HBridge, openLoop)
#pragma config(Motor,  port10,          leftmotor,     tmotorVex393_HBridge, openLoop, reversed)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

void Pointturn(int leftSpeed, int rightSpeed, int waittime){
	motor[rightmotor]=rightSpeed;
	motor[leftmotor]=leftSpeed;
	wait1Msec(waittime);
	//* easy turns
}

void Straight(int leftval, int rightval){
	motor[rightmotor] = rightval;
	motor[leftmotor] = leftval;
	//* make rightval=63, and leftval=73
	//* To clean up the code
}

void Waitvoid (){
	motor[leftmotor] = 0;
	motor[rightmotor] = 0;
	wait1Msec(1000);
	//* give the robot a rest
}

task main(){
//when the right sensor is more than 20, go straight.
	wait1Msec(1000);
while(SensorValue[sonarright]<20){
 Straight(80, 70);
	}
//when it is less than 20, turn right.
Waitvoid();
Pointturn(120, -120, 415);
Waitvoid();
//when it is more than 20, go straight.
while(SensorValue[sonarin]>25){
	Straight(63,53);
}
Waitvoid();
//when left sensor is more than 20, turn left.
Pointturn(120,-120,412);
Waitvoid();
//while it is less than 20, go straight.
while(SensorValue[sonarin]>30){
	Straight(63, 53);
}
Waitvoid();
Pointturn(60, -60, 1750);
Waitvoid();
}