#include <iostream>
#include <string>
#include <vector>
#include <map>

using namespace std;
using ull = unsigned long long;

using unit = unsigned long long;
constexpr size_t unitbyte = sizeof(unit);


vector<string> str_split_backward(const string& str, size_t split)
{
	vector<string> out;
	out.reserve(str.length() / split + (str.length() % split) ? 1 : 0);

	size_t i = str.length() % split;
	if (i != 0)
		out.push_back(str.substr(0, i));
	for (; i < str.length(); i += split)
		out.push_back(str.substr(i, split));
	return out;
}
vector<string> str_split(const string& str, size_t split)
{
	vector<string> out;
	out.reserve(str.length() / split + (str.length() % split) ? 1 : 0);
	for (size_t i = 0; i < str.length(); i += split)
		out.push_back(str.substr(i, split));
	return out;
}
vector<char> str_explode(const string& str)
{
	vector<char> out;
	out.reserve(str.length());
	for (size_t i = 0; i < str.length(); ++i)
		out.push_back(str[i]);
	return out;
}

string num2bin(unit num)
{
	string temp;
	for (unit i = (unit)1 << (unitbyte * 8 - 1); i != 0; i >>= 1)
		temp.push_back((num & i) ? '1' : '0');
	return temp;
}
string num2hex(unit num)
{
	string temp;
	for (unit i = (unit)1 << (unitbyte * 8 - 1); i != 0; i >>= 1)
	{
		char dec = bool(num & i);
		i >>= 1;
		dec = (dec << 1) | static_cast<char>(static_cast<bool>(num & i));
		i >>= 1;
		dec = (dec << 1) | static_cast<char>(static_cast<bool>(num & i));
		i >>= 1;
		dec = (dec << 1) | static_cast<char>(static_cast<bool>(num & i));
		char hex = '0';
		if      (dec <= 9)
			hex = dec + 48;
		else if (dec <= 15)
			hex = dec + 65 - 10;
		temp.push_back(hex);
	}
	return temp;
}

string oct2bin(string oct)
{
	vector<char> chars = str_explode(oct);
	string bin;
	bin.reserve(3*oct.length());
	for (char chr : chars)
	{
		char dec = chr - 48;
		bin.push_back(dec & 4 ? '1' : '0');
		bin.push_back(dec & 2 ? '1' : '0');
		bin.push_back(dec & 1 ? '1' : '0');
	}
	return bin;
}

ull integerSqrt(ull n)
{
	if (n < 2)
		return n;
	else
	{
		ull sC = integerSqrt(n >> 2) << 1;
		ull lC = sC + 1;
		if (lC * lC > n)
			return sC;
		else
			return lC;
	}
}
vector<ull> primes(ull limit)
{
	vector<ull> primes;
	if (limit < 2)
		return primes;
	primes.push_back(2);
	for (ull number = 3; number <= limit; number += 2)
	{
		ull maxDiv = integerSqrt(number) + 1;
		bool isPrime = true;
		for (const ull& prime : primes)
		{
			if (prime > maxDiv)
				break;
			if (number % prime == 0)
			{
				isPrime = false;
				break;
			}
		}
		if (isPrime)
			primes.push_back(number);
	}
	return primes;
}


class varint
{
public:
	bool sign = 0;
	vector<unit> value;
	varint(unit num)
	{
		value.push_back(num);
	}
	varint(string num, size_t base = 16)
	{
		if      (base == 16) // hex
		{
			vector<string> pieces = str_split_backward(num, unitbyte * 2);
			value.reserve(pieces.size());
			for (vector<string>::reverse_iterator i = pieces.rbegin(); i != pieces.rend(); ++i)
				value.push_back(stoull(*i, 0, 16));
//			for (string piece : pieces)
//				value.push_back(stoull(piece, 0, 16));
		}
		else if (base == 8) // oct
		{
			vector<string> pieces = str_split_backward(oct2bin(num), unitbyte * 8);
			value.reserve(pieces.size());
			for (vector<string>::reverse_iterator i = pieces.rbegin(); i != pieces.rend(); ++i)
				value.push_back(stoull(*i, 0, 2));
//			for (string piece : pieces)
//				value.push_back(stoull(piece, 0, 2));
		}
		else if (base == 2) // bin
		{
			vector<string> pieces = str_split_backward(num, unitbyte * 8);
			value.reserve(pieces.size());
			for (vector<string>::reverse_iterator i = pieces.rbegin(); i != pieces.rend(); ++i)
				value.push_back(stoull(*i, 0, 2));
//			for (string piece : pieces)
//				value.push_back(stoull(piece, 0, 2));
		}
		else // non pow2 systems
		{

		}
		reduce();
	}
	
	varint& increment(unit incr, size_t pos = 0)
	{
		if (pos >= value.size())
		{
			value.reserve(pos);
			for (size_t i = value.size(); i < pos; ++i)
				value.at(i) = 0;
		}
		do
		{
//			cout << "(" << pos << ") " << incr << endl;
			bool a = value.at(pos) >> (unitbyte * 8 - 1);
			bool b = incr >> (unitbyte * 8 - 1);
			value.at(pos) += incr;
			bool s = value.at(pos) >> (unitbyte * 8 - 1);
			incr = (!s && (a || b) || a && b);
			++pos;
		}
		while (incr != 0);
		reduce();
		return *this;
	}

	bool operator >  (const unit& rhs)
	{
		if (value.size() > 1)
			return true;
		if (value.size() == 1 && value.at(0) > rhs)
			return true;
		return false;
	}
	bool operator >= (const unit& rhs)
	{
		if (value.size() > 1)
			return true;
		if (value.size() == 1 && value.at(0) >= rhs)
			return true;
		return false;
	}
	bool operator <  (const unit& rhs)
	{
		if (value.size() == 1 && value.at(0) < rhs)
			return true;
		return false;
	}
	bool operator <= (const unit& rhs)
	{
		if (value.size() == 1 && value.at(0) <= rhs)
			return true;
		return false;
	}
	bool operator == (const unit& rhs)
	{
		if (value.size() == 1 && value.at(0) == rhs)
			return true;
		return false;
	}
	bool operator != (const unit& rhs)
	{
		if (value.size() != 1 || value.at(0) != rhs)
			return true;
		return false;
	}

	bool operator >  (const varint& rhs)
	{
		if (value.size() > rhs.value.size())
			return true;
		if (value.size() < rhs.value.size())
			return false;
		for (int i = value.size() - 1; i >= 0; --i)
		{
			if (value.at(i) > rhs.value.at(i))
				return true;
			if (value.at(i) < rhs.value.at(i))
				return false;
		}
		return false;
	}
	bool operator >= (const varint& rhs)
	{
		if (value.size() > rhs.value.size())
			return true;
		if (value.size() < rhs.value.size())
			return false;
		for (int i = value.size() - 1; i >= 0; --i)
		{
			if (value.at(i) > rhs.value.at(i))
				return true;
			if (value.at(i) < rhs.value.at(i))
				return false;
		}
		return true;
	}
	bool operator <  (const varint& rhs)
	{
		if (value.size() < rhs.value.size())
			return true;
		if (value.size() > rhs.value.size())
			return false;
		for (int i = value.size() - 1; i >= 0; --i)
		{
			if (value.at(i) < rhs.value.at(i))
				return true;
			if (value.at(i) > rhs.value.at(i))
				return false;
		}
		return false;
	}
	bool operator <= (const varint& rhs)
	{
		if (value.size() < rhs.value.size())
			return true;
		if (value.size() > rhs.value.size())
			return false;
		for (int i = value.size() - 1; i >= 0; --i)
		{
			if (value.at(i) < rhs.value.at(i))
				return true;
			if (value.at(i) > rhs.value.at(i))
				return false;
		}
		return true;
	}
	bool operator == (const varint& rhs)
	{
		if (value.size() != rhs.value.size())
			return false;
		for (int i = value.size() - 1; i >= 0; --i)
		{
			if (value.at(i) != rhs.value.at(i))
				return false;
		}
		return true;
	}
	bool operator != (const varint& rhs)
	{
		if (value.size() != rhs.value.size())
			return true;
		for (int i = value.size() - 1; i >= 0; --i)
		{
			if (value.at(i) != rhs.value.at(i))
				return true;
		}
		return false;
	}


	varint& operator+=(const unit& rhs)
	{
		return increment(rhs, 0);
	}
	varint  operator+ (const varint& rhs)
	{
		varint temp(*this);

		return temp;
	}

	void reduce()
	{
		for (size_t i = value.size() - 1; i != 0; --i)
			if (value.at(i) == 0)
				value.pop_back();
			else
				break;
	}

	string hex() const
	{
		string result = "0x\n";
		for (unit piece : value)
			result.append(num2hex(piece)+'\n');
		return result;
	}
	string dec() const
	{
		string result = "0d\n";
		for (unit piece : value)
			result.append(to_string(piece)+'\n');
		return result;
	}
	string bin() const
	{
		string result = "0b\n";
		for (unit piece : value)
			result.append(num2bin(piece)+'\n');
		return result;
	}
};

/*class Meth
{
private:
	map<ull, ull> factors;
public:
	Meth(ull nom, ull den = 1)
	{

	}
};*/

int main()
{
	string hex = "1234567890ABCDEFFEDCBA9876543210FFFFFFFFFFFFFFFF";
	string oct = "22150531704653633677766713523035452062041777777777777777777777";
	string bin = "0000000000000000000000000000000000000000000010010001101000101011001111000100110101011110011011110111111111110110111001011101010011000011101100101010000110010000100001111111111111111111111111111111111111111111111111111111111111111";
	varint num(hex, 16);
//	cout << num.dec() << endl << num.hex() << endl << num.bin() << endl;

	varint snd(hex, 16);
//	snd += 1;

	cout << (num != snd);
}
