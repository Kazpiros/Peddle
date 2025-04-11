# PEDDLE
*for when you just want to smuggle and distribute some sweet tunes*

## Requirements:

**ON MAC**

```$ brew tap osx-cross/avr```

```$ brew install avr-gcc avrdude```

--- 
**ON DEBIAN**

```$ brew tap osx-cross/avr```

```$ brew install avr-gcc avrdude```

## To Build: 
Uses:

  ```make```            *Compile Workspace*

  ```make clean```      *Cleans Workspace*
  
  ```make flash```      *Flashes MCU*
   
  ```make lisp```       ***L**oad **ISP***  
  
## Current Active Pins:
![image](https://www.allaboutcircuits.com/uploads/articles/Atmega328_Arduino_Pinout.png)

*Courtesy of allaboutcircuits.com*


## Current TODO:
- [ ] Add Bluetooth
- [ ] Add USB interfacing
- [ ] Improve USART Accuracy
- [ ] Finish Port Mapping
- [ ] Add Equilizer
- [ ] External ADC & MUX Support
- [ ] I2C
- [ ] FFT & Volume Swinger
- [ ] Distortion Effects

Using an ATMEGA 328, 
  this project is targetted at 300 series ATMEL MCUs.