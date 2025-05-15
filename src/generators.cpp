#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>

#include "circular_buffer.hpp"
#include "CORDIC.h"
#include "config.hpp"
#include "MCP_DAC.hpp"
#include "timers.hpp"

// Q15 format constants
#define Q15_SHIFT 15
#define Q15_MAX   0x7FFF
#define Q15_SCALE (1 << Q15_SHIFT)

#define ANGLE_SHIFT 16
#define ANGLE_MAX   (1 << ANGLE_SHIFT)

// REWRITE THIS. I DONT KNOW WHATS GOING ON
#define Q15_SHIFT 15
#define Q15_MAX 0x7FFF
#define Q15_SCALE (1 << Q15_SHIFT)
#define ANGLE_SHIFT 16
#define ANGLE_MAX (1 << ANGLE_SHIFT)
#define TWO_PI (int) (3.14159*Q15_SCALE) // ?? does that work?
#define SINGLE_PUNCH (int)(BUFFER_SIZE/TWO_PI + 0.5) // i dont have any better names for this.
#define INV_SINGLE_PUNCH (int) ((1/BUFFER_SIZE/TWO_PI) << ANGLE_SHIFT)

class WaveformGenerator
{
public:
    virtual ~WaveformGenerator() {}
    
    void start_output(circular_buf* buf, MCP_DAC dac) {
        int index = 0;
        uint16_t freq_width = 1000000/(getFrequency()*BUFFER_SIZE); // "us per sample"
        while(1)
        {
            dac.DAC_write(read_cbuf(buf,index));
            index = index & BUFFER_SIZE_MASK; // quick loop around
            index++;
            delay_us(freq_width); // works if freq << F_CPU (probably most safe under 50khz?)
        }
        // Initialize any hardware resources if needed
    }

    // to nab freq from child class
    virtual uint16_t getFrequency() const = 0;  
    
    virtual void fillBuffer(circular_buf& buf) = 0;
};

class SineGenerator : virtual public WaveformGenerator
{
public:
    SineGenerator(uint16_t freq, uint16_t amplitude = Q15_MAX)
        : WaveformGenerator(), _freq(freq), _amp(amplitude), _phase(0)
    {}
    
    void setFreq(uint16_t f) { _freq = f; }
    void setAmp(uint16_t a) { _amp = a; }
    
    void fillBuffer(circular_buf& buf) override
    {
        int buffersize = 16;
        //buffer = 1 period -> each angle is Buffersize/2pi 
        int yDirection = SINGLE_PUNCH;
        int dummy;
        int angle;
        for (int i = 0; i < BUFFER_SIZE; i++)
        {
            CORDIC_Rotating(&yDirection,&dummy,&angle);
            write_cbuf(&buf, yDirection);
            angle++;
        }
    }
     uint16_t getFrequency() const override
    {
        return _freq;
    }
    
private:
    uint16_t _freq;    // Frequency in Hz
    uint16_t _amp;     // Amplitude (Q15 format)
    uint32_t _phase;   // Current phase (fixed-point angle)
};

// Example usage:
void setup_sine_wave_generator() {
    // Create a sine wave generator at 440 Hz with 80% amplitude
    MCP_DAC dac(0);
    circular_buf buf;
    SineGenerator sineGen(440, (uint16_t)(Q15_MAX * 0.8));
    sineGen.start_output(&buf,dac);

}