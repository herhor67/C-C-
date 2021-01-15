#ifndef Color_H
#define Color_H
#include <iostream> 
#include <windows.h> 

class Color
{
private:
	int color = 7;
	HANDLE consoleHandle;

public:
	Color(int txtColor = 7, bool underscore = false, int bgColor = 0)
	{
		consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
		operator()(txtColor, underscore, bgColor);
	}

	Color& operator() (int txtColor = 7, bool underscore = false, int bgColor = 0)
	{
		this->color = txtColor | (bgColor << 4) | (underscore ? COMMON_LVB_UNDERSCORE : 0);
		return *this;
	}

	friend std::ostream& operator<<(std::ostream& ss, Color obj) {
		SetConsoleTextAttribute(obj.consoleHandle, obj.color);
		return ss;
	}
};

#endif
