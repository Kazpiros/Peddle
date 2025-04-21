/*
    Device communication starts with command byte,
    then followed by databytes on SDO or SDI depending on
    READ or WRITE
*/
#include <avr/io.h>

#define SPI_DEVICE_ADDR (0b01)

/* Writable addresses are 0x01 to 0x0D */
#define ADDR_LENGTH (16)
/*  CMD byte structure (masks):      */
#define DEVICE_ADDRESS_MASK 0xC0 // Device address
#define FCMD_RADDR_MASK 0x3C // Register Address/Fast Command Bits
#define CMD_TYPE_MASK 0x3 // Command Type Bits

/* Fast Command Bytes*/
#define MCP_ADC_CONVERSION_START (0x28)
#define MCP_ADC_STANDBY (0x2C)
#define MCP_ADC_SHUTDOWN (0X30)
#define MCP_ADC_FULL_SHUTDOWN (0x34)
#define MCP_ADC_RESET (0X38)

/* ADDR[5:2] CMD[1:0]*/
#define MCP_ADC_STATIC_READ (0x01)// Static read of register address
#define MCP_ADC_WRITE (0x02)    // Incremental Write at Register address
#define MCP_ADC_READ (0x03)     // Incremental Read at Register address

#define MCP_ADC_EN_FASTCMD 1 // Must be flipped in IRQ Register (default)

/* Status Byte Structure (masks):*/
#define MCP_ADC_DEV_ADDR_ACK (0x30) // STAT[5:4] = DEV_ADDR[1:0]

// Complementary to STAT[5], used to determine if "output or HiZ"
#define MCP_ADC_DEV_ADDR_HIZ (0x08) // STAT[3] = !DEV_ADDR[0]
#define MCP_ADC_DR_STATUS (0x04) // STAT[2] = !ADC_READY Interrupt
#define MCP_ADC_CRCCFG_STATUS (0x02) //STAT[1] = !(CRC Bit error on Reg Map)
#define MCP_ADC_POR_STATUS (0x01) // STAT[0] = !(POR_int_status)

/* ensure SPI_mode bits are set for proper SCK reading */

// CONFIGURATION REGISTER 0
#define VREF_SEL   1       // Bit 7: Reference voltage selection (1 = Internal reference)
#define PSM        1       // Bit 6: Partial shutdown mode (1 = Enabled)
#define CLK_SEL    2       // Bits 5-4: Clock selection (10 = Internal clock)
#define CS_SEL     0       // Bits 3-2: Current source selection (00 = No source)
#define ADC_MODE   3       // Bits 1-0: ADC mode (11 = Conversion mode)
#define CONFIG0    (uint8_t)((VREF_SEL << 7) | (PSM << 6) | (CLK_SEL << 4) | (CS_SEL << 2) | (ADC_MODE))

// CONFIGURATION REGISTER 1
#define PRE        0       // Bits 7-6: Prescaler setting (00 -> AMCLK = MCLK)
#define OSR        3       // Bits 5-2: Oversampling ratio (0011 = 256)
#define CONFIG1    (uint8_t)((PRE << 6) | (OSR << 2))

// CONFIGURATION REGISTER 2
#define BOOST      2       // Bits 7-6: Boost setting (10 = Option 2)
#define GAIN       1       // Bits 5-3: Gain setting (001 = Option 1)
#define AZ_MUX     0       // Bit 2: Auto-zeroing MUX (0 = Disabled)
#define AZREF      1       // Bit 1: Auto-zeroing reference (1 = Enabled)
#define CONFIG2    (uint8_t)((BOOST << 6) | (GAIN << 3) | (AZ_MUX << 2) | (AZREF << 1))

// CONFIGURATION REGISTER 3
#define CONV_MODE    3     // Bits 7-6: Conversion mode (11 = Mode 3)
#define DATA_FORMAT  0     // Bits 5-4: Data format (00 = default 16 bit data output)
#define CRC_FORMAT   0     // Bit 3: CRC format (0 = Option 0)
#define EN_CRCCOM    0     // Bit 2: Enable CRC communication (0 = Disabled)
#define EN_OFFCAL    0     // Bit 1: Enable offset calibration (0 = Disabled)
#define EN_GAINCAL   0     // Bit 0: Enable gain calibration (0 = Disabled)
#define CONFIG3      (uint8_t)((CONV_MODE << 6) | (DATA_FORMAT << 4) | (CRC_FORMAT << 3) | (EN_CRCCOM << 2) | (EN_OFFCAL << 1) | (EN_GAINCAL))

// IRQ REGISTER
#define IRQ_MODE         1  // Bits 3-2: IRQ mode (01 = Mode 1)
#define MCP_ADC_EN_FASTCMD 1  // Bit 1: Enable fast command (1 = Enabled)
#define EN_STP           1  // Bit 0: Enable STP (1 = Enabled)
#define IRQ_REG          (uint8_t)((IRQ_MODE << 2) | (MCP_ADC_EN_FASTCMD << 1) | (EN_STP))

// MUX REGISTER
#define MUX_VINP        0   // Bits 7-4: Positive input channel selection (0000)
#define MUX_VINM        1   // Bits 3-0: Negative input channel selection (0001)
#define MUX_REG         (uint8_t)((MUX_VINP << 4) | (MUX_VINM))

#define SCAN_REG        0 // zero default is ok
// 24 bit register
#define TIMER_REG       0 // zero default is ok

#define OFFSETCAL_REG   0 // zero default is ok? should be disabled from above

#define GAINCAL_REG     0 // zero default is ok? should be disabled from above

#define LOCK_REG        0xA5 // set pswd to 0x00

#define CRCCFG_REG      0 // RO  

#define MCP_ADC_SINGLE_READ ((SPI_DEVICE_ADDR << 6) | ( 0x00 << 2) | (MCP_ADC_STATIC_READ))
#define MCP_ADC_INC_READ ((SPI_DEVICE_ADDR << 6) | ( 0x00 << 2) | (MCP_ADC_READ))

static uint8_t incWriteLoop[ADDR_LENGTH] =
{
    0x00,
    CONFIG0,
    CONFIG1,
    CONFIG2,
    CONFIG3,
    IRQ_REG,
    MUX_REG,
    SCAN_REG,
    TIMER_REG,
    OFFSETCAL_REG,
    GAINCAL_REG,
    0x00,
    0x00,
    LOCK_REG,
    0x00
};

uint8_t incReadLoop[ADDR_LENGTH];