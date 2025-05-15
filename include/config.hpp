#ifndef CONFIG_HPP
#define CONFIG_HPP

/*
    File of configs, these get used in many files in my source,
    Keeping them here makes me less annoyed
*/

// CPU running frequency
#define F_CPU 16000000U

// Running sample rate
// native ADC is exeedingly slow.
#define SAMPLE_RATE 1000000U

#if F_CPU == 16000000U
    #define MIRCOSECOND 16
#elif F_CPU == 8000000U
    #define MIRCOSECOND 8
#endif 

#endif