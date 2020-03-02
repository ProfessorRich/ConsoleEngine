#pragma once

#include <windows.h>
#include "GlobalConstants.h"

/* #include <tchar.h> - only needed if I switch to unicode (non A versions) of windows.h operations */

class ConsoleEngine {

private:
	const SMALL_RECT m_consoleSize = { 0, 0, g_consoleWidth, g_consoleHeight };  // this is used in SetConsoleWindowInfo, but I'm not sure it does anything...
	
	HANDLE m_currentConsole, m_keyboard;
	DWORD m_written;
	COORD m_currentCursorPosition;
	WORD m_textColour, m_backgroundColour;

public:
	ConsoleEngine();

	void ResetCursorPosition();
	void SetCursorPosition(COORD);
	void SetTitle(char*);
	void Clear();
	void WriteRow(char*);
	void WriteChar(char*);
	void WriteString(char*);
	void WriteString(const char*);
	void SetManualColour(WORD);
	void SetTextColour(WORD);
	void SetBackgroundColour(WORD);
	void ResetColours();
	int GetKey();
};