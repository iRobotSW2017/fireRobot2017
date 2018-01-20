void walkStraight(){
//while(SensorValue[ultrain]>10){
	if ((nMotorEncoder[rightMotor]) > (abs(nMotorEncoder[leftMotor]))){
		motor[rightMotor] = 40;
		motor[leftMotor] = 63;
	}

	if (nMotorEncoder[rightMotor] < abs(nMotorEncoder[leftMotor])){
		motor[rightMotor] = 63;
		motor[leftMotor] = 40;
	}
	if (nMotorEncoder[rightMotor] == abs(nMotorEncoder[leftMotor])){
		motor[rightMotor] = 63;
		motor[leftMotor] = 63;
	}
//}
}





task main()
{



}
