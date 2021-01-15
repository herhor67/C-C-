// ProgressBar.h
#ifndef ProgressBar_H
#define ProgressBar_H

#include <conio.h>
#include <iostream>
#include <string>
//#include <vector>
#include <windows.h>


template <typename numT>
class ProgressBar
{
	typedef short crdT;
	typedef std::pair<crdT, crdT> crdP;

	crdP startPos;
	unsigned maxWidth = 0;
	unsigned currentWidth = 0;
	numT maxValue;

	void cursorToYX(const crdP& coords) const
	{
		COORD coord = { coords.second, coords.first };
		HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
		if (!SetConsoleCursorPosition(hConsole, coord))
			throw std::runtime_error("Could not move the cursor.");
	}
	crdP whereCursor() const
	{
		CONSOLE_SCREEN_BUFFER_INFO csbi;
		if (!GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi))
			return crdP(0, 0);
		return crdP(csbi.dwCursorPosition.Y, csbi.dwCursorPosition.X);
	}

public:
	ProgressBar(unsigned width, numT maxval) : maxValue(maxval)
	{
		if (width < 3)
			throw std::invalid_argument("Width of ProgressBar cannot be smaller than 3 (bool)!");

		maxWidth = width - 2;

		startPos = whereCursor();
		++startPos.second;
		std::cout << "[>" << std::string(maxWidth-1, ' ') << ']';
	}
	~ProgressBar()
	{
	}
	void update(numT currentValue)
	{
		if (currentValue > maxValue)
			currentValue = maxValue;

		unsigned newWidth = currentValue * maxWidth / maxValue;

		if (newWidth != currentWidth)
		{
			currentWidth = newWidth;
			crdP old = whereCursor();
			cursorToYX(startPos);

			std::cout << std::string(newWidth, '='); // '=' '#'

			if (currentWidth < maxWidth)
				std::cout << '>';
			if (currentWidth + 1 < maxWidth)
				std::cout << std::string(maxWidth - currentWidth - 1, ' ');

			cursorToYX(old);
		}
	}
};

#endif
