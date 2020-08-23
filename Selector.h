// Selector.h
#ifndef Selector_H
#define Selector_H

#include <conio.h>
#include <map>
#include <memory>
#include <iostream>
#include <string>
#include <vector>
#include <windows.h>
#include "Functions.h"
using namespace std;

#define SEL_CHAR ">"
#define EMP_CHAR " "


template <typename keyT>
class Selector
{
	vector<pair<keyT, string>> options;
	bool lnbr;
	void cursorToYX(const int& Y, const int& X) const
	{
		COORD coord;
		coord.Y = Y;
		coord.X = X;
		HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
		if (!SetConsoleCursorPosition(hConsole, coord))
			throw runtime_error("Could not move the cursor.");
	}
	void cursorToYX(const pair<int, int>& coords) const
	{
		COORD coord;
		coord.Y = coords.first;
		coord.X = coords.second;
		HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
		if (!SetConsoleCursorPosition(hConsole, coord))
			throw runtime_error("Could not move the cursor.");
	}
	pair<int, int> whereCursorAt() const
	{
		CONSOLE_SCREEN_BUFFER_INFO csbi;
		if (!GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi))
			return pair<int, int>(0, 0);
		return pair<int, int>(csbi.dwCursorPosition.Y, csbi.dwCursorPosition.X);
	}
	void drawArrows(const pair<int, int>& pos) const
	{
		pair<int, int> end = whereCursorAt();
		for (int r = pos.first; r <= pos.second; r++)
		{
			cursorToYX(r, 0);
			cout << SEL_CHAR;
		}
		cursorToYX(end);
	}
	void deleteArrows(const pair<int, int>& pos) const
	{
		pair<int, int> end = whereCursorAt();
		for (int r = pos.first; r <= pos.second; r++)
		{
			cursorToYX(r, 0);
			cout << EMP_CHAR;
		}
		cursorToYX(end);
	}
public:
	Selector() : lnbr(true)
	{
	}
	Selector(bool b) : lnbr(b)
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
		vector<pair<int, int>> rows;
		size_t size = options.size();
		if (select < 0 || select >= size)
			select = size - 1;
		for (const pair<keyT, string>& option : options)
		{
			int start = whereCursorAt().first;
			cout << EMP_CHAR << " " << str_replace(option.second, "\n", string("\n") + EMP_CHAR + " ");
			rows.push_back(pair<int, int>(start, whereCursorAt().first));
			cout << endl;
			if (lnbr)
				cout << endl;
		}
		cout << endl;
		drawArrows(rows.at(select));

		int ch;
		while (true)
		{
			ch = _getch();
			switch (ch)
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
						select--;
					drawArrows(rows[select]);
					break;
				case 77: // arrow left
				case 80: // arrow down
					deleteArrows(rows[select]);
					if (select >= size - 1)
						select = 0;
					else
						select++;
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
				keyT empty = keyT();
				return empty;
				break;
			}
		}
	}
};

#endif
