
#include "Nintendo.h"

// Define a Gamecube Controller
CGamecubeController GamecubeController(7);

// Pin definitions
#define pinLed LED_BUILTIN

void setup()
{
  pinMode(pinLed, OUTPUT);
  Serial.begin(38400);
}

void loop()
{
  // Try to read the controller data
  if (GamecubeController.read())
  {
    auto status = GamecubeController.getStatus();
    auto report = GamecubeController.getReport();
    process();

    // Rumble if button "A" was pressed
    if (report.a) {
      GamecubeController.setRumble(true);
    }
    else {
      GamecubeController.setRumble(false);
    }
  }
  else
  {
    // Add debounce if reading failed
    Serial.println(F("Error reading Gamecube controller."));
    digitalWrite(pinLed, HIGH);
    delay(1000);
  }
  digitalWrite(pinLed, LOW);

}

void process()
{

  if (GamecubeController.read())
  {
    // Print Controller information
    auto status = GamecubeController.getStatus();
    auto report = GamecubeController.getReport();

    noteSend(21,report.a);
    noteSend(22,report.b);
    noteSend(23,report.x);
    noteSend(24,report.y);
    noteSend(5,report.dup);
    noteSend(6,report.ddown);
    noteSend(27,report.dleft);
    noteSend(28,report.dright);
    noteSend(33,report.z);
    stickSend(11,report.yAxis);
    stickSend(12,report.cyAxis);

    if(report.left >= 255)
    {
      noteSend(29,1);
    }
    else
    {
      noteSend(29,0);
    }
    if(report.right >= 255)
    {
      noteSend(30,1);
    }
    else
    {
      noteSend(30,0);
    }

  }
  else
  {
    // Add debounce if reading failed
    Serial.println(F("Error reading Gamecube controller."));
    digitalWrite(pinLed, HIGH);
    delay(1000);
  }
}

void noteSend(int channel, int stat)
{
  Serial.write(channel + B10000000);
  Serial.write(stat);
  Serial.write(0);
}

void stickSend(int channel, int val)
{
  int byte2val;
  int byte3val;
  if(val > 127)
  {
    byte2val = 127;
    byte3val = val - 127;
  }
  else
  {
    byte2val = val;
    byte3val = 0;
  }

  Serial.write(channel + B10000000);
  Serial.write(byte2val);
  Serial.write(byte3val);
}

