#pragma config(Motor,  port1,           leftMotor,     tmotorVex393_HBridge, openLoop, reversed)
#pragma config(Motor,  port10,          rightMotor,    tmotorVex393_HBridge, openLoop)

task main()
{
wait1Msec(2000);
motor(leftMotor)=0;       // Swing turn: Left motor is set to zero
motor(rightMotor)=60;
wait1Msec(1985);          // wait twice as much time as point turn (sometimes more)
motor(rightMotor)=0;
motor(leftMotor)=0;       // make sure to stop

}
