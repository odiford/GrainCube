#include <Encoder.h>

byte byte1; //
byte byte2;
byte byte3;

int inChannel;
int channel;
int state;

int preset = 0;
int pChannel = 16;

int aChannel = 1;
int bChannel = 2;
int a = 21;
int b = 22;
int aState = 0;
int bState = 0;
int lastAState = 0;
int lastBState = 0;

int dUp = 5;
int dDown = 6;
int dUpState = 0;
int dDownState = 0;
int lastUpState = 0;
int lastDownState = 0;

int leftChannel = 7;
int rightChannel = 8;
int dLeft = 27;
int dRight = 28;
int dLeftState = 0;
int dRightState = 0;
int lastLeftState = 0;
int lastRightState = 0;

int lChannel = 9;
int rChannel = 10;
int l = 29;
int r = 30;
int lState = 0;
int rState = 0;
int lastLState = 0;
int lastRState = 0;

int xChannel = 3;
int yChannel = 4;
int zChannel = 13;
int x = 23;
int y = 24;
int z = 33;
int xState = 0;
int yState = 0;
int zState = 0;
int lastXState = 0;
int lastYState = 0;
int lastZState = 0;



unsigned long lastPrintTime = 0;

Encoder myEnc(2, 3);

int newPosition = 0;
int oldPosition = 0;
int usefulVal = 0;

int volChannel = 14;
int volPin = A9;
int vol;
int lastVol;

// For every LED, ground blue pin, red pin is first(0), green pin is second(1).
//int ledPins[5][2] = {{5,6},{7,8},{9,10},{11,12},{14,15}};
int ledPins[5][2] = {{14,15},{11,12},{9,10},{7,8},{5,6}};

void setup()
{
  Serial1.begin(38400);
  //Serial.begin(9600);

  pinMode(volPin, INPUT);

  for(int i = 4; i < 16; i++)
  {
    pinMode(i,OUTPUT);
  }

}

void loop()
{
  serialProcess();
  printData();
  ledSelect(preset);
}


void serialProcess() // Reads in and handles incoming Arduino Data
{
  
if (Serial1.available() >= 3)
  {
    byte1 = Serial1.read();

    if (byte1 & B10000000) 
    {
      byte1 = byte1 - B10000000;
      byte2 = Serial1.read();
      byte3 = Serial1.read();
      
      inChannel = byte1;
      channel = byte1;
      if (channel > 16)
      {
        channel = 15;
      }
      
      state = byte2 + byte3;

      buttonProcess();

    }
  }
  
 

}


/*NOTE ON RGB LEDS
 * 
 * To have 5 RGB LEDs you would need 15 pins which we cannot spare. 
 * However, we only need red and green and they only every need to be full on or full off.
 * This means that we can always ground the blue pin, and run the red and green off of digital write.
 * 
 */
void ledSelect(int led) 
{
  for(int i=0; i<5; i++)
  {
    if(i == led) //Make it green
    {
      digitalWrite(ledPins[i][1], HIGH);
      digitalWrite(ledPins[i][0], LOW);
    }
    else //Make it red
    {
      digitalWrite(ledPins[i][1], LOW);
      digitalWrite(ledPins[i][0], HIGH);
    }
  }
}


void printData()  // sends data into Max every millisecond
{
  if(lastPrintTime < (millis() - 1))
  {
      lastPrintTime = millis();
      usbMIDI.sendControlChange(byte3, byte2, channel);
      checkEncoder();
      checkVol();
  }
}


void buttonProcess()
{
  ////////////////////////////A and B Buttons
  if(inChannel == a)
  {
    lastAState = aState;
    aState = state;
 
    if (aState == HIGH && lastAState == LOW)   //if button has been pressed, change note state
    {
      usbMIDI.sendControlChange(0, 1, aChannel);
    }
    else if(aState == LOW && lastAState == HIGH) 
    {
      usbMIDI.sendControlChange(0, 0, aChannel);
    }
 
    
  }
  else if(inChannel == b)
  {
    lastBState = bState;
    bState = state;
 
    if (bState == HIGH && lastBState == LOW)   //if button has been pressed, change note state
    {
      usbMIDI.sendControlChange(0, 1, bChannel);
    }
    else if(bState == LOW && lastBState == HIGH) 
    {
      usbMIDI.sendControlChange(0, 0, bChannel);
    }
  }
  

   ///////////////////////////D-Pad 
   if(channel == dUp)
  {
    lastUpState = dUpState;
    dUpState = state;
 
    if (dUpState == HIGH && lastUpState == LOW)   //if button has been pressed, change note state
    {
      preset = preset + 1;
      if(preset > 4)
      {
        preset = 0;
      }
      usbMIDI.sendControlChange(0, preset, pChannel);
    }
    
  }
  else if(channel == dDown)
  {
    lastDownState = dDownState;
    dDownState = state;
 
    if (dDownState == HIGH && lastDownState == LOW)   //if button has been pressed, change note state
    {
      preset = preset - 1;
      if(preset < 0)
      {
        preset = 4;
      }
      usbMIDI.sendControlChange(0, preset, pChannel);
    }
  
  }
  if(inChannel == dLeft)
  {
    
    lastLeftState = dLeftState;
    dLeftState = state;
 
    if (dLeftState == HIGH && lastLeftState == LOW)   //if button has been pressed, change note state
    {
      usbMIDI.sendControlChange(0, 1, leftChannel);
    }
    else if(dLeftState == LOW && lastLeftState == HIGH) 
    {
      usbMIDI.sendControlChange(0, 0, leftChannel);
    }
  }
  else if(inChannel == dRight)
  {
    lastRightState = dRightState;
    dRightState = state;
 
    if (dRightState == HIGH && lastRightState == LOW)   //if button has been pressed, change note state
    {
      usbMIDI.sendControlChange(0, 1, rightChannel);
    }
    else if(dRightState == LOW && lastRightState == HIGH) 
    {
      usbMIDI.sendControlChange(0, 0, rightChannel);
    }
  }
  
  ////////////////////////////Left and Right Bumpers
  if(inChannel == l)
  {
    lastLState = lState;
    lState = state;
 
    if (lState == HIGH && lastLState == LOW)   //if button has been pressed, change note state
    {
      usbMIDI.sendControlChange(0, 1, lChannel);
    }
    else if(lState == LOW && lastLState == HIGH) 
    {
      usbMIDI.sendControlChange(0, 0, lChannel);
    }
 
    
  }
  else if(inChannel == r)
  {
    lastRState = rState;
    rState = state;
 
    if (rState == HIGH && lastRState == LOW)   //if button has been pressed, change note state
    {
      usbMIDI.sendControlChange(0, 1, rChannel);
    }
    else if(rState == LOW && lastRState == HIGH) 
    {
      usbMIDI.sendControlChange(0, 0, rChannel);
    }
  }


  /////////////////////////// X, Y, and Z Buttons
  if(inChannel == x)
  {
    lastXState = xState;
    xState = state;
 
    if (xState == HIGH && lastXState == LOW)   //if button has been pressed, change note state
    {
      usbMIDI.sendControlChange(0, 1, xChannel);
    }
    else if(xState == LOW && lastXState == HIGH) 
    {
      usbMIDI.sendControlChange(0, 0, xChannel);
    }
 
    
  }
  else if(inChannel == y)
  {
    lastYState = yState;
    yState = state;
 
    if (yState == HIGH && lastYState == LOW)   //if button has been pressed, change note state
    {
      usbMIDI.sendControlChange(0, 1, yChannel);
    }
    else if(yState == LOW && lastYState == HIGH) 
    {
      usbMIDI.sendControlChange(0, 0, yChannel);
    }
  }
  else if(inChannel == z)
  {
    lastZState = zState;
    zState = state;
 
    if (zState == HIGH && lastZState == LOW)   //if button has been pressed, change note state
    {
      usbMIDI.sendControlChange(0, 1, zChannel);
    }
    else if(zState == LOW && lastZState == HIGH) 
    {
      usbMIDI.sendControlChange(0, 0, zChannel);
    }
  }

}


void checkEncoder()  // Tracks encoder position and updates preset value and Max if it changes
{
  newPosition = myEnc.read();
  if (newPosition >= oldPosition + 4)
  {
    oldPosition = newPosition;
    usefulVal = oldPosition/4;
    preset = preset - 1;  
    if(preset < 0)
    {
      preset = 4;
    }

    usbMIDI.sendControlChange(0, preset, pChannel);
  } 
  else if(newPosition <= oldPosition - 4) 
  {
    oldPosition = newPosition;
    usefulVal = oldPosition/4;
    preset++;
    if(preset > 4)
    {
      preset = 0;
    }
    usbMIDI.sendControlChange(0, preset, pChannel);
  }
}

void checkVol() // Reads volume knob and sends value to max
{
  vol = map(analogRead(volPin), 0,1023,0,127);
  if(vol != lastVol)
  {
    usbMIDI.sendControlChange(0, vol, volChannel);
    lastVol = vol;
  }
}




