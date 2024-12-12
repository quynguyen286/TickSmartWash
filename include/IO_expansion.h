#ifdef IO_EXPANSION_H
#define I0_EXPANSION_H

void mcp_init();
void mcp_input_pin_set(int num);
void mcp_output_pin_set(int num);
void mcp_output_high(int num);
void mcp_output_low(int num);
int read_input_pin(int num);

#endif