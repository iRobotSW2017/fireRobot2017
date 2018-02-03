#pragma config(I2C_Usage, I2C1, i2cSensors)
#pragma config(Sensor, dgtl3,  frontUltra,     sensorSONAR_cm)
#pragma config(Sensor, dgtl5,  leftUltra,      sensorSONAR_cm)
#pragma config(Sensor, dgtl7,  rightUltra,     sensorSONAR_cm)
#pragma config(Sensor, dgtl9,  Bumpey,         sensorTouch)
#pragma config(Sensor, dgtl11, fan,            sensorDigitalOut)
#pragma config(Sensor, dgtl12, redLed,         sensorDigitalOut)
#pragma config(Sensor, I2C_1,  ,               sensorQuadEncoderOnI2CPort,    , AutoAssign )
#pragma config(Sensor, I2C_2,  ,               sensorQuadEncoderOnI2CPort,    , AutoAssign )
#pragma config(Motor,  port1,           rightMotor,    tmotorVex393_HBridge, openLoop, encoderPort, I2C_1)
#pragma config(Motor,  port6,           fanMotor,      tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port10,          leftMotor,     tmotorVex393_HBridge, openLoop, reversed, encoderPort, I2C_2)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

task main()
{
	bool isLEDon = false;
	SensorValue[redLed] = 1; //make sure RED-LED is off
	SensorValue[fan] = 0; //make sure RED-LED is off
	motor[fanMotor]=50;

	/*while(true){
		if(SensorValue[Bumpey]==1){
				if(isLEDon){
					SensorValue[redLed] = 1; //make sure RED-LED is on
					SensorValue[fan] = 1; //make sure RED-LED is on
				}else{
					SensorValue[redLed] = 0;
					SensorValue[fan] = 0; //make sure RED-LED is on
				}
				isLEDon = !isLEDon;


		}
	}*/


}