#pragma config(Sensor, in1,    lineTg,         sensorLineFollower)
#pragma config(Sensor, in2,    lineT2,         sensorLineFollower)
#pragma config(Sensor, in3,    lineT3,         sensorLineFollower)
#pragma config(Sensor, dgtl1,  ground,         sensorTouch)
#pragma config(Sensor, dgtl2,  floor2,         sensorTouch)
#pragma config(Sensor, dgtl3,  floor3,         sensorTouch)
#pragma config(Motor,  port2,           Emotor,        tmotorVex393_MC29, openLoop)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

task main()
{
	while(1==1)
	{
		if(SensorValue(floor2)==1){
			motor[Emotor] = -30;
		}
	}


}
