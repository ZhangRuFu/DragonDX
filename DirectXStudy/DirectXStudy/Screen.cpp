#include "Screen.h"

Screen::Screen(int width, int height)
{
	m_width = width;
	m_height = width;
}

void Screen::Init(int width, int height)
{
	if (m_instance == nullptr)
		m_instance = new Screen(width, height);
}

int Screen::GetScreenWidth(void) { return m_instance->m_width; }
int Screen::GetScreenHeight(void) { return m_instance->m_height; }

Screen* Screen::m_instance = nullptr;
