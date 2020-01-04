Platform Hardware Information
============================

Brief
-----

This page contains information about the hardware platform this software (firmware) will run on. The firmware itself
runs on a STM32F103RET6 microcontroller, containing an ARM Cortex-M3 processor core. This chip is implemented on an
IntelliModule-Alpha controller board, which plugs into the main PCB of the platform. The main PCB is a backplane
connecting the keyboard switches together into a matrix. This matrix is read by the firmware and sent to the USB host.

Interfaces
----------

### I2C

- **I2C1**<br/>
  Used for communication between the main microcontroller and the I2C port expander chips which control the probe-lines
  of the keyboard matrix. The connected devices are listed below:

  + **PORTEX_0**<br/>
    *Device: PCA9555PW,118*<br/>
    *I2C-Bus Address: `0x20`*<br/>
    This device connects to the keyboard matrix probe lines 0 to 15.<br/>
    The following table shows the connections of the port expander:
    | Port Expander IO | Signal |
    | :-- | :-- |
    | IO0_0 | KBM_PROBE_0 |
    | IO0_1 | KBM_PROBE_1 |
    | IO0_2 | KBM_PROBE_2 |
    | IO0_3 | KBM_PROBE_3 |
    | IO0_4 | KBM_PROBE_4 |
    | IO0_5 | KBM_PROBE_5 |
    | IO0_6 | KBM_PROBE_6 |
    | IO0_7 | KBM_PROBE_7 |
    | IO1_0 | KBM_PROBE_8 |
    | IO1_1 | KBM_PROBE_9 |
    | IO1_2 | KBM_PROBE_10 |
    | IO1_3 | KBM_PROBE_11 |
    | IO1_4 | KBM_PROBE_12 |
    | IO1_5 | KBM_PROBE_13 |
    | IO1_6 | KBM_PROBE_14 |
    | IO1_7 | KBM_PROBE_15 |

  + **PORTEX_1**<br/>
    *Device: PCA9555PW,118*<br/>
    *I2C-Bus Address: `0x21`*<br/>
    This device connects to the keyboard matrix probe lines 16 to 21.<br/>
    The following table shows the connections of the port expander:
    | Port Expander IO | Signal |
    | :-- | :-- |
    | IO0_0 | KBM_PROBE_16 |
    | IO0_1 | KBM_PROBE_17 |
    | IO0_2 | KBM_PROBE_18 |
    | IO0_3 | KBM_PROBE_19 |
    | IO0_4 | KBM_PROBE_20 |
    | IO0_5 | KBM_PROBE_21 |
    | IO0_6 | *Not Connected* |
    | IO0_7 | *Not Connected* |
    | IO1_0 | *Not Connected* |
    | IO1_1 | *Not Connected* |
    | IO1_2 | *Not Connected* |
    | IO1_3 | *Not Connected* |
    | IO1_4 | *Not Connected* |
    | IO1_5 | *Not Connected* |
    | IO1_6 | *Not Connected* |
    | IO1_7 | *Not Connected* |

  + **EEPROM_0**<br/>
    *Device: CAT24C512WI-GT3*<br/>
    *Size: 512kb (64k x 8) (64kByte)*<br/>
    *I2C-Bus Address: `0x50`*<br/>
    This device sits on the IntelliModule-Alpha controller board and is a general-purpose EEPROM. It is used to save the
    keyboard matrix configuration.

### U(S)ART

- **USART1**<br/>
  Used as a serial debug connection port. This port directly connects to the debug header and is used by the firmware
  to output debug messages. This port is configured to use the baud rate 115200 by default.

GPIOs
-----

The following table shows the hardware-mapping of the processor's GPIO pins/ports:

| Pin Name | Direction | Pull-Resistor Config | Signal |
| :-- | :-- | :-- | :-- |
| PC12 | Input | None | KBM_DATA_0 |
| PC11 | Input | None | KBM_DATA_1 |
| PC10 | Input | None | KBM_DATA_2 |
| PC9 | Input | None | KBM_DATA_3 |
| PC8 | Input | None | KBM_DATA_4 |
| PC4 | Input | None | KBM_DATA_5 |
| PC5 | Input | None | KBM_DATA_6 |
| PC7 | Output (PWM) | None | LED_DATA_IN |
| PD2 | Input | None | PROBE_PORT_EXP_INTn |
| PB0 | Output | None | USER_LED_0 |
| PB1 | Output | None | USER_LED_1 |
| PB2 | Output | None | USER_LED_2 |
| PA4 | Output | None | ERROR_LED |
| PA8 | Input | None | HWBIT0 |
| PB8 | Input | None | HWBIT1 |
| PB9 | Input | None | HWBIT2 |
| PB12 | Input | None | HWBIT3 |
| PC6 | Input | None | HWBIT4 |
| PB5 | Output | None | EEPROM_WP |