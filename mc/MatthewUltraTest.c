#pragma config(Sensor, dgtl3,  fwdultrain,     sensorSONAR_cm)
#pragma config(Sensor, dgtl7,  Rultrain,      sensorSONAR_cm)
#pragma config(Motor,  port1,           RightMotor,    tmotorVex393_HBridge, openLoop)
#pragma config(Motor,  port10,          LeftMotor,     tmotorVex393_HBridge, openLoop, reversed)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

task main()
{
while(SensorValue[fwdultrain] > 36){
	motor(LeftMotor)=50;
	motor(RightMotor)=50;
}
while(SensorValue[fwdultrain] < 36){
	motor(RightMotor)=0;
	motor(LeftMotor)=0;
	wait1Msec(2000);
	while(SensorValue[Rultrain] > 4){
		motor(LeftMotor)=120;
		motor(RightMotor)=-120;
		wait1Msec(1000);
	}
	while(SensorValue[Rultrain] < 4){
		motor(LeftMotor)=0;
		motor(RightMotor)=0;
	}
}
	while(SensorValue[fwdultrain] > 38){
		motor(LeftMotor)=60;
		motor(RightMotor)=60;
	}
	while(SensorValue[fwdultrain] < 38){
		motor(LeftMotor)=0;
		motor(RightMotor)=0;
		wait1Msec(2000);
		while(SensorValue[Rultrain] > 4){
			motor(RightMotor)=-120;
			motor(LeftMotor)=120;
			wait1Msec(1000);
		}
		while(SensorValue[fwdultrain] > 78){
			motor(LeftMotor)=60;
			motor(RightMotor)=60;
		}
		while(SensorValue[fwdultrain] < 78){
			motor(LeftMotor)=0;
			motor(RightMotor)=0;
			while(SensorValue[Rultrain] > 4){
				motor(RightMotor)=-120;
				motor(LeftMotor)=120;
				wait1Msec(1000);
			}
		}
		while(SensorValue[fwdultrain] > 8){
			motor(LeftMotor)=60;
			motor(RightMotor)=60;
		}
		while(SensorValue[fwdultrain] < 8){
			motor(LeftMotor)=0;
			motor(RightMotor)=0;
		}
		//ANNIHALATE. EXODIA, OBLITERATE!!!!!!!!! DIESTUPIDKOALASCUMBAGNAMEDRILEYRENDLOW
	}
}