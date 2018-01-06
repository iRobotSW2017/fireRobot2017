#pragma config(Sensor, dgtl3,  ultrain,        sensorSONAR_cm)
#pragma config(Sensor, dgtl7,  RightUltra,     sensorSONAR_cm)
#pragma config(Motor,  port1,           rightmotor,    tmotorVex393_HBridge, openLoop)
#pragma config(Motor,  port10,          leftmotor,     tmotorVex393_HBridge, openLoop, reversed)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

void Pointturn(int leftSpeed, int rightSpeed, int waittime){
	motor[rightmotor]=rightSpeed;
	motor[leftmotor]=leftSpeed;
	wait1Msec(waittime);
}

task main()
{
wait1Msec(1000);
while (SensorValue[ultrain] > 18 && SensorValue[RightUltra] <= 15){
	motor[rightmotor] = 63;
	motor[leftmotor] = 73;
	//* go to the conjunction
}
motor[leftmotor] = 0;
motor[rightmotor] = 0;
wait1Msec(1000);
Pointturn(120, -120, 475);
//* turn
motor[rightmotor] = 0;
motor[leftmotor] = 0;
wait1Msec(1000);
while (SensorValue[ultrain]>25){
	motor[rightmotor] = 53;
	motor[leftmotor] = 63;
}
//* go to the wall so you can turn into the room
motor[rightmotor] = 0;
motor[leftmotor] = 0;
wait1Msec(1000);
Pointturn (120, -120, 475);
//* turn into the room
motor[rightmotor] = 0;
motor[leftmotor] = 0;
wait1Msec(1000);
while (SensorValue[ultrain] > 15){
	motor[rightmotor] = 53;
	motor[leftmotor] = 63;
}
//* enter the room
motor(rightmotor) = 0;
motor(leftmotor) = 0;
}
