/* Copyright 2017 Jason Williams
 * Copyright 2018 Jack Humbert
 * Copyright 2018 Yiancar
 * Copyright 2020 MelGeek
 * Copyright 2021 MasterSpoon
 * Copyright 2023 Framework Computer Inc
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#pragma once

#include <stdint.h>
#include <stdbool.h>
#include "progmem.h"

extern uint8_t aw20xxx_chip_id[4];

#define AW20_TIMEOUT 100

#define AW20198_PAGE_FUNC 0xC0
#define AW20198_PAGE_PWM 0xC1
#define AW20198_PAGE_SCALE 0xC2
#define AW20198_PAGE_PAT 0xC3
#define AW20198_PAGE_PWM_SCALE 0xC4

#define AW20198_REG_GCR 0x00
#define AW20198_REG_GCR_SWSEL_MASK 0xF0
#define AW20198_REG_GCR_SWSEL_SHIFT 4
#define AW20198_REG_GCR_SW1_ACTIVE 0
#define AW20198_REG_GCR_SW1_TO_SW2_ACTIVE 1
#define AW20198_REG_GCR_SW1_TO_SW3_ACTIVE 2
#define AW20198_REG_GCR_SW1_TO_SW4_ACTIVE 3
#define AW20198_REG_GCR_SW1_TO_SW5_ACTIVE 4
#define AW20198_REG_GCR_SW1_TO_SW6_ACTIVE 5
#define AW20198_REG_GCR_SW1_TO_SW7_ACTIVE 6
#define AW20198_REG_GCR_SW1_TO_SW8_ACTIVE 7
#define AW20198_REG_GCR_SW1_TO_SW9_ACTIVE 8
#define AW20198_REG_GCR_SW1_TO_SW10_ACTIVE 9
#define AW20198_REG_GCR_SW1_TO_SW11_ACTIVE 10

#define AW20198_REG_GCC 0x01
#define AW20198_REG_MIXCR 0x46
#define AW20198_REG_PAGE 0xF0

#define AW20198_RESET_MAGIC 0xAE

#define BIT_CHIPEN_DIS			(~(1<<0))
#define BIT_CHIPEN_EN			(0x1)

#ifdef RGB_MATRIX_ENABLE
typedef struct aw20_led {
    uint8_t driver;
    uint8_t r;
    uint8_t g;
    uint8_t b;
} __attribute__((packed)) aw20_led;

extern const aw20_led g_aw20_leds[RGB_MATRIX_LED_COUNT];

#elif defined(LED_MATRIX_ENABLE)
typedef struct aw20_led {
    uint8_t driver;
    uint8_t v;
} __attribute__((packed)) aw20_led;

extern const aw20_led __flash g_aw20_leds[LED_MATRIX_LED_COUNT];
#endif

// TODO: CHECK AW20_
#ifdef AW20_MANUAL_SCALING
extern const aw20_led __flash g_aw20_scaling[];
void AW20_set_manual_scaling_buffer(void);
#endif

void AW20_write_single_register(uint8_t addr, uint8_t reg, uint8_t data);
bool AW20_write_multi_registers(uint8_t addr, uint8_t *source_buffer, uint8_t buffer_size, uint8_t transfer_size, uint8_t start_reg_addr);
void AW20_unlock_register(uint8_t addr, uint8_t page);
void AW20_common_init(uint8_t addr, uint8_t ssr);

void AW20_common_update_pwm_register(uint8_t addr, uint8_t index);
void AW20_common_update_scaling_register(uint8_t addr, uint8_t index);

#ifdef RGB_MATRIX_ENABLE
// RGB Matrix Specific scripts
void AW20_RGB_set_color(int index, uint8_t red, uint8_t green, uint8_t blue);
void AW20_RGB_set_color_all(uint8_t red, uint8_t green, uint8_t blue);
void AW20_RGB_set_scaling_buffer(uint8_t index, bool red, bool green, bool blue);
#elif defined(LED_MATRIX_ENABLE)
// LED Matrix Specific scripts
void AW20_simple_set_scaling_buffer(uint8_t index, bool value);
void AW20_simple_set_brightness(int index, uint8_t value);
void AW20_simple_set_brigntness_all(uint8_t value);
#endif

// This is a 7-bit address, that gets left-shifted and bit 0
// set to 0 for write, 1 for read (as per I2C protocol)
// The address will vary depending on your wiring:
// 00 <-> GND
// 01 <-> SCL
// 10 <-> SDA
// 11 <-> VCC
// ADDR1 represents A1:A0 of the 7-bit address.
// ADDR2 represents A3:A2 of the 7-bit address.
// The result is: 0b010(ADDR2)(ADDR1)
#ifndef DRIVER_ADDR_1
#    define DRIVER_ADDR_1 0b0100000
#endif

// Set defaults for Spread Spectrum Register
#ifndef AW20_SSR_1
#    if DRIVER_COUNT == 1
#        define AW20_SSR_1 0x00
#    else
#        define AW20_SSR_1 0x80
#    endif
#endif
#ifndef AW20_SSR_2
#    define AW20_SSR_2 0x40
#endif
#ifndef AW20_SSR_3
#    define AW20_SSR_3 0x40
#endif
#ifndef AW20_SSR_4
#    define AW20_SSR_4 0x40
#endif

// Command Registers
#define AW20_COMMANDREGISTER_WRITELOCK 0xFE
#define AW20_COMMANDREGISTER 0xFD
#define AW20_IDREGISTER 0xFC
#define AW20_REGISTER_UNLOCK 0xC5

// Response Registers
//#define AW20_PAGE_PWM 0x00
//#define AW20_PAGE_SCALING 0x01
//#define AW20_PAGE_FUNCTION 0x02

// Registers under Function Register
#define AW20_REG_CONFIGURATION 0x00 // GCR
#define AW20_REG_GLOBALCURRENT 0x01
#define AW20_REG_PULLDOWNUP 0x02
#define AW20_REG_TEMP 0x27
#define AW20_REG_SSR 0x28
#define AW20_REG_PWM_CLOCK 0x28
#define AW20_REG_PCCR 0x29
#define AW20_REG_UVLO 0x2A
#define AW20_REG_SRCR 0x2B
#define AW20_REG_RESET 0x2F

#define PWMFRQ_62k 0x00
#define PWMFRQ_31k 0x20
#define PWMFRQ_15k 0x40
#define PWMFRQ_7k  0x60

#define PHSEL_SYNC   0x00
#define PHSEL_INVERT 0x01
#define PHSEL_THREE  0x10

#define SSE_ENABLE  0x10
#define SSE_DISABLE 0x00
#define SSR_5PCT    0x00
#define SSR_15PCT   0x04
#define SSR_25PCT   0x08
#define SSR_35PCT   0x0C

#define SLEW_SSR_1NS 0x00 // Default
#define SLEW_SSR_6NS 0x04

#define SLEW_SRF_1NS  0x00
#define SLEW_SRF_3NS  0x01
#define SLEW_SRF_6NS  0x02 // Default
#define SLEW_SRF_10NS 0x03

// Set defaults for Function Registers
#ifndef AW20_CONFIGURATION
#    define AW20_CONFIGURATION 0x01
#endif
//#ifndef AW20_GLOBALCURRENT
//#    define AW20_GLOBALCURRENT 0xFF
//#endif
#ifndef AW20_PULLDOWNUP
#    define AW20_PULLDOWNUP 0x33 // CSx pull up and SWx pull down 8k
#endif
#ifndef AW20_TEMP
#    define AW20_TEMP 0x00 // Default
#endif

// Set defaults for Scaling registers
#ifndef AW20_SCAL_RED
#    define AW20_SCAL_RED 0xFF
#endif
#ifndef AW20_SCAL_BLUE
#    define AW20_SCAL_BLUE 0xFF
#endif
#ifndef AW20_SCAL_GREEN
#    define AW20_SCAL_GREEN 0xFF
#endif
#define AW20_SCAL_RED_OFF 0x00
#define AW20_SCAL_GREEN_OFF 0x00
#define AW20_SCAL_BLUE_OFF 0x00

#ifndef AW20_SCAL_LED
#    define AW20_SCAL_LED 0xFF
#endif
#define AW20_SCAL_LED_OFF 0x00

// Set buffer sizes
#define AW20_MAX_LEDS 198
#define AW20_SCALING_SIZE 198
#define AW20_PWM_TRF_SIZE 18
#define AW20_SCALING_TRF_SIZE 18

// Location of 1st bit for PWM and Scaling registers
#define AW20_PWM_REG_1ST 0x00
#define AW20_SCL_REG_1ST 0x00

// Map CS SW locations to order in PWM / Scaling buffers
// This matches the ORDER in the Datasheet Register not the POSITION
// It will always count from 0x00 to (AW20_MAX_LEDS - 1)
#define CS1_SW1 0x00
#define CS2_SW1 0x01
#define CS3_SW1 0x02
#define CS4_SW1 0x03
#define CS5_SW1 0x04
#define CS6_SW1 0x05
#define CS7_SW1 0x06
#define CS8_SW1 0x07
#define CS9_SW1 0x08
#define CS10_SW1 0x09
#define CS11_SW1 0x0A
#define CS12_SW1 0x0B
#define CS13_SW1 0x0C
#define CS14_SW1 0x0D
#define CS15_SW1 0x0E
#define CS16_SW1 0x0F
#define CS17_SW1 0x10
#define CS18_SW1 0x11

#define CS1_SW2 0x12
#define CS2_SW2 0x13
#define CS3_SW2 0x14
#define CS4_SW2 0x15
#define CS5_SW2 0x16
#define CS6_SW2 0x17
#define CS7_SW2 0x18
#define CS8_SW2 0x19
#define CS9_SW2 0x1A
#define CS10_SW2 0x1B
#define CS11_SW2 0x1C
#define CS12_SW2 0x1D
#define CS13_SW2 0x1E
#define CS14_SW2 0x1F
#define CS15_SW2 0x20
#define CS16_SW2 0x21
#define CS17_SW2 0x22
#define CS18_SW2 0x23

#define CS1_SW3 0x24
#define CS2_SW3 0x25
#define CS3_SW3 0x26
#define CS4_SW3 0x27
#define CS5_SW3 0x28
#define CS6_SW3 0x29
#define CS7_SW3 0x2A
#define CS8_SW3 0x2B
#define CS9_SW3 0x2C
#define CS10_SW3 0x2D
#define CS11_SW3 0x2E
#define CS12_SW3 0x2F
#define CS13_SW3 0x30
#define CS14_SW3 0x31
#define CS15_SW3 0x32
#define CS16_SW3 0x33
#define CS17_SW3 0x34
#define CS18_SW3 0x35

#define CS1_SW4 0x36
#define CS2_SW4 0x37
#define CS3_SW4 0x38
#define CS4_SW4 0x39
#define CS5_SW4 0x3A
#define CS6_SW4 0x3B
#define CS7_SW4 0x3C
#define CS8_SW4 0x3D
#define CS9_SW4 0x3E
#define CS10_SW4 0x3F
#define CS11_SW4 0x40
#define CS12_SW4 0x41
#define CS13_SW4 0x42
#define CS14_SW4 0x43
#define CS15_SW4 0x44
#define CS16_SW4 0x45
#define CS17_SW4 0x46
#define CS18_SW4 0x47

#define CS1_SW5 0x48
#define CS2_SW5 0x49
#define CS3_SW5 0x4A
#define CS4_SW5 0x4B
#define CS5_SW5 0x4C
#define CS6_SW5 0x4D
#define CS7_SW5 0x4E
#define CS8_SW5 0x4F
#define CS9_SW5 0x50
#define CS10_SW5 0x51
#define CS11_SW5 0x52
#define CS12_SW5 0x53
#define CS13_SW5 0x54
#define CS14_SW5 0x55
#define CS15_SW5 0x56
#define CS16_SW5 0x57
#define CS17_SW5 0x58
#define CS18_SW5 0x59

#define CS1_SW6 0x5A
#define CS2_SW6 0x5B
#define CS3_SW6 0x5C
#define CS4_SW6 0x5D
#define CS5_SW6 0x5E
#define CS6_SW6 0x5F
#define CS7_SW6 0x60
#define CS8_SW6 0x61
#define CS9_SW6 0x62
#define CS10_SW6 0x63
#define CS11_SW6 0x64
#define CS12_SW6 0x65
#define CS13_SW6 0x66
#define CS14_SW6 0x67
#define CS15_SW6 0x68
#define CS16_SW6 0x69
#define CS17_SW6 0x6A
#define CS18_SW6 0x6B

#define CS1_SW7 0x6C
#define CS2_SW7 0x6D
#define CS3_SW7 0x6E
#define CS4_SW7 0x6F
#define CS5_SW7 0x70
#define CS6_SW7 0x71
#define CS7_SW7 0x72
#define CS8_SW7 0x73
#define CS9_SW7 0x74
#define CS10_SW7 0x75
#define CS11_SW7 0x76
#define CS12_SW7 0x77
#define CS13_SW7 0x78
#define CS14_SW7 0x79
#define CS15_SW7 0x7A
#define CS16_SW7 0x7B
#define CS17_SW7 0x7C
#define CS18_SW7 0x7D

#define CS1_SW8 0x7E
#define CS2_SW8 0x7F
#define CS3_SW8 0x80
#define CS4_SW8 0x81
#define CS5_SW8 0x82
#define CS6_SW8 0x83
#define CS7_SW8 0x84
#define CS8_SW8 0x85
#define CS9_SW8 0x86
#define CS10_SW8 0x87
#define CS11_SW8 0x88
#define CS12_SW8 0x89
#define CS13_SW8 0x8A
#define CS14_SW8 0x8B
#define CS15_SW8 0x8C
#define CS16_SW8 0x8D
#define CS17_SW8 0x8E
#define CS18_SW8 0x8F

#define CS1_SW9 0x90
#define CS2_SW9 0x91
#define CS3_SW9 0x92
#define CS4_SW9 0x93
#define CS5_SW9 0x94
#define CS6_SW9 0x95
#define CS7_SW9 0x96
#define CS8_SW9 0x97
#define CS9_SW9 0x98
#define CS10_SW9 0x99
#define CS11_SW9 0x9A
#define CS12_SW9 0x9B
#define CS13_SW9 0x9C
#define CS14_SW9 0x9D
#define CS15_SW9 0x9E
#define CS16_SW9 0x9F
#define CS17_SW9 0xA0
#define CS18_SW9 0xA1

#define CS1_SW10 0xA2
#define CS2_SW10 0xA3
#define CS3_SW10 0xA4
#define CS4_SW10 0xA5
#define CS5_SW10 0xA6
#define CS6_SW10 0xA7
#define CS7_SW10 0xA8
#define CS8_SW10 0xA9
#define CS9_SW10 0xAA
#define CS10_SW10 0xAB
#define CS11_SW10 0xAC
#define CS12_SW10 0xAD
#define CS13_SW10 0xAE
#define CS14_SW10 0xAF
#define CS15_SW10 0xB0
#define CS16_SW10 0xB1
#define CS17_SW10 0xB2
#define CS18_SW10 0xB3

#define CS1_SW11 0xB4
#define CS2_SW11 0xB5
#define CS3_SW11 0xB6
#define CS4_SW11 0xB7
#define CS5_SW11 0xB8
#define CS6_SW11 0xB9
#define CS7_SW11 0xBA
#define CS8_SW11 0xBB
#define CS9_SW11 0xBC
#define CS10_SW11 0xBD
#define CS11_SW11 0xBE
#define CS12_SW11 0xBF
#define CS13_SW11 0xC0
#define CS14_SW11 0xC1
#define CS15_SW11 0xC2
#define CS16_SW11 0xC3
#define CS17_SW11 0xC4
#define CS18_SW11 0xC5
