#include "Console.h"

QString Console::Read()
{
	return QString();
}

void Console::Write(QString str)
{

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