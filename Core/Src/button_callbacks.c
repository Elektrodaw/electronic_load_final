/*
 * button_callbacks.c
 *
 *  Created on: Jan 6, 2026
 *      Author: ddaw7
 */

#include "button_callbacks.h"
#include "main.h"

void buttonCallbackSetNew(){
	previousMode = actualMode;
	actualMode = 7;
}
void buttonCallbackLoadSW(){
	if(loadStatus == 0){
		loadStatus = 1;
	}else{
		loadStatus = 0;
	}
}
void buttonCallbackVolatageSrc(){
	if(voltageSrcState == 0){
		voltageSrcState = 1;
	}else{
		voltageSrcState = 0;
	}
}
void buttonCallbackBack(){
	previousMode = actualMode;
	actualMode = 0;
}

void buttonCallbackCC(){
	previousMode = actualMode;
	actualMode = 1;

}
void buttonCallbackCV(){
	previousMode = actualMode;
	actualMode = 2;
}
void buttonCallbackCR(){
	previousMode = actualMode;
	actualMode = 3;
}
void buttonCallbackCP(){
	previousMode = actualMode;
	actualMode = 4;
}

void buttonCallbackPV(){
	previousMode = actualMode;
	actualMode = 9;
}

void buttonCallbackSave(){
	SetValueGlobal = NewValue;

	int modeMemory = 0;
	modeMemory = actualMode;
	actualMode = previousMode;
	previousMode = modeMemory;
}

void buttonCallbackPlus10(){
	if(NewValue < 90.00){
		NewValue = NewValue + 10.00;
	}
}
void buttonCallbackPlus1(){
	if(NewValue<99.00){
		NewValue = NewValue + 1.0;
	}
}
void buttonCallbackPlus01(){
	if(NewValue < 99.90){
		NewValue = NewValue + 0.1;
	}
}
void buttonCallbackPlus001(){
	if(NewValue < 99.99){
		NewValue = NewValue + 0.01;
	}
}
void buttonCallbackMinus10(){
	if(NewValue > 9.99){
		NewValue = NewValue - 10.00;
	}
}
void buttonCallbackMinsu1(){
	if(NewValue > 0.99)
	NewValue = NewValue - 1.00;
}
void buttonCallbackMinus01(){
	if(NewValue > 0.09){
		NewValue = NewValue - 0.10;
	}
}
void buttonCallbackMinus001(){
	if(NewValue > 0){
		NewValue = NewValue - 0.01;
	}
}

void buttonCallbackPVstart(){
	PVtestStart = 1;
}
