#ifndef SimpleBar_H
#define SimpleBar_H

#include <iostream>

std::string getProgress(unsigned size, unsigned current, unsigned max)
{
	if (size == 0)
		return "";
	if (size == 1)
		return "#";

	if (current > max)
		current = max;

	std::string ret(1, '[');
	ret.reserve(size);
	ret.resize(1 + current * (size - 2) / max, '='); // '=' '#'
	ret.resize(size, ' ');
	ret[current * (size - 2) / max + 1] = '>';
	ret[size - 1] = ']';

	return ret;
}

#endif
