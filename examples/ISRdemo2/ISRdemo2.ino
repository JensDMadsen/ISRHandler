//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                                                      //
//  Arduino ISR Handler                                                                                                                 //
//  Copyright (C) 2019-2024 Jens Dyekjær Madsen                                                                                         //
//                                                                                                                                      //
//  Filename: ISRdemo2.ino                                                                                                              //
//                                                                                                                                      //
//  Description:                                                                                                                        //
//  Demo example showing how to implement Class-Instances interrupts using ISRHandler.h Library                                         //
//                                                                                                                                      //
//  This library is free software; you can redistribute it and/or modify it under the terms of the GNU Lesser General Public License    //
//  as published by the Free Software Foundation; either version 2.1 of the License, or (at your option) any later version.             //
//                                                                                                                                      //
//  This library is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of      //
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public License for more details.                   //
//  You should have received a copy of the GNU Lesser General Public License along with this library; if not, write to the              //
//  Free Software Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA                                            //
//                                                                                                                                      //
//  It is not allowed to change any license or copyright statements, but feel free to modify, change, and add your own copyrights       //
//  below this line only !                                                                                                              //
//  ----------------------------------------------------------------------------------------------------------------------------------  //
//                                                                                                                                      //
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



#include "ISRHandler.h"                                                         // Include headers

auto& Stdout = Serial;                                                          // Uses Serial as Stdout

class MyInterruptHandler : public ISRHandler<2,RISING, 3,FALLING> {             // intr() triggered on INT=2 RISING and INT=3 FALLING
protected:
  void intr(uint8_t interruptNum) override {                                    // Handle interrupts here, both INT2 and INT3 calls same intr()
    Stdout.print("Interrupt "); Stdout.print(interruptNum); Stdout.println(" triggered!");
  }
};

MyInterruptHandler handler;                                                     // Interrupt handler

void setup() {
  Stdout.begin(115200);                                                         // Setup 115200 baud
  while (!Serial);                                                              // Wait on Serial
  Stdout.println("Interrupt handler test");                                     // Check Serial print ok

  // Set the pins as input with pull-up resistors
  pinMode(2, INPUT_PULLUP);                                                     // Set pin 2 as input with pull-up
  pinMode(3, INPUT_PULLUP);                                                     // Set pin 3 as input with pull-up

  handler.begin();                                                              // Start handling interrupts on pins 2 and 3
}

void loop() {
                                                                                // Main code logic
}