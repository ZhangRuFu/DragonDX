#pragma once

class Screen
{
private:
	static Screen *m_instance;

private:
	int m_width;
	int m_height;

private:
	Screen(int width, int height);

public:
	static int GetScreenWidth(void);
	static int GetScreenHeight(void);
	static void Init(int width, int height);
};