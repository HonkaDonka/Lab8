#include "env.h"
#include "blynk.h"
#include "oled-wing-adafruit.h"
#include "SparkFun_VCNL4040_Arduino_Library.h"

SYSTEM_THREAD(ENABLED);

#define DISTANCEPIN V0
#define LIGHTPIN V1
#define ORANGEPIN D7
#define BLUEPIN D6
#define GREENPIN D5

OledWingAdafruit display;
VCNL4040 VCNLSensor;

int displaymode = 0;

void setup()
{
  Wire.begin();

  VCNLSensor.begin();
  VCNLSensor.powerOnAmbient();
  VCNLSensor.powerOnProximity();

  display.setup();
  display.clearDisplay();
  display.display();
  display.setTextSize(1);
  display.setTextColor(WHITE);

  Blynk.begin(BLYNK_AUTH_TOKEN);

  pinMode(DISTANCEPIN, OUTPUT);
  pinMode(LIGHTPIN, OUTPUT);
  pinMode(ORANGEPIN, OUTPUT);
  pinMode(BLUEPIN, OUTPUT);
  pinMode(GREENPIN, OUTPUT);
}

void loop()
{
  display.loop();
  Blynk.run();

  unsigned int proxVal = VCNLSensor.getProximity();
  unsigned int ambVal = VCNLSensor.getAmbient();

  // Sets what the OLED wing will display
  if (display.pressedA())
  {
    displaymode = 0;
  }
  else if (display.pressedB())
  {
    displaymode = 1;
  }
  else if (display.pressedC())
  {
    displaymode = 2;
  }

  // Switch cases instead of if statements so I don't have to write as much
  switch (displaymode)
  {
  case 0:
    displayTextresetPins("PROXIMITY: ", proxVal);
    break;
  case 1:
    displayTextresetPins("AMBIENCE: ", ambVal);
    break;
  case 2:
    resetDisplay();
    Blynk.virtualWrite(DISTANCEPIN, proxVal);
    Blynk.virtualWrite(LIGHTPIN, ambVal);
    display.display();
    break;
  }

  // Determines which LED to turn on based on proxVal
  if (proxVal < 30)
  {
    resetLEDs();
    digitalWrite(GREENPIN, HIGH);
  }
  else if (proxVal < 300)
  {
    resetLEDs();
    digitalWrite(BLUEPIN, HIGH);
  }
  else if (proxVal < 12000)
  {
    resetLEDs();
    digitalWrite(ORANGEPIN, HIGH);
  }
}

void resetDisplay()
{
  display.clearDisplay();
  display.setCursor(0, 0);
}

void resetLEDs()
{
  digitalWrite(GREENPIN, LOW);
  digitalWrite(BLUEPIN, LOW);
  digitalWrite(ORANGEPIN, LOW);
}

void displayTextresetPins(String text, int pin)
{
  resetDisplay();
  Blynk.virtualWrite(DISTANCEPIN, 0);
  Blynk.virtualWrite(LIGHTPIN, 0);
  display.print(text);
  display.println(pin);
  display.display();
}