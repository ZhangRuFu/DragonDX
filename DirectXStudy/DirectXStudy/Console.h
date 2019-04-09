#pragma once
#include <Windows.h>
#include <QtCore\qstring.h>

class Console
{
private:
	HANDLE m_stdInput;
	HANDLE m_stdOutput;
	static Console *m_instance;

private:
	Console();
	~Console();

public:
	static void Init();
	static void Destroy();
	static void Write(QString str);
	static QString Read();
};