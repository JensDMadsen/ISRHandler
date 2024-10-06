# ISRHandler Library Documentation

## Introduction

**ISRHandler** is a flexible and reusable C++ library for handling interrupts in Arduino-based projects. It simplifies managing interrupts by encapsulating them in a class structure and using templates to handle multiple interrupt pins efficiently.

This library is designed for embedded systems where precise and efficient interrupt handling is essential.

## Key Features

-   **Static ISR**: A lambda-based static interrupt service routine (ISR) that can be attached to one or more interrupt pins.
-   **Template Support**: Manage multiple interrupts across different pins by specifying them through templates.
-   **Atomic Operations**: Ensures safe assignment of the interrupt handler, especially on 8/16-bit systems.
-   **Multiple Interrupts**: Supports a single handler for multiple interrupts with the ability to check which interrupt triggered the handler.

## Installation

1.  Download or clone the library into your Arduino `libraries` folder.
2.  Open the Arduino IDE, and the library will appear in the **Sketch > Include Library** menu.
3.  Include the library in your sketch using `#include <ISRHandler.h>`.

## ISRHandler Class

The **ISRHandler** class allows handling multiple interrupt pins using a single `intr(uint8_t interruptNum)` function. You can attach multiple interrupts by specifying their pin numbers and modes (e.g., `RISING`, `FALLING`) through template parameters.

### Example Code

```cpp
#include "ISRHandler.h"

// intr() triggered on INT=2 RISING and INT=3 FALLING
class MyInterruptHandler : public ISRHandler<2, RISING, 3, FALLING> {             
protected:
  void intr(uint8_t interruptNum) override { 
    // Handle interrupts here, INT2 and INT3 both trigger this handler 
    Serial.print("Interrupt ");
    Serial.print(interruptNum); 
    Serial.println(" triggered!");
  }
};

MyInterruptHandler handler;

void setup() {
  Serial.begin(115200);
  while (!Serial);  // Wait for Serial to initialize
  Serial.println("ISRHandler test");

  // Set the pins as input with pull-up resistors
  pinMode(2, INPUT_PULLUP);  // Set pin 2 as input with pull-up
  pinMode(3, INPUT_PULLUP);  // Set pin 3 as input with pull-up

  handler.begin();  // Start handling interrupts on pins 2 and 3
}

void loop() {
  // Main code logic
} 
```


### Explanation of Example

1.  **ISRHandler Class**: The class `MyInterruptHandler` inherits from `ISRHandler`, specifying two interrupts (pin 2 and pin 3) with their respective modes (`RISING` for pin 2 and `FALLING` for pin 3).
2.  **intr(uint8_t interruptNum)**: This function is triggered whenever one of the specified interrupts occurs. It prints which interrupt triggered the handler by passing the interrupt pin number.
3.  **begin()**: This function starts the interrupt handling for the pins defined in the template parameters.
4.  **end()**: This function stops interrupt handling by detaching the ISR and resetting the handler.

## Detailed Usage

### Template Parameters

The **ISRHandler** class uses template parameters to configure interrupt modes and pin numbers:

-   `ISRHandler<pin1, mode1, pin2, mode2, ...>`
    -   **pinX**: The pin number for each interrupt.
    -   **modeX**: The interrupt mode for each pin (`RISING`, `FALLING`, `CHANGE`, etc.).

### Functions

-   **`begin()`**: Initializes the interrupt handler by attaching the ISR to the specified interrupt pins.
-   **`end()`**: Stops handling interrupts by detaching the ISR and resetting the handler pointer to `nullptr`.

### Using Multiple Interrupts

When using multiple interrupts, the **ISRHandler** provides the interrupt number to the `intr()` function. You can handle different interrupt sources within a single `intr()` function by checking the interrupt number.

```cpp
void intr(uint8_t interruptNum) override {
  if (interruptNum == 2) {
    // Handle interrupt from pin 2
  } else if (interruptNum == 3) {
    // Handle interrupt from pin 3
  }
}
``` 

## Error Handling and Edge Cases

### Common Issues

-   **Interrupts not working**: Ensure that `begin()` is called after setting up the pins, and that interrupts are enabled on the pins you want to use.
-   **Interrupt handling stops**: Verify that the pins are properly configured (e.g., use `INPUT_PULLUP` if needed).
-   **Interrupts trigger unexpectedly**: For noisy environments or bouncing signals, consider adding external pull-up/pull-down resistors or debouncing the signal in software.

### Tips

-   **Pin Configuration**: Always ensure that the interrupt pins are configured as `INPUT` or `INPUT_PULLUP` before calling `begin()`.
-   **Handling Spikes**: In noisy environments, use debouncing techniques or hardware filters to avoid false triggers.

## FAQ

**Q: Can I handle multiple interrupts with a single handler?**  
Yes, **ISRHandler** allows you to manage multiple interrupts with a single `intr()` function by checking the interrupt number.

**Q: How do I debounce an interrupt signal?**  
For mechanical buttons or switches, you can debounce the signal in software by ignoring rapid successive triggers, or use a hardware debouncing circuit.

**Q: Can I use **ISRHandler** with multiple classes?**  
Yes, you can instantiate different handler objects in multiple classes, each managing its own set of interrupts.


## Learn More

For a detailed explanation of the static pointer method used in the ISRHandler library, read the article: [Interrupts in C++ Using a Static Pointer Method](https://arduino.one/ISRHandler/StaticPointerInterrupt.html).


## Conclusion

The **ISRHandler** library simplifies interrupt handling across multiple pins in Arduino projects. By encapsulating interrupt logic in a class and using templates to manage multiple interrupt pins, it provides flexibility and ease of use for developers working with embedded systems.

The library is ideal for applications requiring multiple interrupt sources, and the template system allows efficient and clean code for managing these interrupts.

For more examples and detailed documentation, see the `examples` folder in the library repository.


## Contact

If you have any questions or would like to reach out for any reason, feel free to [Contact Me](https://www.arduino.one/contact.html) through my online form.

Thank you for visiting this repository, and I hope you find the tools and information useful!