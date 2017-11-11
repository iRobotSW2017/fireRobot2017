#pragma config(Motor,  port1,           leftMotor,     tmotorVex393_HBridge, openLoop, reversed)
#pragma config(Motor,  port10,          rightMotor,    tmotorVex393_HBridge, openLoop)
task main()
{
wait1Msec(2000);
motor(leftMotor)=-60;
motor(rightMotor)=60;
wait1Msec(980);
motor(rightMotor)=0;
motor(leftMotor)=0;

}
