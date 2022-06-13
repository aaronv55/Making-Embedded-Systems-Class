// Console IO is a wrapper between the actual in and output and the console code

#include "consoleIo.h"
#include <stdio.h>

eConsoleError ConsoleIoInit(void)
{
	return CONSOLE_SUCCESS;
}

// This is modified for the STM32F410R8T6
eConsoleError ConsoleIoReceive(uint8_t *buffer, const uint32_t bufferLength, uint32_t *readLength)
{
	uint32_t i = 0;
	char ch;

	while (uart1_is_readable())
	{
		ch = uart1_receive_byte();
		uart1_send_byte((uint8_t) ch); // echo
		buffer[i] = (uint8_t) ch;

		i++;
	}

	*readLength = i;
	return CONSOLE_SUCCESS;
}

eConsoleError ConsoleIoSendString(const char *buffer)
{
	uart1_printf(buffer);
	return CONSOLE_SUCCESS;
}

