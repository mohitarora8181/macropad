#include <BleKeyboard.h>
#include <Keypad.h>
#include <RotaryEncoder.h>

BleKeyboard bleKeyboard("Mohit8181");

#define ROW_NUM 3
#define COLUMN_NUM 2

#define PIN_IN1 21
#define PIN_IN2 19
#define PIN_IN3 22  // Mute

char keys[ROW_NUM][COLUMN_NUM] = {
  { KEY_LEFT_ARROW, KEY_RIGHT_ARROW },
  { KEY_UP_ARROW, KEY_TAB },
  { KEY_DOWN_ARROW, KEY_NUM_ENTER }
};

byte pin_rows[ROW_NUM] = { 18, 5, 17 };
byte pin_column[COLUMN_NUM] = { 2, 4 };

Keypad keypad = Keypad(makeKeymap(keys), pin_rows, pin_column, ROW_NUM, COLUMN_NUM);

RotaryEncoder encoder(PIN_IN1, PIN_IN2, RotaryEncoder::LatchMode::TWO03);

void setup() {
  Serial.begin(115200);
  Serial.println("Starting BLE work!");
  bleKeyboard.begin();

  pinMode(PIN_IN3, INPUT_PULLUP);
}

void loop() {

  if (bleKeyboard.isConnected()) {
    char key = keypad.getKey();
    if (key) {
      bleKeyboard.press(key);
      delay(200);
      bleKeyboard.release(key);
    }

    static int pos = 0;
    encoder.tick();

    int newPos = encoder.getPosition();
    if (pos != newPos) {
      if ((int)(encoder.getDirection()) == 1) {
        bleKeyboard.press(KEY_MEDIA_VOLUME_UP);
        delay(100);
        bleKeyboard.release(KEY_MEDIA_VOLUME_UP);
      } else {
        bleKeyboard.press(KEY_MEDIA_VOLUME_DOWN);
        delay(100);
        bleKeyboard.release(KEY_MEDIA_VOLUME_DOWN);
      }
      pos = newPos;
    }

    if (digitalRead(PIN_IN3) == LOW) {
      bleKeyboard.write(KEY_MEDIA_MUTE);
      delay(1000);
    }
  }
}
