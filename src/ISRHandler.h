//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                                                      //
//  Arduino ISR Handler                                                                                                                 //
//  Copyright (C) 2019-2024 Jens Dyekjær Madsen                                                                                         //
//                                                                                                                                      //
//  Filename: ISRHandler.h                                                                                                              //
//                                                                                                                                      //
//  Description:                                                                                                                        //
//  This Library does Implementation of Class-Instances interrupts easy. It allows multiple interrupts, but common intr() handler.      //                                                                  //
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


#ifndef ISRHANDLER_H
#define ISRHANDLER_H

#include <Arduino.h>
#include <util/atomic.h>                                                        // Required for 8/16-bit systems to ensure atomic operations

// USE:
//   class MyInterruptHandler : public ISRHandler<2,RISING, 3,FALLING> {        // intr() triggered on INT=2 RISING and INT=3 FALLING
//   protected:
//     void intr(uint8_t interruptNum) override {                               // Handle interrupts here, both INT2 and INT3 calls same intr()
//       -- code for interrupts here --
//     }
//   };
//
//   MyInterruptHandler handler;                                                // Interrupt handler
//
//   void setup() {
//     handler.begin();                                                         // Start handling interrupts on pins 2 rising and 3 falling
//   }

template <uint8_t... InterruptAndModePairs>                                     // Use template with interrupt and mode pairs for each interrupt
class ISRHandler {
protected:                                                                      // Function that will handle the specific interrupt for the instance
  virtual void intr(uint8_t interruptNum) = 0;                                  //   Pure virtual function, must be implemented by derived class

public:
  ~ISRHandler() {                                                               // Destructor ensures that interrupts are properly stopped and detached
    end();                                                                      //   uses end() to stop and detach all interrupts
  }

  // Begin interrupt handling by setting up the static handle pointer and attaching the ISR
  bool begin() {                                                                // Starts interrupt handling for all interrupts in the template parameter pack
    if (!intHandle) {                                                           // Only start if the handler is not already set
      ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
        // Use atomic block for 8/16-bit systems. It ensures that interrupts are fully disabled during
        // pointer assignment, providing safety in case previous detachInterrupt() wasn't called or failed.
        intHandle = this;  // Set the static pointer to the current instance
      }
      // Attach ISR for each interrupt and mode pair in the parameter pack
      attachInterrupts<InterruptAndModePairs...>();                             // Recursively attach interrupts to static ISR
      return true;                                                              // Return true if success
    }
    return false;                                                               // Return false if intHandle is already set
  }

  // Ending interrupt handling by detach all interrupts and resetting the handler to nullptr
  bool end() {                                                                  // Stops interrupt handling for all interrupts in the template parameter pack
    if (intHandle == this) {                                                    // Only stop if intHandle points to this instance
      ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
        // Use atomic block for 8/16-bit systems. If detachInterrupt() fails, this atomic block ensures
        // intHandle is safely set to nullptr, preventing any invalid interrupt calls. Since detachInterrupt()
        // doesn't return success/failure code, handling this carefully is important.
        intHandle = nullptr;  // Reset the static pointer
      }                                                                         // Detach ISR for each interrupt and mode pair in the parameter pack
      detachInterrupts<InterruptAndModePairs...>();                             // Recursively detach interrupts
      return true;                                                              // Return true if success
    }
    return false;                                                               // Return false if intHandle doesn't match this instance
  }

private:
  typedef ISRHandler* volatile IntHandleT;                                      // Define a static pointer type for interrupt handling
  static IntHandleT intHandle;                                                  // Static pointer to the current instance for interrupt handling

  // Helper function to attach interrupts recursively
  template <uint8_t InterruptNum, int Mode, uint8_t... Rest>
  void attachInterrupts() {
    attachInterrupt(                                                            // Attach ISR for the current interrupt and mode
      digitalPinToInterrupt(InterruptNum),                                      //   Attach current InterruptNum
      []() {                                                                    //   Lambda function as static interrupt service routine (ISR)
        if (intHandle) {                                                        //     Make sure instance handle exists
          intHandle->intr(InterruptNum);                                        //       Call the instance interrupt trough handler if exists
        }
      },
      Mode);                                                                    //   Attach Mode
    if constexpr (sizeof...(Rest) > 0) {                                        // Check if there are more interrupt and mode pairs in the parameter pack
      attachInterrupts<Rest...>();                                              // Recursively attach the remaining interrupts and modes
    }
  }

  // Helper function to detach interrupts recursively
  template <uint8_t InterruptNum, int Mode, uint8_t... Rest>
  void detachInterrupts() {
    detachInterrupt(digitalPinToInterrupt(InterruptNum));                       // Detach ISR for the current interrupt
    if constexpr (sizeof...(Rest) > 0) {                                        // Check if there are more interrupt and mode pairs in the parameter pack
      detachInterrupts<Rest...>();                                              // Recursively detach the remaining interrupts
    }
  }
};

// Initialize the static pointer to nullptr (must be in the header file)
template<uint8_t... InterruptAndModePairs>
typename ISRHandler<InterruptAndModePairs...>::IntHandleT ISRHandler<InterruptAndModePairs...>::intHandle = nullptr;

#endif  // ISRHANDLER_H
