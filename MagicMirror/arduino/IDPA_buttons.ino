#include <Keyboard.h>
#include <KeyboardLayout.h>

// Definition welcher Inputpin für was ist
#define NEXT_FILTER 2
#define PREV_FILTER 3
#define CYCLE_FILTERS 5

// Definition welcher Keycode an den PC gesendet werden soll
#define NEXT_KEY 'n'
#define PREV_KEY 'p'
#define CYCLE_KEY 'c'

// Makro für die Signalauswertung. HIGH = gedrückt | LOW = nicht gedrückt
#define IS_PRESSED(x) digitalRead(x) == HIGH
#define IS_RELEASED(x) digitalRead(x) == LOW

int debounce = 250;
int pressedButton = 0;

// USB verbindung starten & verwendete Pins auf Inputmodus schalten
void setup() {
  Serial.begin(9600);
  Keyboard.begin();

  pinMode(NEXT_FILTER, INPUT);
  pinMode(PREV_FILTER, INPUT);
  pinMode(CYCLE_FILTERS, INPUT);

}


// Signal von den Tastern erfassen
void loop() {

  int delayTime = 10;

  if (pressedButton == 0) {
    if (IS_PRESSED(NEXT_FILTER)) {
      Keyboard.write(NEXT_KEY);
      pressedButton = NEXT_FILTER;
    }
    else if (IS_PRESSED(PREV_FILTER)) {
      Keyboard.write(PREV_KEY);
      pressedButton = PREV_FILTER;
    }
    else if (IS_PRESSED(CYCLE_FILTERS)) {
      Keyboard.write(CYCLE_KEY);
      pressedButton = CYCLE_FILTERS;
    }
  }
  else {
    if (IS_RELEASED(pressedButton)) {
      pressedButton = 0;
      delayTime = debounce;
    }
  }

  delay(delayTime);
}
