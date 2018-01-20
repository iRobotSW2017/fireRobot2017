#pragma config(Sensor, dgtl3,  fwdultrain,     sensorSONAR_cm)
#pragma config(Sensor, dgtl7,  Rultrain,      sensorSONAR_cm)
#pragma config(Motor,  port1,           RightMotor,    tmotorVex393_HBridge, openLoop)
#pragma config(Motor,  port10,          LeftMotor,     tmotorVex393_HBridge, openLoop, reversed)
//*!!Code automatically generated by 'ROBOTC' configuration wizard
void goStraight (int leftspeed, int rightspeed ){
//this fuction walks straight and has a consistent speed
	motor(RightMotor)=rightspeed;
	motor(LeftMotor)=leftspeed;
}

void completeStop (int delaytime  ){
//this stops program for 1 second
	motor(RightMotor)=0;
	motor(LeftMotor)=0;
	wait1Msec(delaytime);
}
void pointTurn (int leftspeed, int rightspeed, int delaytime){
motor(RightMotor)=rightspeed;
	motor(LeftMotor)=leftspeed;//function to turn
	wait1Msec(delaytime);
}

task main()
{
while(SensorValue[Rultrain]<20){//Right motor is better to use than fwd motor because of different interferences
	goStraight(60,60);
}
completeStop(1000);
goStraight(60,60);//to move more forward so the bot doesnt hit wall
wait1Msec(300);
completeStop(500);
pointTurn(60,-60,900); //turn to the room so we can almost enter.
goStraight(60,60);
wait1Msec(500);

while(SensorValue[fwdultrain]>25){
	goStraight(60,60);
}
while(SensorValue[fwdultrain]<25){  //when there is no space ahead
	completeStop(1000);
	pointTurn(60,-60,920);//turn into the room
}
completeStop(1000);
while(SensorValue[fwdultrain]>30){
	goStraight(60,60);  //Walk straight
}
while(SensorValue[fwdultrain]<35){
	completeStop(1000);
	pointTurn(60,-60,1750);//180 degree turn
	completeStop(1000);
}
while(SensorValue[fwdultrain]>25){
	goStraight(60,60);
}
while(SensorValue[fwdultrain]<25){
	completeStop(1000);//exit the room
	pointTurn(-60,60,920);
}
completeStop(1000);
goStraight(60,60);// move to center of the hallways
wait1Msec(1500);

//ANNIHALATE EXODIA OBLITERATE!!!! BOIBOIBOI!!!!! DIDDLY DARN IT, I MISSED!!!! CHARGE UP THE DEATHSTAR!!!!!
//DESTROY ALDERAAN!!!! DIIIIIIEEEEE. AVADA KEDAVRA!!! MASONISWEARTOGAWD
//SOMEBODY TOUCHA MY SPAGHETT!!!
//WHYDIDN'TYOUSHOWUPFORELFPRACTICE?!?!?!?!?!?
//DO YOU KNOW DE WAY
}
