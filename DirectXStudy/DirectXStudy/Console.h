#pragma once
#include <Windows.h>
#include <string>
#include "Common.h"

class Console
{
private:
	HANDLE m_stdInput;
	HANDLE m_stdOutput;
	static const int InputBufferLength = 64;
	wchar_t m_inputBuffer[InputBufferLength];
	static Console *m_instance;

private:
	Console();
	~Console();

public:
	static void Init();
	static void Destroy();
	static void Write(const String &str);
	static String Read();
};