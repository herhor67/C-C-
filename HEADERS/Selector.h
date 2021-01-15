// Selector.h
#ifndef Selector_H
#define Selector_H

#include <conio.h>
#include <iostream>
#include <string>
#include <vector>
#include <windows.h>

using namespace std;

#ifndef SEL_CHAR
#define SEL_CHAR "> "
#endif
#ifndef EMP_CHAR
#define EMP_CHAR "  "
#endif


template <typename keyT>
class Selector
{
	typedef short crdT;
	typedef pair<crdT, crdT> crdP;

	vector<pair<keyT, string>> options;
	bool lnbr;

	void cursorToYX(crdT Y, crdT X) const
	{
		COORD coord = { X, Y };
		HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
		if (!SetConsoleCursorPosition(hConsole, coord))
			throw runtime_error("Could not move the cursor.");
	}
	void cursorToYX(const crdP& coords) const
	{
		cursorToYX(coords.first, coords.second);
	}
	crdP whereCursorAt() const
	{
		CONSOLE_SCREEN_BUFFER_INFO csbi;
		if (!GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi))
			return crdP(0, 0);
		return crdP(csbi.dwCursorPosition.Y, csbi.dwCursorPosition.X);
	}
	void drawArrows(const crdP& pos) const
	{
		auto end = whereCursorAt();
		for (int r = pos.first; r <= pos.second; r++)
		{
			cursorToYX(r, 0);
			cout << SEL_CHAR;
		}
		cursorToYX(end);
	}
	void deleteArrows(const crdP& pos) const
	{
		auto end = whereCursorAt();
		for (int r = pos.first; r <= pos.second; r++)
		{
			cursorToYX(r, 0);
			cout << EMP_CHAR;
		}
		cursorToYX(end);
	}

public:
	Selector(bool b = true) : lnbr(b)
	{
	}
	~Selector()
	{
	}
	void addOption(const keyT& key, const string& val)
	{
		options.push_back(pair<keyT, string>(key, val));
	}
	keyT selectOption(size_t select = -1) const
	{
		vector<crdP> rows;
		size_t size = options.size();
		if (select < 0 || select >= size)
			select = size - 1;
		for (const pair<keyT, string>& option : options)
		{
			crdT start = whereCursorAt().first;

			size_t from = 0;
			size_t to = 0;
			do
			{
				to = option.second.find('\n', from);
				cout << EMP_CHAR << option.second.substr(from, to - from) << endl;
				from = to+1;
			}
			while (to != std::string::npos);

			rows.push_back(crdP(start, whereCursorAt().first-1));

			if (lnbr)
				cout << endl;
		}
		if (!lnbr)
			cout << endl;

		drawArrows(rows.at(select));

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
					deleteArrows(rows[select]);
					if (select <= 0)
						select = size - 1;
					else
						--select;
					drawArrows(rows[select]);
					break;
				case 77: // arrow left
				case 80: // arrow down
					deleteArrows(rows[select]);
					if (select >= size - 1)
						select = 0;
					else
						++select;
					drawArrows(rows[select]);
					break;
				default:
					break;
				}
				break;
			case 13: // carriage return
			case 32: // space
				return options[select].first;
				break;
			case 8:  // backspace
			case 24: // cancel
			case 27: // escape
				return keyT();
				break;
			}
		}
	}
};

#endif
