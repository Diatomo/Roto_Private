
#include <Arduino.h>
#include <outputExtend.h>
#include "cRelay.h"


CRelay::CRelay(){

		//pins oeRelay
		uint8_t const oeData = 7;
		uint8_t const oeClk = 6;
		uint8_t const oeLtch = 5;
		uint8_t const nOE = 1;
		
		
		action = false;
		currDevice = -1;
		resetTimer = 0;
		audio = 5;
		optoCopter = 6;

		oe = new outputExtend(oeData, oeClk, oeLtch, nOE);
}

void CRelay::update(uint8_t action){
		Serial.println("relay ACTION!!");
		uint8_t command = (action & 0x10) >> 4; //either a 0 or a 1;
		uint8_t button = action & 0x0F; //represents an output pin
		uint8_t deviceMap[5] = {1,0,3,2,4};
		button = deviceMap[button];
		test(command, button);
		if (button != optoCopter || button != audio){
			currDevice = button;
		}
		resetTimer = millis();
		oe->extendedWrite(button, command);
		oe->extendedWrite(audio, HIGH);
		delay(50);
		oe->extendedWrite(audio,LOW);
}

void CRelay::loop(){
	uint32_t currTimer = millis();
	Serial.println("loop");
	if (resetTimer != 0){
		if ((currTimer - resetTimer) > resetTime){
			Serial.println(" RESET ");
			oe->extendedWrite(currDevice, LOW);
			currDevice = -1;
			resetTimer = 0;
		}
	}
}

void CRelay::test(uint8_t command, uint8_t button){
	Serial.print("command :: ");
	Serial.println(command);
	Serial.print("Button :: ");
	Serial.println(button);

}
