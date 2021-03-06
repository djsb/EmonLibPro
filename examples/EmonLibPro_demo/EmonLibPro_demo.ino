/*
    Energy monitor Library Pro
    Based on emonTx hardware from OpenEnergyMonitor http://openenergymonitor.org/emon/
    This is a generic library for any voltage and current sensors.
    Interrupt based, implements a zero cross detector and phase-locked loop for better precision.
    Completely line frequency independent.
    
    Copyright (C) 2013  Nuno Chaveiro  nchaveiro[at]gmail.com  Lisbon, Portugal

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
     any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
 
#include <Arduino.h>
#include "EmonLibPro.h"

/*  Energy monitor Library Pro 
    Based on emonTx hardware from OpenEnergyMonitor http://openenergymonitor.org/emon/
    
    This version is a generic library for any voltage and current sensors.
    Interrupt based, implements a zero cross detector and phase-locked loop for better precision.
    Completely line frequency independent.
    
    Reports Voltage, Frequency, Current, Active Power, Aparent Power and Power Factor.
    
    This library idea cames from ATMEL AVR465: Single-Phase Power/Energy Meter.
    
    This file is a demonstration usage.
    
    Edit configuration in EmonLibPro.h.

    Author: Nuno Chaveiro  nchaveiro[at]gmail.com  Lisbon, Portugal
    

*/

//--------------------------------------------------------------------------------------------------

byte i;
int userCommand;

void printCycle(byte i);
void printResults(byte i);
void printMenu();
void printStatus();

EmonLibPro  Emon ;

void setup()
{
  //Serial.begin(230400);
  Serial.begin(9600);
  Serial.println("Ready");

  Emon = EmonLibPro();    // Required.  
  Emon.begin();
  printMenu();
}

void loop()
{
  if(Emon.FlagCALC_READY && userCommand == 1) { 
      Emon.calculateResult();
      for (i=0;i<CURRENTCOUNT;i++){
        printResults(i);
      }
  }
  if(userCommand == 2) {
        printStatus();
        userCommand = 0;
  }  

  if(Serial.available())
  {
    userCommand=Serial.parseInt();
    Serial.print("Got: ");
    Serial.println(userCommand);
  }
  
}



void printMenu(){
    Serial.println("EmonLipPro Demo");
    Serial.println(" 1 - Print Calculated data. Change interval in define CALCULATESAMPLES.");
    Serial.println(" 2 - Print Lib Status.");
    Serial.println("Press a key...");
}

void printResults(byte i)
{
    Serial.print("Result");
    Serial.print(i);
    Serial.print(": ");
    Serial.print(Emon.ResultV[0].U);
    Serial.print("VAC\t");
    Serial.print(Emon.ResultV[0].HZ);
    Serial.print("Hz\t");
    Serial.print(Emon.ResultP[i].I);
    Serial.print("A\t");
    Serial.print(Emon.ResultP[i].P);
    Serial.print("W\t");
    Serial.print(Emon.ResultP[i].S);
    Serial.print("VA\t");
    Serial.print(Emon.ResultP[i].F);
    Serial.println("Pfact");
}

void printStatus()
{
  if (Emon.FlagOutOfTime) Serial.println("ERROR: Timer has overflown, reduce SAMPLESPSEC.");
  if(Emon.pllUnlocked) { 
      Serial.println("PLL: Unlocked");
  } else {
      Serial.println("PLL: Locked");
  } 
  Serial.print("ADC samples per Cycle: ");
  Serial.println((Emon.SamplesPerCycleTotal/Emon.CyclesPerTotal) * (VOLTSCOUNT + CURRENTCOUNT));
  Serial.print("VOLTSCOUNT: ");
  Serial.println(VOLTSCOUNT);
  Serial.print("CURRENTCOUNT: ");
  Serial.println(CURRENTCOUNT);
  Serial.print("SAMPLESPSEC: ");
  Serial.println(SAMPLESPSEC);
  Serial.print("CALCULATESAMPLES: ");
  Serial.println(CALCULATESAMPLES);
}
