#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>

#include "circular_buffer.hpp"
#include "CORDIC.h"
#include "config.hpp"
// Q15 format constants
#define Q15_SHIFT 15
#define Q15_MAX   0x7FFF
#define Q15_SCALE (1 << Q15_SHIFT)

#define ANGLE_SHIFT 16
#define ANGLE_MAX   (1 << ANGLE_SHIFT)

class WaveformGenerator
{
    public:
    virtual ~WaveformGenerator()
    {
    }

    //haha what
    virtual void fillBuffer(circular_buf& buf, uint16_t sampleRate) = 0;
};

class SineGenerator : public WaveformGenerator
{
    public:
    SineGenerator(uint16_t freq, uint16_t amplitude = Q15_MAX)
    : _freq(freq), _amp(amplitude), _phase(0) // random number ionno
    {}
    void setFreq(uint16_t f) { _freq = f;}
    void setAmp(uint16_t A) { _amp = A;}
    void fillBuffer(circular_buf& buf, uint16_t sampleRate) override
    {
        cbuf_init(&buf);
        const uint32_t phase_inc = ((uint32_t)_freq * ANGLE_MAX) / sampleRate;
        for(int i = 0; i < BUFFER_SIZE; i++)
        {
            // Get current angle from phase accumulator (upper 16 bits)
            int angle = _phase >> (32 - ANGLE_SHIFT);
            CORDIC_Rotating(&x,&y,&angle);
            int32_t scaled = (int32_t) y * _amp;
            int16_t output = (scaled + (1 << 14)) >> 15;
            write_cbuf(&buf, output);
            _phase += phase_inc;
        }
    }
    private:
    uint16_t _freq, _amp, _phase;
    int x, y; // for my cordic program
};
// in rad
class SweepGenerator : public WaveformGenerator 
{
    public:
    SweepGenerator(uint16_t start_freq, uint16_t stop_freq, uint32_t sweep_time_s, uint16_t amplitude = Q15_MAX)
        : _start_freq(start_freq),
          _stop_freq(stop_freq),
          _sweep_time_s(sweep_time_s),
          _amp(amplitude),
          _phase_acc(0),
          _current_sample_rate(0),
          _sweep_samples_remaining(0) {}
    
    void fillBuffer(circular_buf& buf, uint16_t sampleRate) override 
    {
        if(_current_sample_rate != sampleRate) 
        {
            initialize_sweep(sampleRate);
        }
        const uint32_t phase_inc_max = ((uint32_t)_stop_freq << ANGLE_SHIFT) / sampleRate;
        const uint32_t phase_inc_min = ((uint32_t)_start_freq << ANGLE_SHIFT) / sampleRate;
        const int32_t phase_inc_delta = (phase_inc_max - phase_inc_min) / _sweep_samples_total;

        for(uint32_t i = 0; i < BUFFER_SIZE; i++) 
        {
                uint32_t phase_inc = phase_inc_min + 
                    (phase_inc_delta * (_sweep_samples_total - _sweep_samples_remaining));
    
                // Generate sample
                int angle = _phase_acc >> (32 - ANGLE_SHIFT);
                int x = A_, y = 0;
                CORDIC_Rotating(&x, &y, &angle);
                
                // Write to buffer
                int32_t scaled = (int32_t)y * _amp;
                write_cbuf(&buf, (scaled + (1 << 14)) >> 15);
    
                // Update phase
                _phase_acc += phase_inc;
                _sweep_samples_remaining--;
            }
    }
    
    private:
    void initialize_sweep(uint16_t sampleRate) 
    {
        _current_sample_rate = sampleRate;
        _sweep_samples_total = _sweep_time_s * sampleRate;
        _sweep_samples_remaining = _sweep_samples_total;
        _phase_acc = 0;  // Reset phase on sample rate change
    }

    // Member variables
    uint16_t _start_freq, _stop_freq;
    uint32_t _sweep_time_s;
    int16_t _amp;
    uint32_t _phase_acc;
    uint32_t _sweep_samples_total;
    uint32_t _sweep_samples_remaining;
    uint16_t _current_sample_rate;
    };

class SawGenerator : public WaveformGenerator
{
    //TODO
};

class SquareGenerator : public WaveformGenerator
{
    //TODO
};

class DCGenerator : public WaveformGenerator
{
    //TODO
};

