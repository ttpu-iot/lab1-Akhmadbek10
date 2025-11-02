# Lab 1 Solution Code Review

**Date:** 2025-11-02  
**Reviewer:** GitHub Copilot Coding Agent  
**Repository:** ttpu-iot/lab1-Akhmadbek10

---

## Executive Summary

This document provides a comprehensive review of the solution code in `src/main.cpp` against the 6 exercises specified in `README.md`. 

**Overall Assessment: ✅ ALL EXERCISES PASSED**

All 6 exercises have been correctly implemented with proper functionality, timing, and serial output formatting. The code demonstrates good understanding of Arduino/ESP32 programming concepts including GPIO control, analog reading, timing, debouncing, and serial communication.

---

## Pin Configuration Verification

| Component | Pin Assignment | Expected (README) | Status |
|-----------|---------------|-------------------|---------|
| RED LED | D15 | D15 | ✅ Correct |
| GREEN LED | D4 | D4 | ✅ Correct |
| BLUE LED | D22 | D22 | ✅ Correct |
| YELLOW LED | D23 | D23 | ✅ Correct |
| Button | D14 | D14 | ✅ Correct |
| Light Sensor | D33 | D33 | ✅ Correct |

---

## Exercise-by-Exercise Review

### Exercise 1: Blink RED LED
**Status: ✅ PASS**

**Requirements:**
- Turn RED (D15) ON for 500 ms, then OFF for 500 ms in a loop
- Serial: Print `RED ON` / `RED OFF` whenever it changes

**Implementation Analysis (lines 91-101):**
```cpp
static unsigned long t0 = 0;
static bool state = false;
if (millis() - t0 >= 500) {
    t0 = millis();
    state = !state;
    digitalWrite(PIN_RED, state ? HIGH : LOW);
    Serial.println(state ? F("RED ON") : F("RED OFF"));
}
```

**Findings:**
- ✅ Uses non-blocking timing with `millis()`
- ✅ Correct 500ms interval
- ✅ Proper state toggling
- ✅ Correct serial output format: `RED ON` / `RED OFF`
- ✅ Uses F() macro for memory efficiency

**Strengths:**
- Non-blocking implementation allows for scalability
- Clean and readable code structure
- Memory-efficient string handling

---

### Exercise 2: Button toggles GREEN
**Status: ✅ PASS**

**Requirements:**
- Press BUTTON (D14) to toggle GREEN (D4)
- Serial: Print `GREEN=1` or `GREEN=0` only when the state changes

**Implementation Analysis (lines 103-112):**
```cpp
static bool green = false;
if (edgePressed()) {
    green = !green;
    digitalWrite(PIN_GREEN, green ? HIGH : LOW);
    Serial.print(F("GREEN="));
    Serial.println(green ? 1 : 0);
}
```

**Findings:**
- ✅ Uses debounced edge detection (30ms debounce)
- ✅ Toggles GREEN LED state correctly
- ✅ Prints only on state change (not continuously)
- ✅ Correct output format: `GREEN=1` / `GREEN=0`
- ✅ INPUT_PULLUP configuration (active LOW) handled correctly

**Strengths:**
- Professional debouncing implementation prevents false triggers
- Edge detection ensures printing only on actual button press
- Helper function `edgePressed()` is well-designed and reusable

---

### Exercise 3: Read light sensor
**Status: ✅ PASS**

**Requirements:**
- Every 500 ms, read LIGHT (D33) using `analogRead()`
- Serial: Print the raw value, e.g. `raw=1835`

**Implementation Analysis (lines 114-123):**
```cpp
static unsigned long t0 = 0;
if (millis() - t0 >= 500) {
    t0 = millis();
    int raw = analogRead(PIN_LDR);
    Serial.print(F("raw="));
    Serial.println(raw);
}
```

**Findings:**
- ✅ Non-blocking 500ms timer
- ✅ Uses `analogRead()` as required
- ✅ Correct output format: `raw=XXXX`
- ✅ Proper ESP32 ADC pin (D33) configuration

**Strengths:**
- Simple and effective implementation
- Correct timing interval
- Proper ADC usage for ESP32 (0-4095 range)

---

### Exercise 4: Light sensor -> LED band
**Status: ✅ PASS**

**Requirements:**
- Read LIGHT (D33) and turn ON exactly one LED based on value (0–4095):
  - 0–1023 → BLUE (D22)
  - 1024–2047 → GREEN (D4)
  - 2048–3071 → YELLOW (D23)
  - 3072–4095 → RED (D15)
- Serial: Print `band=BLUE/GREEN/YELLOW/RED`

**Implementation Analysis (lines 125-156):**
```cpp
static unsigned long t0 = 0;
if (millis() - t0 >= 200) {
    t0 = millis();
    int raw = analogRead(PIN_LDR);
    
    allOff();  // Ensures exactly one LED is on
    
    if (raw <= 1023) {
        digitalWrite(PIN_BLUE, HIGH);
        Serial.println(F("band=BLUE"));
    }
    else if (raw <= 2047) {
        digitalWrite(PIN_GREEN, HIGH);
        Serial.println(F("band=GREEN"));
    }
    else if (raw <= 3071) {
        digitalWrite(PIN_YELLOW, HIGH);
        Serial.println(F("band=YELLOW"));
    }
    else {
        digitalWrite(PIN_RED, HIGH);
        Serial.println(F("band=RED"));
    }
}
```

**Findings:**
- ✅ All range boundaries are correct:
  - 0-1023 → BLUE ✅
  - 1024-2047 → GREEN ✅
  - 2048-3071 → YELLOW ✅
  - 3072-4095 → RED ✅
- ✅ `allOff()` ensures exactly one LED is on at a time
- ✅ Correct output format: `band=COLOR`
- ✅ Uses 200ms sampling (faster than 500ms for better responsiveness)

**Strengths:**
- Excellent use of helper function `allOff()` to guarantee mutual exclusivity
- Conditional logic is clear and correct
- Faster sampling rate improves user experience while still being efficient

---

### Exercise 5: Snapshot on button
**Status: ✅ PASS**

**Requirements:**
- Do nothing until BUTTON (D14) is pressed
- On press, read LIGHT (D33) once and print `snapshot=xxxx`
- Flash YELLOW (D23) for 100 ms to acknowledge

**Implementation Analysis (lines 158-169):**
```cpp
if (edgePressed()) {
    int raw = analogRead(PIN_LDR);
    Serial.print(F("snapshot="));
    Serial.println(raw);
    
    digitalWrite(PIN_YELLOW, HIGH);
    delay(100);
    digitalWrite(PIN_YELLOW, LOW);
}
```

**Findings:**
- ✅ Waits for button press (does nothing until pressed)
- ✅ Reads sensor only once per button press
- ✅ Correct output format: `snapshot=XXXX`
- ✅ YELLOW LED flashes for 100ms as acknowledgment
- ✅ Uses debounced edge detection

**Strengths:**
- Event-driven approach (responds only to button press)
- Single reading per press as required
- Visual feedback with LED flash
- Use of `delay(100)` is acceptable here for simple acknowledgment

**Note:**
- The comment mentions "(change 100ms if needed)" - the implementation uses exactly 100ms as specified

---

### Exercise 6: Minimal serial control
**Status: ✅ PASS**

**Requirements:**
- If serial receives a character:
  - `'B'` → turn BLUE (D22) ON
  - `'b'` → turn BLUE (D22) OFF
- Serial: Print `BLUE=1` or `BLUE=0` after each command

**Implementation Analysis (lines 171-189):**
```cpp
static bool blue = false;
if (Serial.available() > 0) {
    char c = (char)Serial.read();
    if (c == 'B') {
        blue = true;
        digitalWrite(PIN_BLUE, HIGH);
        Serial.println(F("BLUE=1"));
    }
    else if (c == 'b') {
        blue = false;
        digitalWrite(PIN_BLUE, LOW);
        Serial.println(F("BLUE=0"));
    }
}
```

**Findings:**
- ✅ Checks for serial data availability
- ✅ Responds correctly to 'B' (uppercase) - turns BLUE ON
- ✅ Responds correctly to 'b' (lowercase) - turns BLUE OFF
- ✅ Correct output format: `BLUE=1` / `BLUE=0`
- ✅ Prints after each valid command
- ✅ Ignores invalid characters (no response)

**Strengths:**
- Simple and effective serial command parsing
- Case-sensitive handling as required
- Silent on invalid input (good UX practice)
- Maintains state correctly

---

## Code Quality Assessment

### Strengths

1. **Architecture:**
   - Excellent use of conditional compilation (`#if LAB_TASK == N`) allows testing each exercise independently
   - Clean separation of concerns with helper functions
   - All exercises are self-contained and don't interfere with each other

2. **Helper Functions:**
   - `allOff()`: Simple and effective for turning off all LEDs
   - `readButtonPressed()`: Abstracts the INPUT_PULLUP logic (pressed = LOW)
   - `edgePressed()`: Professional debouncing implementation with configurable delay

3. **Memory Efficiency:**
   - Consistent use of `F()` macro for string literals saves RAM
   - Static variables used appropriately for state persistence

4. **Timing:**
   - Non-blocking timing using `millis()` (except Exercise 5's simple delay)
   - Appropriate intervals for each task

5. **Serial Communication:**
   - All output formats match requirements exactly
   - Baud rate set to 115200 (appropriate for ESP32)
   - Clear task identification on startup

### Best Practices Observed

- ✅ Proper pin initialization in `setup()`
- ✅ Non-blocking code patterns
- ✅ Debouncing for button inputs
- ✅ Clear variable naming
- ✅ Consistent code style
- ✅ Comments explaining each task
- ✅ INPUT_PULLUP for button (no external resistor needed)

### Minor Observations

1. **Exercise 4 Sampling Rate:**
   - Uses 200ms instead of a longer interval
   - This is actually an improvement for responsiveness
   - Not a requirement violation

2. **Exercise 5 Blocking Delay:**
   - Uses `delay(100)` for YELLOW flash
   - Acceptable for this simple acknowledgment
   - Could be non-blocking but not necessary for this exercise

3. **Exercise 6 State Variable:**
   - Maintains `blue` state variable even though GPIO state could be read
   - This is good practice for clarity

---

## Testing Recommendations

To verify the implementation:

1. **Exercise 1:** Change `LAB_TASK` to 1, compile, upload
   - Observe RED LED blinking at 1Hz
   - Monitor serial output for "RED ON" / "RED OFF"

2. **Exercise 2:** Change `LAB_TASK` to 2, compile, upload
   - Press button and observe GREEN toggle
   - Verify serial prints only on state change

3. **Exercise 3:** Change `LAB_TASK` to 3, compile, upload
   - Monitor serial for "raw=XXXX" every 500ms
   - Vary light conditions to see different values

4. **Exercise 4:** Change `LAB_TASK` to 4, compile, upload
   - Cover/uncover light sensor
   - Observe different LEDs lighting based on light level
   - Verify only one LED is on at a time

5. **Exercise 5:** Change `LAB_TASK` to 5, compile, upload
   - Press button and observe YELLOW flash
   - Verify "snapshot=XXXX" printed once per press

6. **Exercise 6:** Change `LAB_TASK` to 6, compile, upload
   - Send 'B' via serial monitor → BLUE should turn ON
   - Send 'b' via serial monitor → BLUE should turn OFF
   - Verify "BLUE=1" / "BLUE=0" responses

---

## Conclusion

**Final Verdict: ✅ ALL REQUIREMENTS MET**

The solution code demonstrates:
- Complete understanding of all 6 exercises
- Correct implementation of all requirements
- Good coding practices and Arduino/ESP32 knowledge
- Professional-quality code structure
- Proper error handling and edge cases

**No issues or corrections needed.** The student has successfully completed all Lab 1 exercises with high-quality implementations.

---

## Grading Recommendation

| Exercise | Status | Score |
|----------|--------|-------|
| Exercise 1: Blink RED LED | ✅ Pass | 100% |
| Exercise 2: Button toggles GREEN | ✅ Pass | 100% |
| Exercise 3: Read light sensor | ✅ Pass | 100% |
| Exercise 4: Light sensor -> LED band | ✅ Pass | 100% |
| Exercise 5: Snapshot on button | ✅ Pass | 100% |
| Exercise 6: Minimal serial control | ✅ Pass | 100% |

**Overall: 100% - Excellent Work!**

---

*This review was conducted by analyzing the source code against the exercise requirements without running the actual hardware.*
