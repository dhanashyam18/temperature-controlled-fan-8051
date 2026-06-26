# Temperature Controlled Fan using 8051

## Overview

This project automatically controls a DC fan according to temperature using an 8051 microcontroller.

The temperature is measured using an LM35 sensor through an ADC0804. The fan speed is controlled using PWM generated with Timer0 interrupts.

## Features

- Temperature Monitoring
- LCD Display
- PWM Fan Speed Control
- ADC0804 Interface
- Proteus Simulation

## Components

- AT89C51
- LM35
- ADC0804
- 16x2 LCD
- L293D
- DC Fan

## Software

- Keil uVision
- Proteus 8

## Fan Logic

| Temperature | Fan |
|--------------|-----|
| <30°C | OFF |
| 30–49°C | Slow |
| 50–69°C | Medium |
| ≥70°C | Fast |

## Author

**Dhanashyam Babu**
