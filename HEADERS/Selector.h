// Selector.h
#ifndef Selector_H
#define Selector_H

#include <conio.h>
#include <iostream>
#include <string>
#include <vector>
#include <windows.h>

#define SEL_STR "> "
#define EMP_STR "  "

template <typename keyT>
class Selector
{
	typedef short crdT;
	typedef std::pair<crdT, crdT> crdP;

	std::vector<std::pair<keyT, std::string>> options;
	bool lnbr;

	void cursorToYX(crdT Y, crdT X) const
	{
		COORD coord = { X, Y };
		HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
		if (!SetConsoleCursorPosition(hConsole, coord))
			throw std::runtime_error("Could not move the cursor.");
	}
	void cursorToYX(const crdP& coords) const
	{
		cursorToYX(coords.first, coords.second);
	}
	crdP whereCursor() const
	{
		CONSOLE_SCREEN_BUFFER_INFO csbi;
		if (!GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi))
			return crdP(0, 0);
		return crdP(csbi.dwCursorPosition.Y, csbi.dwCursorPosition.X);
	}
	void selectorDraw(const crdP& pos) const
	{
//		auto end = whereCursor();
		for (int r = pos.first; r <= pos.second; r++)
		{
			cursorToYX(r, 0);
			std::cout << SEL_STR;
		}
//		cursorToYX(end);
	}
	void deselectorDraw(const crdP& pos) const
	{
//		auto end = whereCursor();
		for (int r = pos.first; r <= pos.second; r++)
		{
			cursorToYX(r, 0);
			std::cout << EMP_STR;
		}
//		cursorToYX(end);
	}

public:
	Selector(bool b = true) : lnbr(b)
	{
	}
	~Selector()
	{
	}
	void addOption(const keyT& key, const std::string& val)
	{
		options.push_back(std::pair<keyT, std::string>(key, val));
	}
	keyT selectOption(size_t select = -1) const
	{
		std::vector<crdP> rows;
		size_t size = options.size();
		if (select < 0 || select >= size)
			select = size - 1;
		for (const std::pair<keyT, std::string>& option : options)
		{
			crdT start = whereCursor().first;

			size_t from = 0;
			size_t to = 0;
			do
			{
				to = option.second.find('\n', from);
				std::cout << EMP_STR << option.second.substr(from, to - from) << std::endl;
				from = to + 1;
			} while (to != std::string::npos);

			rows.push_back(crdP(start, whereCursor().first - 1));

			if (lnbr)
				std::cout << std::endl;
		}
		if (!lnbr)
			std::cout << std::endl;

		crdP lastPos = whereCursor();

		selectorDraw(rows[select]);

		while (true)
		{
			switch (_getch())
			{
			case 0:
			case 224:
				switch (_getch())
				{
				case 72: // arrow up
				case 75: // arrow left
					deselectorDraw(rows[select]);
					if (select <= 0)
						select = size - 1;
					else
						--select;
					selectorDraw(rows[select]);
					break;
				case 77: // arrow left
				case 80: // arrow down
					deselectorDraw(rows[select]);
					if (select >= size - 1)
						select = 0;
					else
						++select;
					selectorDraw(rows[select]);
					break;
				default:
					break;
				}
				break;
			case 13: // carriage return
			case 32: // space
				cursorToYX(lastPos);
				return options[select].first;
				break;
			case 8:  // backspace
			case 24: // cancel
			case 27: // escape
				cursorToYX(lastPos);
				return keyT();
				break;
			}
		}
	}
};

#endif
