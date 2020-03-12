#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <stdexcept>

using namespace std;

typedef unsigned long long ull;
typedef long long sll;
typedef unsigned long long unit; //unsigned int;
constexpr unsigned unitbyte = sizeof(unit);

template <typename T>
T gvoe(const vector<T>& vec, size_t pos)
{
	if (pos >= vec.size())
		return T();
	return vec[pos];
}

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
	for (unit i = unit(1) << (unitbyte * 8 - 1); i != 0; i >>= 1)
	{
		char dec = bool(num & i);
		i >>= 1;
		dec = (dec << 1) | static_cast<char>(static_cast<bool>(num & i));
		i >>= 1;
		dec = (dec << 1) | static_cast<char>(static_cast<bool>(num & i));
		i >>= 1;
		dec = (dec << 1) | static_cast<char>(static_cast<bool>(num & i));
		char hex = '0';
		if (dec <= 9)
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
	bin.reserve(3 * oct.length());
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

namespace Meth
{
	class varint
	{
	public:
		bool pstv = 1;
		vector<unit> value;
		varint() {}
		varint(unit num, bool noneg = 1)
		{
			pstv = noneg;
			value.push_back(num);
			reduce();
		}
		varint(string num, size_t base = 16, bool noneg = 1)
		{
			pstv = noneg;
			if (base == 16) // hex
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
			do
			{
				if (pos >= value.size())
				{
					size_t i = value.size();
					value.resize(pos + 1);
					for (; i <= pos; ++i)
						value.at(i) = 0;
				}
				bool a = value.at(pos) >> (unitbyte * 8 - 1);
				bool b = incr >> (unitbyte * 8 - 1);
				value.at(pos) += incr;
				bool s = value.at(pos) >> (unitbyte * 8 - 1);
				incr = (!s && (a || b) || a && b);
				++pos;
			}
			while (incr != 0);
			return *this;
		}

		varint abs()  const
		{
			varint temp(*this);
			temp.pstv = 1;
			return temp;
		}
		int    sign() const
		{
			if (value.size() == 0)
				return 0;
			return pstv * 2 - 1;
		}
		varint cmpl(size_t pad = 0) const
		{
			varint temp(~*this);
			temp.increment(1, 0);
			for (size_t i = temp.value.size(); i < pad; ++i)
				temp.value.push_back(~0);
			return temp;
		}

		bool operator> (const varint& rhs) const
		{
			if (pstv > rhs.pstv)
				return true;
			if (pstv < rhs.pstv)
				return false;
			size_t maxize = value.size() >= rhs.value.size() ? value.size() : rhs.value.size();
			for (size_t i = maxize; i > 0; --i)
			{
				if (gvoe(value, i - 1) > gvoe(rhs.value, i - 1))
					return pstv;
				if (gvoe(value, i - 1) < gvoe(rhs.value, i - 1))
					return !pstv;
			}
			return false;
		}
		bool operator>=(const varint& rhs) const
		{
			if (pstv > rhs.pstv)
				return true;
			if (pstv < rhs.pstv)
				return false;
			size_t maxize = value.size() >= rhs.value.size() ? value.size() : rhs.value.size();
			for (size_t i = maxize; i > 0; --i)
			{
				if (gvoe(value, i - 1) > gvoe(rhs.value, i - 1))
					return pstv;
				if (gvoe(value, i - 1) < gvoe(rhs.value, i - 1))
					return !pstv;
			}
			return true;
		}
		bool operator< (const varint& rhs) const
		{
			if (pstv < rhs.pstv)
				return true;
			if (pstv > rhs.pstv)
				return false;
			size_t maxize = value.size() >= rhs.value.size() ? value.size() : rhs.value.size();
			for (size_t i = maxize; i > 0; --i)
			{
				if (gvoe(value, i - 1) < gvoe(rhs.value, i - 1))
					return pstv;
				if (gvoe(value, i - 1) > gvoe(rhs.value, i - 1))
					return !pstv;
			}
			return false;
		}
		bool operator<=(const varint& rhs) const
		{
			if (pstv < rhs.pstv)
				return true;
			if (pstv > rhs.pstv)
				return false;
			size_t maxize = value.size() >= rhs.value.size() ? value.size() : rhs.value.size();
			for (size_t i = maxize; i > 0; --i)
			{
				if (gvoe(value, i - 1) < gvoe(rhs.value, i - 1))
					return pstv;
				if (gvoe(value, i - 1) > gvoe(rhs.value, i - 1))
					return !pstv;
			}
			return true;
		}
		bool operator==(const varint& rhs) const
		{
			if (pstv != rhs.pstv)
				return false;
			size_t maxize = value.size() >= rhs.value.size() ? value.size() : rhs.value.size();
			for (size_t i = maxize; i > 0; --i)
			{
				if (gvoe(value, i - 1) != gvoe(rhs.value, i - 1))
					return false;
			}
			return true;
		}
		bool operator!=(const varint& rhs) const
		{
			if (pstv != rhs.pstv)
				return true;
			size_t maxize = value.size() >= rhs.value.size() ? value.size() : rhs.value.size();
			for (size_t i = maxize; i > 0; --i)
			{
				if (gvoe(value, i - 1) != gvoe(rhs.value, i - 1))
					return true;
			}
			return false;
		}

		bool operator> (const unit& rhs) const
		{
			return *this > varint(rhs);
		}
		bool operator>=(const unit& rhs) const
		{
			return *this >= varint(rhs);
		}
		bool operator< (const unit& rhs) const
		{
			return *this < varint(rhs);
		}
		bool operator<=(const unit& rhs) const
		{
			return *this <= varint(rhs);
		}
		bool operator==(const unit& rhs) const
		{
			return *this == varint(rhs);
		}
		bool operator!=(const unit& rhs) const
		{
			return *this != varint(rhs);
		}

		varint  operator- () const
		{
			varint temp(*this);
			temp.pstv = !pstv;
			return temp;
		}
		varint  operator~ () const
		{
			varint temp;
			temp.value.resize(value.size());
			temp.pstv = !pstv;
			for (size_t i = temp.value.size(); i > 0; --i)
				temp.value.at(i - 1) = ~value.at(i - 1);
			return temp;
		}

		varint& operator<<=(const size_t shift)
		{
			if (shift && value.size())
			{
				size_t lshift = shift % (unitbyte * 8);
				size_t append = shift / (unitbyte * 8);
				unsigned r = 0;
				unit temp = value.back();
				while (temp >>= 1)
					++r;
				size_t old = value.size() + append;
				value.resize(old + 1 + (lshift >= unitbyte * 8 - r));
				for (size_t i = value.size(); i > 0; --i)
				{
					size_t p1 = i - 1;
					size_t p2 = i - 2;
					value.at(p1) = ((p1 <= old && p1 >= append) ? value.at(p1 - append) << lshift : 0) | ((p2 <= old && p2 >= append) ? value.at(p2 - append) >> (unitbyte * 8 - lshift) : 0);
				}
			}
			return *this;
		}
		varint& operator>>=(const size_t shift)
		{
			if (shift)
			{
				size_t rshift = shift % (unitbyte * 8);
				size_t remove = (shift - rshift) / (unitbyte * 8);
				unsigned r = 0;

				//			size_t old = value.size() - 1 + append;
				for (size_t i = 0; i < value.size(); ++i)
					value.at(i) = (gvoe(value, i + remove) >> rshift) | (rshift ? (gvoe(value, i + 1 + remove) << (unitbyte * 8 - rshift)) : 0);
				//			value.resize(old + 1 + (lshift >= unitbyte * 8 - r));
				reduce();
			}
			return *this;
		}

		varint& operator+=(const varint rhs)
		{
			if (sign() == 0)
			{
				value = rhs.value;
				pstv = rhs.pstv;
			}
			else if (rhs.sign() == 0) {}
			else if (sign() == rhs.sign())
			{
				for (size_t i = 0; i < rhs.value.size(); ++i)
					increment(gvoe(rhs.value, i), i);
			}
			else if (abs() == rhs.abs())
			{
				value.clear();
			}
			else if (abs() > rhs.abs())
			{
				varint cpl = rhs.cmpl(value.size());
				for (size_t i = 0; i < cpl.value.size(); ++i)
					increment(gvoe(cpl.value, i), i);
				value.pop_back();
			}
			else// if (abs() < rhs.abs())
			{
				*this = cmpl(rhs.value.size());
				pstv = rhs.pstv;
				for (size_t i = 0; i < rhs.value.size(); ++i)
					increment(gvoe(rhs.value, i), i);
				value.pop_back();
			}
			reduce();
			return *this;
		}
		varint& operator-=(const varint rhs)
		{
			return operator+=(-rhs);
		}
		varint& operator*=(const varint rhs)
		{
			pstv = (pstv == rhs.pstv); // n-xor: +*+=+, -*-=+, +*-=-, -*+=+
			varint temp = *this;
			value.clear();
			value.resize(temp.value.size() + rhs.value.size());
			for (size_t u = 0; u < rhs.value.size(); ++u)
				for (size_t b = 0; b < unitbyte * 8; ++b)
				{
					if (rhs.value.at(u) & (unit(1) << b))
						operator+=(temp);
					temp <<= 1;
				}
			reduce();
			return *this;
		}
		varint& operator/=(const varint rhs)
		{
			*this = operator/(rhs);
			return *this;
		}
		varint& operator%=(const varint rhs)
		{
			bool sign = pstv;
			*this = abs();
			varint divisor = rhs.abs();
			divisor <<= leftmostone() - divisor.leftmostone();
			while (*this >= rhs.abs())
			{
//				cout << "this > div" << endl;
//				cout << bin() << divisor.bin() << endl;
				if (*this >= divisor)
					*this -= divisor;
				divisor >>= 1;
//				system("pause");
			}
			pstv = sign;
			return *this;
		}

		varint  operator<<(const size_t rhs) const
		{
			varint temp(*this);
			temp.operator<<=(rhs);
			return temp;
		}
		varint  operator>>(const size_t rhs) const
		{
			varint temp(*this);
			temp.operator>>=(rhs);
			return temp;
		}

		varint  operator+ (const varint rhs) const
		{
			varint temp(*this);
			temp.operator+=(rhs);
			return temp;
		}
		varint  operator- (const varint rhs) const
		{
			varint temp(*this);
			temp.operator+=(-rhs);
			return temp;
		}
		varint  operator* (const varint rhs) const
		{
			varint temp(*this);
			temp.operator*=(rhs);
			return temp;
		}
		varint  operator/ (const varint rhs) const
		{
			if (rhs.sign() != 0)
			{
				varint divident = abs();
				varint divisor = rhs.abs();
				if (divident < divisor)
					return 0;

				varint divisorshftd = divisor << (divident.leftmostone() - divisor.leftmostone());
				varint result(0);
				while (divisorshftd >= divisor)
				{
					if (divident >= divisorshftd)
					{
						divident -= divisorshftd;
						result.increment(1);
					}
					result <<= 1;
					divisorshftd >>= 1;
				}
				result >>= 1;
				result.pstv = (pstv == rhs.pstv);
				return result;
			}
			else
				throw runtime_error("Divisor canot be zero!");
		}
		varint  operator% (const varint rhs) const
		{
			varint temp(*this);
			temp.operator%=(rhs);
			return temp;
		}


		varint& operator+=(const unit& rhs)
		{
			return increment(rhs, 0);
		}

		varint pow(ull n) const
		{
			varint x(*this);
			varint y(1);
			while (true)
			{
				if (n & 1)
					y *= x;
				n >>= 1;
				if (n == 0)
					break;
				x *= x;
			}
			return y;
		}
//*		
		varint root(ull n = 2) const
		{
			varint x = abs();
			// Base cases 
			if (x <= 1)
				return x;

			// Do Binary Search for floor(sqrt(x)) 
			varint beg(1);
			varint end = x >> 1;/// n;
			varint ans(0);
			while (beg <= end)
			{
				varint mid = (beg + end) >> 1;
				varint midpow = mid.pow(n);
				
				if (midpow == x)
					return mid;
				
				if (midpow < x)
				{
					beg = mid + 1;
					ans = mid;
				}
				else // midpow > x
					end = mid - 1;
			}
			return ans;
		}
//*/
		
		varint log2() const
		{
			return varint(leftmostone());
		}
		
		
		size_t leftmostone() const
		{
			size_t r = 0;
			if (value.size() == 0)
				return 0;
			unit temp = value.back();
			while (temp >>= 1)
				++r;
			return unitbyte * 8 * (value.size() - 1) + r;
		}
		
		void reduce()
		{
			for (size_t i = value.size(); i > 0; --i)
				if (value.at(i - 1) == 0)
					value.pop_back();
				else
					break;
			if (value.size() == 0)
				pstv = 1;
		}
		
		string hex() const
		{
			string result = string(pstv ? "+" : "-") + "0x\n";
			for (size_t i = value.size(); i > 0; --i)
				result.append(num2hex(value.at(i - 1)) + '\n');
			return result;
		}
		string dec() const
		{
			string result = string(pstv ? "+" : "-") + "0d\n";
			for (size_t i = value.size(); i > 0; --i)
				result.append(to_string(value.at(i - 1)) + '\n');
			return result;
		}
		string bin() const
		{
			string result = string(pstv ? "+" : "-") + "0b\n";
			for (size_t i = value.size(); i > 0; --i)
				result.append(num2bin(value.at(i - 1)) + '\n');
			return result;
		}
	};

	/*class Frac
	{
	private:
		map<ull, ull> factors;
	public:
		Meth(ull nom, ull den = 1)
		{

		}
	};*/
}

using namespace Meth;

int main()
{
	string hex = "0123456789ABCDEFFEDCBA9876543210FFF2FDAFF3FFF9FC";
	string oct = "22150531704653633677766713523035452062041777777777777777777777";
	string bin = "0111010101011101010";

	//varint num(hex, 16, 1);
	varint num(2, 1);

	varint snd(17, 1);

	cout << "A:\n" << num.hex() << num.bin() << endl << endl;
	//	cout << "B:\n" << snd.hex() << endl << endl;

	varint wyn = num.root(17);
//	wyn <<= 1;

	cout << "Wyn:\n" << wyn.hex() << wyn.bin() << endl << endl;
}
