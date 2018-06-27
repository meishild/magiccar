#include "SerialCommands.h"

void SerialCommands::AddCommand(SerialCommand *command)
{
#ifdef SERIAL_COMMANDS_DEBUG
	Serial.print("Adding #");
	Serial.print(commands_count_);
	Serial.print(" cmd=[");
	Serial.print(command->command);
	Serial.println("]");
#endif
	command->next = NULL;
	if (commands_head_ == NULL)
	{
		commands_head_ = commands_tail_ = command;
	}
	else
	{
		commands_tail_->next = command;
		commands_tail_ = command;
	}
	commands_count_++;
}

SERIAL_COMMANDS_ERRORS SerialCommands::ReadSerial()
{
	if (serial_ == NULL)
	{
		return SERIAL_COMMANDS_ERROR_NO_SERIAL;
	}

	while (serial_->available() > 0)
	{
		int ch = serial_->read();
#ifdef SERIAL_COMMANDS_DEBUG
		Serial.print("Read: bufLen=");
		Serial.print(buffer_len_);
		Serial.print(" bufPos=");
		Serial.print(buffer_pos_);
		if (ch < 32)
		{
			Serial.print(" ch=#");
			Serial.print(ch);
		}
		else
		{
			Serial.print(" ch=[");
			Serial.print((char)ch);
			Serial.print("]");
		}
		Serial.println();
#endif
		if (ch <= 0)
		{
			continue;
		}

		if (buffer_pos_ < buffer_len_)
		{
			buffer_[buffer_pos_++] = ch;
		}
		else
		{
#ifdef SERIAL_COMMANDS_DEBUG
			Serial.println("Buffer full");
#endif
			return SERIAL_COMMANDS_ERROR_BUFFER_FULL;
		}
		bool equal = false;
		if (buffer_pos_ >= term_size_)
		{
			for (int i = 0; i < term_size_; i++)
			{
				if (term_[i] != buffer_[buffer_pos_ - term_size_ + i])
				{
					equal = false;
					continue;
				}
				{
					equal = true;
				}
			}
		}

		if (equal)
		{
			buffer_[buffer_pos_ - strlen(term_)] = '\0';

#ifdef SERIAL_COMMANDS_DEBUG
			Serial.print("Received: [");
			Serial.print(buffer_);
			Serial.println("]");
#endif
			char *command = strtok_r(buffer_, delim_, &last_token_);
			if (command != NULL)
			{
				boolean matched = false;
				int cx;
				SerialCommand *cmd;
				for (cmd = commands_head_, cx = 0; cmd != NULL; cmd = cmd->next, cx++)
				{
#ifdef SERIAL_COMMANDS_DEBUG
					Serial.print("Comparing [");
					Serial.print(command);
					Serial.print("] to [");
					Serial.print(cmd->command);
					Serial.println("]");
#endif

					if (strncmp(command, cmd->command, strlen(cmd->command) + 1) == 0)
					{
#ifdef SERIAL_COMMANDS_DEBUG
						Serial.print("Matched #");
						Serial.println(cx);
#endif
						cmd->function(this);
						matched = true;
						break;
					}
				}
				if (!matched && default_handler_ != NULL)
				{
					(*default_handler_)(this, command);
				}
			}

			ClearBuffer();
		}
	}

	return SERIAL_COMMANDS_SUCCESS;
}

Stream *SerialCommands::GetSerial()
{
	return serial_;
}

void SerialCommands::AttachSerial(Stream *serial)
{
	serial_ = serial;
}

void SerialCommands::DetachSerial()
{
	serial_ = NULL;
}

void SerialCommands::SetDefaultHandler(void (*function)(SerialCommands *, const char *))
{
	default_handler_ = function;
}

void SerialCommands::ClearBuffer()
{
	buffer_[0] = '\0';
	buffer_pos_ = 0;
}

char *SerialCommands::Next()
{
	return strtok_r(NULL, delim_, &last_token_);
}
