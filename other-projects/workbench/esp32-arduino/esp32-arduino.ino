#include <Wire.h>
#include <Adafruit_MCP23X17.h>

Adafruit_MCP23X17 mcp;

// The component is called 74HC595. 
// But variables cannot start with a number :)
#define U74HC_SI       19
#define U74HC_RCK      21
#define U74HC_SCK      22
#define ActivateRelays 23

#define SDA            32
#define SCL            33


bool RelayStatus[8];
bool PreviousButtonStatus[8];

void setup()
{
  Serial.begin(115200);
  serialPrint("Initializing");

  pinMode(ActivateRelays, OUTPUT);
  digitalWrite(ActivateRelays, LOW);

  // Shift register
  pinMode(U74HC_SCK, OUTPUT);
  pinMode(U74HC_RCK, OUTPUT);
  pinMode(U74HC_SI, OUTPUT);

  digitalWrite(U74HC_SCK, LOW);
  digitalWrite(U74HC_RCK, LOW);
  digitalWrite(U74HC_SI, LOW);
  //

  // I2C expander
  Wire.setClock(20000);
  Wire.begin(SDA, SCL); // wake up I2C bus
  try {
    if (!mcp.begin_I2C()) {
      serialPrint("Error. I2C not initialized");
      //while (1);
    }
  }catch (String error){
    serialPrint(error);
  }

  for (int i = 0; i < 16; i++ ) {
    mcp.pinMode(i, INPUT);
  }

  // Before activating the relays, let's give some time to the input pins to estabilize
  serialPrint("Delaying relays activation");
  delay(3000);
  digitalWrite(ActivateRelays, HIGH);
  serialPrint("Relays activated!");
}

void loop()
{
  int buttonValue;
  // I2C
  for(int i = 0; i < 8; i++)
  {
    buttonValue = mcp.digitalRead(i);

    // This logic makes sure we change the state of the relay only once if the button is holded
    if(!buttonValue)
    {
      PreviousButtonStatus[i] = false;
    }
    else if(!PreviousButtonStatus[i])
    {
      // Button is now HIGH and previously was LOW, so change the relay state
      RelayStatus[i] = !RelayStatus[i];
      PreviousButtonStatus[i] = true;
    }
    
  }
  writeRelaysStatus();
  delay(10);
}

void writeRelaysStatus()
{
  // Latching
  digitalWrite(U74HC_RCK, LOW);

  // First 8 bits are for the relays, sencond time are for the button LED
  for(int j = 0; j < 2; j++)
  {
    for(int i = 0; i < 8; i++)
    {
      digitalWrite(U74HC_SCK, LOW);
      digitalWrite(U74HC_SI, RelayStatus[i]);
      digitalWrite(U74HC_SCK, HIGH);
    }
  }

  // Unlatching
  digitalWrite(U74HC_RCK, HIGH);
}

void serialPrint(String msg)
{
  Serial.print("v9:");
  Serial.println(msg);
}
