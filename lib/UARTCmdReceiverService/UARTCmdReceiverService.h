/**
=Feature=

Receive the text message from the stream which is a UART port generally,
 and populate it as the command.

The command is based on the text message protocol. The whole command is transferred
by ASCII chars. 

@example
The two messages below indicate the same command:

	"0x0A 23 abc\r\n"
	"10 23 abc\r\n"

The data process workflow via UART is:

<incoming data> --> UART --> ERxTextMessage --> UARTCmdReceiverService

=Dependency=

 * Hardwares
  * Arduino resource
     * Serial port (UART)

 * Softwares
  * Consumed libraries
     * ERxTextMessage

@class ERxUARTCmdReceiverService
**/
#pragma once
#ifndef UART_CMD_RECEIVER_SERVICE_H
#define UART_CMD_RECEIVER_SERVICE_H

#include <ERxTextMessage.h>
#include <ERxIOStream.h>

#define COMMAND_PARAMETER_BUFFER_SIZE 128
#define SYS_INVALID_COMMAND 0x00

static uint8_t sCmdBuffer[COMMAND_PARAMETER_BUFFER_SIZE];

class UARTCmdReceiverService
{
  public:
	/**
	@constructor
	@param {Stream} pIORawStream this is the UART port generally.
	**/
	UARTCmdReceiverService(Stream *pIORawStream);

  public:
	/**
	Read the incoming data from stream and generate the new command to context.

	The schema of the command is like:
	<CMD_ID> <CMD_DATA><CR><LF>

	@method Populate
	**/
	void populate();

	unsigned int getCommandId();

	bool hasValidCommand();

	void invalidateCommand();

	ERxIOStream getCommandParameterStream();

  private:
	ERxTextMessage m_TextMessage;

	unsigned int m_commandId;

	ERxIOStream m_paraStream;
};

#endif // ERX_UART_CMD_RECEIVER_SERVICE_H