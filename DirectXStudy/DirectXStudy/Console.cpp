#include "Console.h"

String Console::Read()
{
	String str;
	unsigned long readCount = 0;
	wchar_t *inputBuffer = m_instance->m_inputBuffer;
	while (true)
	{
		ReadConsoleW(m_instance->m_stdInput, inputBuffer, InputBufferLength, &readCount, nullptr);
		if (inputBuffer[readCount - 1] == u'\n')
		{
			str.append(reinterpret_cast<char16_t*>(inputBuffer), readCount - 2);
			break;
		}
		else
			str.append(reinterpret_cast<char16_t*>(inputBuffer), readCount);
	}
	return str;
}

void Console::Write(const String &str)
{
	WriteConsoleW(m_instance->m_stdOutput, str.data(), str.length(), nullptr, nullptr);
}

void Console::Init()
{
	if (m_instance == nullptr)
		m_instance = new Console();
}

void Console::Destroy()
{
	delete m_instance;
	m_instance = nullptr;
}

Console::Console()
{
	AllocConsole();

	m_stdInput = GetStdHandle(STD_INPUT_HANDLE);
	m_stdOutput = GetStdHandle(STD_OUTPUT_HANDLE);
}

Console::~Console()
{
	FreeConsole();
}

Console* Console::m_instance = nullptr;