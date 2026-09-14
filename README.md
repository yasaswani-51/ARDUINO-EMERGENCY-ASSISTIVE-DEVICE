# ARDUINO-EMERGENCY-ASSISTIVE-DEVICE
An Arduino-based emergency assistive device that provides visual, audible, and mechanical alerts using a push button, LED, buzzer, potentiometer, and servo motor.
# Arduino Emergency Assistive Device

## Project Description

The Arduino Emergency Assistive Device is a simple and low-cost embedded system designed to provide a local emergency alert.

The system uses an Arduino UNO, push button, potentiometer, LED, buzzer, and servo motor.

When the user presses and holds the button for the selected time, the system enters emergency mode. The LED flashes rapidly, the buzzer produces an alert sound, and the servo motor rotates to 90 degrees. The servo can be connected to a small HELP/EMERGENCY flag for visible indication.

A potentiometer is used to adjust the activation time between approximately 1 and 5 seconds. A short button press is cancelled to reduce accidental activation.

## Components Used

- Arduino UNO
- Push Button
- Potentiometer
- LED
- 220 Ω Resistor
- Buzzer
- Servo Motor
- Breadboard
- Jumper Wires

## Pin Configuration

| Component | Arduino Pin |
|---|---|
| Push Button | D2 |
| LED | D8 |
| Buzzer | D9 |
| Potentiometer | A1 |
| Servo Motor | D6 |

## Working

The system operates in three modes:

### 1. Normal Mode
- LED is OFF
- Buzzer is OFF
- Servo remains at 0°

### 2. Activation Check Mode
- User holds the button
- LED blinks slowly
- System checks the selected activation time
- Releasing the button early cancels the activation

### 3. Emergency Mode
- LED flashes rapidly
- Buzzer produces an alert
- Servo rotates to 90°
- Emergency mode continues until reset

## Reset

After emergency mode is activated, the user must release the button and then hold it again for 5 seconds.

After successful reset:
- LED turns OFF
- Buzzer stops
- Servo returns to 0°
- System returns to Normal Mode

## Technologies Used

- Arduino UNO
- Arduino C/C++
- Arduino IDE
- Tinkercad Circuits

## Project Output

The project provides three types of local emergency indication:

1. Visual indication using LED
2. Audible indication using buzzer
3. Mechanical indication using servo motor

## Future Scope

- Add wireless communication for remote alerts
- Add a display for system status
- Add battery backup
- Improve the physical emergency flag and enclosure

## Project Team

- K. Bhavitha
- Doki Yasaswani
- T. Lakshmi Gayathri

## Course

23CSE201 - Procedural Programming Using C

B.Tech CSE - III Semester
Amrita Vishwa Vidyapeetham, Nagercoil
