#include <Arduino.h>

// CONFIG (pins)
const int PIN_RED = 15;    // D15
const int PIN_GREEN = 4;   // D4
const int PIN_BLUE = 22;   // D22
const int PIN_YELLOW = 23; // D23
const int PIN_BTN = 14;    // D14
const int PIN_LDR = 33;    // D33 (ADC)

//  SELECT WHICH LAB EXERCISE TO RUN =
/*
  Set LAB_TASK to 1..6 to compile & run a single exercise:
    1) Blink RED
    2) Button toggles GREEN
    3) Read light sensor
    4) Light sensor -> LED band
    5) Snapshot on button
    6) Minimal serial control (BLUE on 'B' / off 'b')
*/
#define LAB_TASK 1
//

//  Helpers
inline void allOff()
{
    digitalWrite(PIN_RED, LOW);
    digitalWrite(PIN_GREEN, LOW);
    digitalWrite(PIN_BLUE, LOW);
    digitalWrite(PIN_YELLOW, LOW);
}

// For button with INPUT_PULLUP (pressed = LOW)
bool readButtonPressed()
{
    return digitalRead(PIN_BTN) == LOW;
}

// Debounce helper
bool edgePressed(unsigned long debounceMs = 30)
{
    static bool lastStable = false;
    static bool lastRead = false;
    static unsigned long lastChangeMs = 0;

    bool r = readButtonPressed();
    if (r != lastRead)
    {
        lastRead = r;
        lastChangeMs = millis();
    }
    if ((millis() - lastChangeMs) > debounceMs && r != lastStable)
    {
        lastStable = r;
        if (lastStable)
            return true; // pressed edge
    }
    return false;
}

//  SETUP
void setup()
{
    Serial.begin(115200);
    pinMode(PIN_RED, OUTPUT);
    pinMode(PIN_GREEN, OUTPUT);
    pinMode(PIN_BLUE, OUTPUT);
    pinMode(PIN_YELLOW, OUTPUT);
    pinMode(PIN_BTN, INPUT_PULLUP); // uses internal pull-up
    allOff();

#if LAB_TASK == 1
    Serial.println(F("[Task1] Blink RED LED"));
#elif LAB_TASK == 2
    Serial.println(F("[Task2] Button toggles GREEN"));
#elif LAB_TASK == 3
    Serial.println(F("[Task3] Read light sensor every 500ms"));
#elif LAB_TASK == 4
    Serial.println(F("[Task4] Light sensor -> LED band"));
#elif LAB_TASK == 5
    Serial.println(F("[Task5] Snapshot on button + YELLOW flash"));
#elif LAB_TASK == 6
    Serial.println(F("[Task6] Minimal serial control for BLUE ('B'/'b')"));
#endif
}

//  LOOP
void loop()
{

#if LAB_TASK == 1
    // -------- Task 1: Blink RED 500ms on/off & print state --------
    static unsigned long t0 = 0;
    static bool state = false;
    if (millis() - t0 >= 500)
    {
        t0 = millis();
        state = !state;
        digitalWrite(PIN_RED, state ? HIGH : LOW);
        Serial.println(state ? F("RED ON") : F("RED OFF"));
    }

#elif LAB_TASK == 2
    // -------- Task 2: Button toggles GREEN, print only on change --------
    static bool green = false;
    if (edgePressed())
    {
        green = !green;
        digitalWrite(PIN_GREEN, green ? HIGH : LOW);
        Serial.print(F("GREEN="));
        Serial.println(green ? 1 : 0);
    }

#elif LAB_TASK == 3
    // -------- Task 3: Read LDR every 500ms, print raw=XXXX --------
    static unsigned long t0 = 0;
    if (millis() - t0 >= 500)
    {
        t0 = millis();
        int raw = analogRead(PIN_LDR); // 0..4095 on ESP32
        Serial.print(F("raw="));
        Serial.println(raw);
    }

#elif LAB_TASK == 4
    // -------- Task 4: LDR to LED band (exactly one LED on) --------
    static unsigned long t0 = 0;
    if (millis() - t0 >= 200)
    { // sample a bit faster for responsiveness
        t0 = millis();
        int raw = analogRead(PIN_LDR);

        // clear all first
        allOff();

        if (raw <= 1023)
        {
            digitalWrite(PIN_BLUE, HIGH);
            Serial.println(F("band=BLUE"));
        }
        else if (raw <= 2047)
        {
            digitalWrite(PIN_GREEN, HIGH);
            Serial.println(F("band=GREEN"));
        }
        else if (raw <= 3071)
        {
            digitalWrite(PIN_YELLOW, HIGH);
            Serial.println(F("band=YELLOW"));
        }
        else
        {
            digitalWrite(PIN_RED, HIGH);
            Serial.println(F("band=RED"));
        }
    }

#elif LAB_TASK == 5
    //  Task 5: Wait for button; on press read once + flash YELLOW
    if (edgePressed())
    {
        int raw = analogRead(PIN_LDR);
        Serial.print(F("snapshot="));
        Serial.println(raw);

        digitalWrite(PIN_YELLOW, HIGH);
        delay(100); // simple short ack; allowed here
        digitalWrite(PIN_YELLOW, LOW);
    }

#elif LAB_TASK == 6
    // Task 6: Minimal serial control for BLUE
    static bool blue = false;
    if (Serial.available() > 0)
    {
        char c = (char)Serial.read();
        if (c == 'B')
        {
            blue = true;
            digitalWrite(PIN_BLUE, HIGH);
            Serial.println(F("BLUE=1"));
        }
        else if (c == 'b')
        {
            blue = false;
            digitalWrite(PIN_BLUE, LOW);
            Serial.println(F("BLUE=0"));
        }
    }

#endif
}
