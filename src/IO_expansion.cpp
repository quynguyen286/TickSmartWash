#include <MCP23017.h>
#include <Arduino.h>

MCP23017 mcp = MCP23017(0x20);

void mcp_init()
{
    mcp.init();
    Wire.begin();
    Serial.begin(9600);
}

void mcp_input_pin_set(int num)
{
    mcp.pinMode(num, INPUT_PULLUP, false);
}

void mcp_output_pin_set(int num)
{
    mcp.pinMode(num, OUTPUT, false);
}

void mcp_output_high(int num)
{
    mcp.digitalWrite(num, HIGH);
}

void mcp_output_low(int num)
{
    mcp.digitalWrite(num, LOW);
}

int read_input_pin(int num)
{
    return mcp.digitalRead(num);
}
	
