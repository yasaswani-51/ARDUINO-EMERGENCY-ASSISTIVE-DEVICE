/*
=========================================================
       ARDUINO EMERGENCY ASSISTIVE DEVICE
=========================================================

FEATURES:

1. NORMAL MODE
   - LED OFF
   - Buzzer OFF
   - Servo at 0 degrees

2. ACTIVATION CHECK MODE
   - Button is being held
   - LED blinks slowly
   - Buzzer OFF
   - Servo remains at 0 degrees

3. EMERGENCY MODE
   - Button held for required time
   - LED flashes rapidly
   - Buzzer sounds
   - Servo rotates to 90 degrees

4. FALSE TRIGGER PREVENTION
   - Short press is cancelled

5. ADJUSTABLE ACTIVATION TIME
   - Potentiometer sets hold time
   - Range: 1 to 5 seconds

6. EMERGENCY RESET
   - Release button after emergency
   - Press again and hold for 5 seconds
   - Servo returns to 0 degrees
   - System returns to NORMAL mode


CONNECTIONS:

Button          -> D2 and GND
Potentiometer   -> A1 (middle pin)
                   5V and GND (outer pins)
LED             -> D8 through 220 ohm resistor
Buzzer          -> D9

Servo:
Signal          -> D6
VCC             -> 5V
GND             -> GND

=========================================================
*/

#include <Servo.h>


// ------------------------------------------------------
// PIN DEFINITIONS
// ------------------------------------------------------

const int BUTTON_PIN = 2;
const int LED_PIN = 8;
const int BUZZER_PIN = 9;
const int POT_PIN = A1;
const int SERVO_PIN = 6;


// ------------------------------------------------------
// SERVO OBJECT
// ------------------------------------------------------

Servo emergencyServo;


// ------------------------------------------------------
// SYSTEM MODES
// ------------------------------------------------------

#define NORMAL_MODE       0
#define ACTIVATION_MODE   1
#define EMERGENCY_MODE    2

int systemMode = NORMAL_MODE;


// ------------------------------------------------------
// TIMING VARIABLES
// ------------------------------------------------------

unsigned long buttonPressStart = 0;
unsigned long activationHoldTime = 3000;

unsigned long ledPreviousTime = 0;

unsigned long resetStartTime = 0;


// ------------------------------------------------------
// STATUS VARIABLES
// ------------------------------------------------------

bool buttonWasPressed = false;
bool ledState = false;

// Becomes true only after the user releases the button
// after emergency activation.
bool emergencyButtonReleased = false;


// ------------------------------------------------------
// SETUP
// ------------------------------------------------------

void setup()
{
    // Button
    pinMode(BUTTON_PIN, INPUT_PULLUP);

    // LED
    pinMode(LED_PIN, OUTPUT);

    // Buzzer
    pinMode(BUZZER_PIN, OUTPUT);

    // Servo
    emergencyServo.attach(SERVO_PIN);

    // Start servo at 0 degrees
    emergencyServo.write(0);

    // Everything OFF
    digitalWrite(LED_PIN, LOW);
    noTone(BUZZER_PIN);

    Serial.begin(9600);

    Serial.println("=================================");
    Serial.println(" Emergency Assistive Device");
    Serial.println(" System Started");
    Serial.println("=================================");
}


// ------------------------------------------------------
// MAIN LOOP
// ------------------------------------------------------

void loop()
{
    // Read potentiometer
    readPotentiometer();


    // Check current system mode
    if (systemMode == NORMAL_MODE)
    {
        normalMode();
    }

    else if (systemMode == ACTIVATION_MODE)
    {
        activationCheckMode();
    }

    else if (systemMode == EMERGENCY_MODE)
    {
        emergencyMode();
    }
}


// ======================================================
// POTENTIOMETER
// ======================================================

void readPotentiometer()
{
    int potValue;

    potValue = analogRead(POT_PIN);

    /*
       Potentiometer:

       Minimum -> 1 second
       Maximum -> 5 seconds
    */

    activationHoldTime = map(
        potValue,
        0,
        1023,
        1000,
        5000
    );
}


// ======================================================
// NORMAL MODE
// ======================================================

void normalMode()
{
    // LED OFF
    digitalWrite(LED_PIN, LOW);

    // Buzzer OFF
    noTone(BUZZER_PIN);

    // Servo at 0 degrees
    emergencyServo.write(0);


    int buttonState;

    buttonState = digitalRead(BUTTON_PIN);


    // --------------------------------------------------
    // BUTTON PRESSED
    // --------------------------------------------------

    if (buttonState == LOW)
    {
        if (!buttonWasPressed)
        {
            buttonPressStart = millis();

            buttonWasPressed = true;

            Serial.println("Button pressed.");
            Serial.println("Activation check started.");

            // Go to activation mode
            systemMode = ACTIVATION_MODE;
        }
    }

    // --------------------------------------------------
    // BUTTON NOT PRESSED
    // --------------------------------------------------

    else
    {
        buttonWasPressed = false;
    }
}


// ======================================================
// ACTIVATION CHECK MODE
// ======================================================

void activationCheckMode()
{
    int buttonState;

    buttonState = digitalRead(BUTTON_PIN);


    // --------------------------------------------------
    // BUTTON STILL BEING HELD
    // --------------------------------------------------

    if (buttonState == LOW)
    {
        unsigned long currentTime = millis();


        // ------------------------------------------------
        // SLOW LED BLINK
        // ------------------------------------------------

        if (currentTime - ledPreviousTime >= 500)
        {
            ledPreviousTime = currentTime;

            ledState = !ledState;

            digitalWrite(LED_PIN, ledState);
        }


        // ------------------------------------------------
        // CHECK REQUIRED HOLD TIME
        // ------------------------------------------------

        if (currentTime - buttonPressStart >= activationHoldTime)
        {
            Serial.println("Required hold time completed!");

            Serial.println("!!! EMERGENCY ACTIVATED !!!");


            // Change to emergency mode
            systemMode = EMERGENCY_MODE;


            // ------------------------------------------------
            // SERVO ROTATES TO 90 DEGREES
            // ------------------------------------------------

            emergencyServo.write(90);

            Serial.println("Servo rotated to 90 degrees.");


            // Reset LED timing
            ledPreviousTime = millis();

            ledState = false;

            digitalWrite(LED_PIN, LOW);

            // The button has NOT been released yet
            emergencyButtonReleased = false;

            // Reset reset timer
            resetStartTime = 0;
        }
    }


    // --------------------------------------------------
    // BUTTON RELEASED TOO EARLY
    // --------------------------------------------------

    else
    {
        Serial.println("Button released too early.");
        Serial.println("Emergency activation cancelled.");

        // Return to normal
        systemMode = NORMAL_MODE;

        buttonWasPressed = false;

        digitalWrite(LED_PIN, LOW);

        noTone(BUZZER_PIN);

        // Servo returns to 0
        emergencyServo.write(0);
    }
}


// ======================================================
// EMERGENCY MODE
// ======================================================

void emergencyMode()
{
    int buttonState;

    buttonState = digitalRead(BUTTON_PIN);


    // --------------------------------------------------
    // EMERGENCY ALERT
    // --------------------------------------------------

    unsigned long currentTime = millis();


    // --------------------------------------------------
    // RAPID LED FLASHING
    // --------------------------------------------------

    if (currentTime - ledPreviousTime >= 200)
    {
        ledPreviousTime = currentTime;

        ledState = !ledState;

        digitalWrite(LED_PIN, ledState);


        // ------------------------------------------------
        // BUZZER
        // ------------------------------------------------

        if (ledState == true)
        {
            tone(BUZZER_PIN, 2000);
        }

        else
        {
            noTone(BUZZER_PIN);
        }
    }


    // --------------------------------------------------
    // WAIT FOR BUTTON TO BE RELEASED
    // --------------------------------------------------

    if (buttonState == HIGH)
    {
        // Button has been released
        emergencyButtonReleased = true;

        // Reset the reset timer
        resetStartTime = 0;

        Serial.println("Button released.");
        Serial.println("Press and hold again for 5 seconds to reset.");
    }


    // --------------------------------------------------
    // RESET AFTER BUTTON IS RELEASED
    // --------------------------------------------------

    if (emergencyButtonReleased == true)
    {
        if (buttonState == LOW)
        {
            // Start reset timer
            if (resetStartTime == 0)
            {
                resetStartTime = millis();

                Serial.println("Reset started...");
                Serial.println("Hold button for 5 seconds.");
            }


            // ------------------------------------------------
            // 5 SECOND RESET
            // ------------------------------------------------

            if (millis() - resetStartTime >= 5000)
            {
                Serial.println("---------------------------------");
                Serial.println("Emergency reset completed.");
                Serial.println("Servo returning to 0 degrees.");
                Serial.println("Returning to NORMAL mode.");
                Serial.println("---------------------------------");


                // Stop alarm
                digitalWrite(LED_PIN, LOW);

                noTone(BUZZER_PIN);


                // ------------------------------------------------
                // SERVO RETURNS TO 0 DEGREES
                // ------------------------------------------------

                emergencyServo.write(0);


                // Reset variables
                ledState = false;

                buttonWasPressed = false;

                resetStartTime = 0;

                emergencyButtonReleased = false;


                // Return to normal
                systemMode = NORMAL_MODE;


                delay(500);
            }
        }

        else
        {
            // Button released before 5 seconds
            resetStartTime = 0;
        }
    }
}