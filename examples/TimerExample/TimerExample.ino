#include <Adafruit_MCP23X17.h>        // For MCP23017 I/O buffers
#include <OXRS_Output.h>              // For output handling

// I/O buffers
Adafruit_MCP23X17 mcp23017;

// Input handlers
OXRS_Output oxrsOutput;

uint32_t timer;

void setup() {
  // Initialise serial for debug output
  Serial.begin(115200);

  // Initialise the MCP chip (assume at address 0x20)
  mcp23017.begin_I2C(0x20);

  // Set every pin to OUTPUT
  for (uint8_t pin = 0; pin < 16; pin++) {
    mcp23017.pinMode(pin, OUTPUT);
  }

  // Initialise our output handler
  oxrsOutput.begin(outputEvent);

  // Set pin 0 to have a 2s timer
  oxrsOutput.setType(0, TIMER);
  oxrsOutput.setTimer(0, 2);
}

void loop() {
  // Check for any output events
  oxrsOutput.process();

  // Turn on output every 5s to show timer triggering
  if ((millis() - timer) > 5000) {
    Serial.println("Turning ON output 0...");
    
    oxrsOutput.handleCommand(0, 0, RELAY_ON);

    timer = millis();
  }
}

void outputEvent(uint8_t id, uint8_t output, uint8_t type, uint8_t state) {
  char outputType[8];
  getOutputType(outputType, type);
  char eventType[7];
  getEventType(eventType, type, state);

  Serial.print(F("[EVENT]"));
  Serial.print(F(" OUTPUT:"));
  Serial.print(output);
  Serial.print(F(" TYPE:"));
  Serial.print(outputType);
  Serial.print(F(" EVENT:"));
  Serial.println(eventType);
}

void getOutputType(char outputType[], uint8_t type)
{
  // Determine what type of output we have
  sprintf_P(outputType, PSTR("ERROR"));
  switch (type)
  {
    case MOTOR:
      sprintf_P(outputType, PSTR("MOTOR"));
      break;
    case RELAY:
      sprintf_P(outputType, PSTR("RELAY"));
      break;
    case TIMER:
      sprintf_P(outputType, PSTR("TIMER"));
      break;
  }
}

void getEventType(char eventType[], uint8_t type, uint8_t state)
{
  // Determine what event we need to publish
  sprintf_P(eventType, PSTR("ERROR"));
  switch (state)
  {
    case RELAY_ON:
      sprintf_P(eventType, PSTR("ON"));
      break;
    case RELAY_OFF:
      sprintf_P(eventType, PSTR("OFF"));
      break;
  }
}