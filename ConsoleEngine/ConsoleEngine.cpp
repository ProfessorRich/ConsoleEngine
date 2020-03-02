#include <windows.h>
#include "ConsoleEngine.h"

ConsoleEngine::ConsoleEngine() {										// The constructor.
	m_currentConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	m_keyboard = GetStdHandle(STD_INPUT_HANDLE);

	SetConsoleWindowInfo(m_currentConsole, TRUE, &m_consoleSize);		// I'm not sure this actually does anything or is useful at all.
	
	ResetCursorPosition();
	ResetColours();

	SetTextColour(m_textColour);
	SetBackgroundColour(m_backgroundColour); 	
}

int ConsoleEngine::GetKey() {											// If keypressed, returns the int for it.
	DWORD read;
	INPUT_RECORD event;

	ReadConsoleInputA(m_keyboard, &event, 1, &read);

	if (event.EventType == KEY_EVENT) {
		if (event.Event.KeyEvent.bKeyDown) {
			return event.Event.KeyEvent.wVirtualKeyCode;
		}
	}

	return 0;
}

void ConsoleEngine::SetManualColour(WORD colour) {						// Used for experimentation and debugging
	SetConsoleTextAttribute(m_currentConsole, colour);
}

void ConsoleEngine::SetTextColour(WORD colour) {						// This CAPS enums are defined already in windows.h. The |= operator is to additatively combine binaries e.g. 11110000 + 1100101 |= 11110101
	
	m_textColour = 0;

	if (colour & ColourRed) {
		m_textColour |= FOREGROUND_RED;
	}
	if (colour & ColourBlue) {
		m_textColour |= FOREGROUND_BLUE;
	}
	if (colour & ColourGreen) {
		m_textColour |= FOREGROUND_GREEN;
	}
	if (colour & ColourIntensify) {
		m_textColour |= FOREGROUND_INTENSITY;
	}
	SetConsoleTextAttribute(m_currentConsole, m_textColour | m_backgroundColour);
}

void ConsoleEngine::SetBackgroundColour(WORD colour) {

	m_backgroundColour = 0;

	if (colour & ColourRed) {
		m_textColour |= BACKGROUND_RED;
	}
	if (colour & ColourBlue) {
		m_textColour |= BACKGROUND_BLUE;
	}
	if (colour & ColourGreen) {
		m_textColour |= BACKGROUND_GREEN;
	}
	if (colour & ColourIntensify) {
		m_textColour |= BACKGROUND_INTENSITY;
	}

	SetConsoleTextAttribute(m_currentConsole, m_textColour | m_backgroundColour);
}

void ConsoleEngine::ResetCursorPosition() {								
	m_currentCursorPosition.X = 0;
	m_currentCursorPosition.Y = 0;
	SetCursorPosition(m_currentCursorPosition);
}

void ConsoleEngine::SetCursorPosition(COORD position) {
	SetConsoleCursorPosition(m_currentConsole, position);
}

void ConsoleEngine::SetTitle(char* title) {
	SetConsoleTitleA(title);											//Not working?
}

void ConsoleEngine::ResetColours() {
	m_textColour = ColourWhite;

	m_backgroundColour = 0;

	SetTextColour(m_textColour);
	SetBackgroundColour(m_backgroundColour);
}

void ConsoleEngine::Clear() {
	FillConsoleOutputAttribute(m_currentConsole, m_textColour | m_backgroundColour, 100000, { 0,0 }, &m_written); // 100000 is just a random big number... There must be a better way to clear screen?
	FillConsoleOutputCharacter(m_currentConsole, ' ', 100000, { 0,0 }, &m_written);
}

void ConsoleEngine::WriteRow(char* row) {
	WriteConsoleA(m_currentConsole, row, (g_consoleWidth-1), &m_written, NULL);
}

void ConsoleEngine::WriteChar(char* c) {
	WriteConsoleA(m_currentConsole, c, 1, &m_written, NULL);
}

void ConsoleEngine::WriteString(char* string) {
	WriteConsoleA(m_currentConsole, string, strlen(string), &m_written, NULL);
}

void ConsoleEngine::WriteString(const char* string) {
	WriteConsoleA(m_currentConsole, string, strlen(string), &m_written, NULL);
}