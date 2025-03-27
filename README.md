# PEDDLE
*for when you just want to smuggle and distribute some sweet tunes*

## **TO Build:** 
```make```
```make flash```


(ensure:)
brew tap osx-cross/avr
brew install avr-gcc avrdude

## Current functionality:
- Static 2-pole filter
- UART Tx/Rx
- PWM (*'Phase and Frequency Correct' mode*)

## Current TODO:
- [ ] Shorten filter execution time
- [ ] Add second ADC input for envelope follower (Auto Wah)
- [ ] Add 2-3 IIR filters
- [ ] Add FIR functionality
- [ ] SPI for *LCD interfacing*
- [ ] Add "Makefile" for project

Using an ATMEGA 328, 
  this project is targetted at 300 series ATMEL MCUs.
