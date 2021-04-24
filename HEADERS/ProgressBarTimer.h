// ProgressBarTimer.h
#ifndef ProgressBarTimer_H
#define ProgressBarTimer_H

#include <chrono>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <conio.h>
#include <windows.h>


template <typename numT>
class ProgressBar
{
	typedef short crdT;
	typedef std::pair<crdT, crdT> crdP;

	std::chrono::steady_clock::time_point startTime = std::chrono::steady_clock::now();
	unsigned currentWidth = 0;
	bool     reuse;
	char     arrowChar;

	numT     maxValue;
	unsigned maxWidth;

	crdP     startPos;
	crdP     detailsPos;


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
	ProgressBar(unsigned width, numT maxValue, bool reuse = false, char arrowChar = '>') : reuse(reuse), maxValue(maxValue), maxWidth(width - 2), arrowChar(arrowChar)
	{
		if (width < 3)
			throw std::invalid_argument("Width of ProgressBar cannot be smaller than 3 (bool)!");

		startPos = whereCursor();
		std::cout << '[' << arrowChar << std::string(maxWidth - 1, ' ') << ']';

		detailsPos = whereCursor();
		++detailsPos.second;
		std::cout << " 0/" << maxValue << "\tEta: I";
		if (!reuse)
			std::cout << std::endl;
	}
	~ProgressBar()
	{
	}

	void update(numT currentValue)
	{
		if (currentValue > maxValue)
			currentValue = maxValue;

		crdP old = whereCursor();

		unsigned newWidth = currentValue * maxWidth / maxValue;

		// redraw bar if not reusable or displayed width changed
		if (!reuse || newWidth != currentWidth)
		{
			currentWidth = newWidth;

			if (reuse)
				cursorToYX(startPos);

			std::cout << '[' << std::string(newWidth, '='); // '=' '#'

			if (currentWidth < maxWidth)
				std::cout << arrowChar;

			if (currentWidth + 1 < maxWidth)
				std::cout << std::string(maxWidth - currentWidth - 1, ' ');

			std::cout << ']';
		}

		if (reuse)
			cursorToYX(detailsPos);

		std::cout << ' ' << currentValue << '/' << maxValue;

		if (currentValue != 0)
		{
			double ratio = (double)currentValue / maxValue;
			//			std::chrono::duration<double, std::chrono::steady_clock::period> time_taken = ;
			//			std::chrono::duration<double, std::chrono::steady_clock::period> time_left = time_taken * (1 / ratio - 1);
			std::chrono::duration<double, std::chrono::steady_clock::period> time_left = (std::chrono::steady_clock::now() - startTime) * (1 / ratio - 1);

			auto d = std::chrono::duration_cast<std::chrono::duration<int, std::ratio<86400>>>(time_left);
			time_left -= d;
			auto h = std::chrono::duration_cast<std::chrono::hours>(time_left);
			time_left -= h;
			auto m = std::chrono::duration_cast<std::chrono::minutes>(time_left);
			time_left -= m;
			auto s = std::chrono::duration_cast<std::chrono::seconds>(time_left);

			auto dc = d.count();
			auto hc = h.count();
			auto mc = m.count();
			auto sc = s.count();

			std::stringstream ss;
			if (!dc && !hc && !mc && !sc)
				ss << "Done";
			else
			{
				ss.fill('0');
				if (dc)
					ss << std::setw(2) << d.count() << "d ";
				if (dc || hc)
					ss << std::setw(2) << h.count() << "h ";
				if (!dc && (hc || mc))
					ss << std::setw(2) << m.count() << "m ";
				if (!dc && !hc && (mc || sc))
					ss << std::setw(2) << s.count() << "s ";
			}

			std::cout << "\tEta: " << ss.rdbuf() << "                                ";
		}

		if (reuse)
			cursorToYX(old);
		else
			std::cout << std::endl;
	}
};

#endif
