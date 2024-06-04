# Medicine Reminder System

## Project Overview
This project addresses the challenge of medication management, particularly for elderly individuals who may face difficulties with timely pill intake. The Arduino-based solution combines a pill organizer with a buzzer reminder and RFID-controlled access to pill compartments, enhancing safety and preventing unauthorized access.

## Problem
The problem lies in ensuring timely medication intake and preventing children from accessing medication.

## Solution
Our solution optimizes medication organization, preventing missed doses and unauthorized access. At user-defined times, a buzzer and LCD message remind the user to take their medication. The corresponding pill compartment is then unlocked via a servo motor upon presenting the correct RFID tag, ensuring secure access. After taking the medication, the user can close the compartment by pressing a button. A real-time clock (RTC) module accurately tracks and stores time, enabling precise medication reminders and logging.

## Functionality
- **Buzzer Reminder:** At pre-set times, a buzzer sounds and an LCD message prompts the user to take their medication.
- **RFID Access Control:** Only an authorized RFID tag triggers the servo motor to unlock the designated pill compartment, preventing unauthorized access.
- **Servo Motor Control:** Servo motors control the opening and closing of pill compartments based on RFID authorization and button presses.
- **Manual Closure:** After medication intake, the user presses a button to close and secure the compartment.
- **Timekeeping:** An RTC module accurately tracks time, displays it on the LCD screen, and ensures precise timing for medication reminders.

## Hardware Components
- Arduino UNO R3 development board
- Jumper wires (male-to-female, male-to-male)
- 3x SG90 micro servo motors
- RC522 RFID module + tag
- DS3231 real-time clock module
- 5V passive buzzer
- 1602 LCD with I2C interface and yellow-green backlight
- Small push button
- 9V battery connector
- 9V battery + CR2032 3V battery for RTC

## Project Resources
- **Arduino Code:** Complete Arduino sketch implementing the project logic.
- **Hardware Diagram:** Schematic illustrating the circuit connections and component layout.
- **Photo:** Image showcasing the assembled project.

