- HOMEWORK: Explain whole Bitboy robot circuits to Mrs. Chen. Explain Relays. 
- In how2Flame4.c, the first flame sensor usage is after the comment that says "Updated by Qian"
- Maybe we should use return Algorithms for every room just in case of the candle being there
- right4flame integer is basically setting the degees and speed for the turn to find the spike in sensor value.
- 		int _ticks1 = right4flame(180, 60);
		completeStop(1000);
		if(isFlameDetected){
		turnLeft(180, 60); //back to start point
		completeStop(1000);

		int _ticks3 = turnRight(((_ticks1*10/rightTicks) - flameTargetAdj), 60, 0);
		SensorValue[redLed] = 0; // turn on LED
		putOffFlame(); // put off flame
		// how to finish the rest of turn
		turnRight(180, 60, _ticks3);
		}
This is the algorithm for finding the spike and putting out the candle