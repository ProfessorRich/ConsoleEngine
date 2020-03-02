#pragma once

// BEHOLD THE POWER OF OtherMiscDeclarations...

struct tile {
	char pattern;
	WORD colour;
};

// BEHOLD THE AWESOME POWER OF GLOBAL CONSTANTS... MUHAHAHAHAHAHAAAAAA! 

const short g_consoleWidth = 200;
const short g_consoleHeight = 60;
const short g_rollTotal = 1000;

const tile g_debugTile = { '!', 28 };
const tile g_clearTile = { ' ', 0 };
const tile g_fieldTile = { '"', 42 };
const tile g_waterTile = { '~',27 };
const tile g_coastTile = { '|',224 };
const tile g_forestTile = { '&', 160 };
const tile g_woodlandTile = { '^', 106 };
const tile g_streamTile = { '|', 249 };
const tile g_diagStreamTile = { '\\', 249 };
const tile g_sideStreamTile = { '-', 249 };
const tile g_brookTile = { '*', 249 };

enum ColourCodes {
	ColourRed = 1,
	ColourGreen = 2,
	ColourBlue = 4,
	ColourWhite = ColourRed | ColourBlue | ColourGreen,
	ColourIntensify = 8,
}; // These codes are used in fancy way to get colours for text/background. It's all to do with setting 0 and 1 bits and using the | and & operators to either set bits or check if they are set.



