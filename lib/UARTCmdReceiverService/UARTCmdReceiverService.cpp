#include "UARTCmdReceiverService.h"

UARTCmdReceiverService::UARTCmdReceiverService(Stream *pIORawStream)
	: m_TextMessage(pIORawStream), m_paraStream(sCmdBuffer, COMMAND_PARAMETER_BUFFER_SIZE, 0)
{
}

void UARTCmdReceiverService::populate()
{
	if (m_TextMessage.available())
	{
		// Command Format
		// <COMMAND_ID>[<SPACE><COMMAND_PARAMETER>]<CR><LF>

		unsigned int cmdId = (unsigned int)m_TextMessage.parseULong();

		if (cmdId != SYS_INVALID_COMMAND)
		{
			invalidateCommand();
			m_commandId = cmdId;

			int c = 0;
			bool bPreviousCharIsCR = false;

			// Use \r\n as the terminator of the command line.
			while (m_TextMessage.available())
			{
				c = m_TextMessage.read();
				m_paraStream.write(c);
				if ('\r' == c)
				{
					bPreviousCharIsCR = true;
				}
				else if ('\n' == c)
				{
					if (bPreviousCharIsCR)
					{
						break;
					}
				}
			}
		}

		m_TextMessage.invalidate();
	}
}

unsigned int UARTCmdReceiverService::getCommandId()
{
	return m_commandId;
}

bool UARTCmdReceiverService::hasValidCommand()
{
	return m_commandId != SYS_INVALID_COMMAND;
}

void UARTCmdReceiverService::invalidateCommand()
{
	m_commandId = SYS_INVALID_COMMAND;
	m_paraStream.flush(); // Discard all the data.
}