#pragma config(Motor,  port1,           leftMotor,     tmotorVex393_HBridge, openLoop, reversed)
#pragma config(Motor,  port10,          rightMotor,    tmotorVex393_HBridge, openLoop)

task main()
{
wait1Msec(2000);
motor(rightMotor)=30;
motor(leftMotor)=0;
wait1Msec(7500);
motor(rightMotor)=0;
motor(leftMotor)=0;
}
