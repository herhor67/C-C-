#include <algorithm>
#include <iostream>
#include <map>
#include <stdexcept>
#include <string>
#include <vector>

#include <chrono>

using namespace std;

typedef unsigned long long ull;
typedef long long sll;
typedef unsigned long long unit; //unsigned int;
constexpr unsigned unitbyte = sizeof(unit);
constexpr unsigned unitbits = 8 * unitbyte;

template <typename T>
T gvoe(const vector<T>& vec, size_t pos)
{
	if (pos >= vec.size())
		return T();
	return vec[pos];
}

vector<string> str_split_backward(const string & str, size_t split)
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
vector<string> str_split(const string & str, size_t split)
{
	vector<string> out;
	out.reserve(str.length() / split + (str.length() % split) ? 1 : 0);
	for (size_t i = 0; i < str.length(); i += split)
		out.push_back(str.substr(i, split));
	return out;
}
vector<char>   str_explode(const string & str)
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
	for (unit i = (unit)1 << (unitbits - 1); i != 0; i >>= 1)
		temp.push_back((num & i) ? '1' : '0');
	return temp;
}
string num2hex(unit num)
{
	string temp;
	for (unit i = unit(1) << (unitbits - 1); i != 0; i >>= 1)
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
/*
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
}*/

namespace Meth
{
	class VarInt
	{
	private:
		vector<unit> value;
	public:
		// ====={ Constructors }=====
		VarInt() {}
		VarInt(unit num)
		{
			if (num)
				value.push_back(num);
		}
		VarInt(string num, size_t base = 16)
		{
			if (base == 16) // hex
			{
				vector<string> pieces = str_split_backward(num, unitbyte * 2);
				value.reserve(pieces.size());
				for (vector<string>::reverse_iterator i = pieces.rbegin(); i != pieces.rend(); ++i)
					value.push_back(stoull(*i, 0, 16));
			}
			else if (base == 8) // oct
			{
				vector<string> pieces = str_split_backward(oct2bin(num), unitbits);
				value.reserve(pieces.size());
				for (vector<string>::reverse_iterator i = pieces.rbegin(); i != pieces.rend(); ++i)
					value.push_back(stoull(*i, 0, 2));
			}
			else if (base == 2) // bin
			{
				vector<string> pieces = str_split_backward(num, unitbits);
				value.reserve(pieces.size());
				for (vector<string>::reverse_iterator i = pieces.rbegin(); i != pieces.rend(); ++i)
					value.push_back(stoull(*i, 0, 2));
			}
			else // non pow2 systems
			{
				cout << "Y ur gay?";
			}
			reduce();
		}

		// ====={ Operational }=====
		VarInt& reduce()
		{
			for (size_t i = value.size(); i > 0; --i)
				if (value.at(i - 1) == 0)
					value.pop_back();
				else
					break;
			return *this;
		}
		VarInt& nullify()
		{
			value.clear();
			return *this;
		}
		bool    isnull() const
		{
			return !static_cast<bool>(value.size());
		}

		bool operator!() const
		{
			return !static_cast<bool>(value.size());
		}

		VarInt& increment(unit incr, size_t pos = 0)
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
				bool a = value.at(pos) >> (unitbits - 1);
				bool b = incr >> (unitbits - 1);
				value.at(pos) += incr;
				bool s = value.at(pos) >> (unitbits - 1);
				incr = (!s && (a || b) || a && b);
				++pos;
			} while (incr != 0);
			return *this;
		}
		VarInt& decrement(unit decr, size_t pos = 0)
		{
			do
			{
				if (pos >= value.size())
					throw invalid_argument("Cannot subtract from null!");
				
				bool a = value.at(pos) >> (unitbits - 1);
				bool b = decr >> (unitbits - 1);
				value.at(pos) -= decr;
				bool s = value.at(pos) >> (unitbits - 1);
				decr = (s && (!a || b) || !a && b);
				++pos;
			} while (decr != 0);
			return *this;
		}
	
		/*VarInt twoscompl(size_t padTo = 0) const
		{
			VarInt temp(~*this);
			temp.increment(1, 0);
			for (size_t i = temp.value.size(); i < padTo; ++i)
				temp.value.push_back(~0);
			return temp;
		}*/

		size_t highestbit() const
		{
			size_t r = 0;
			if (value.size() == 0)
				return 0;
			unit temp = value.back();
			while (temp >>= 1)
				++r;
			return unitbits * (value.size() - 1) + r;
		}
		bool getbit(size_t pos) const
		{
			size_t el = pos / unitbits;
			size_t bt = pos % unitbits;
			if (el >= value.size())
				return false;
			else
				return value.at(el) & (ull(1) << bt);
		}

		// ====={ Bitwise ops }=====
		VarInt  operator~() const
		{
			VarInt temp;
			temp.value.resize(value.size());
			for (size_t i = temp.value.size(); i > 0; --i)
				temp.value.at(i - 1) = ~value.at(i - 1);
			temp.reduce();
			return temp;
		}

		VarInt& operator<<=(size_t rhs)
		{
			if (rhs && value.size())
			{
				size_t lshift = rhs % unitbits;
				size_t append = rhs / unitbits;
				unsigned r = 0;
				unit temp = value.back();
				while (temp >>= 1)
					++r;
				size_t old = value.size() + append;
				value.resize(old + 1 + (lshift >= unitbits - r));
				for (size_t i = value.size(); i > 0; --i)
				{
					size_t p1 = i - 1;
					size_t p2 = i - 2;
					value.at(p1) = ((p1 <= old && p1 >= append) ? value.at(p1 - append) << lshift : 0) | ((p2 <= old && p2 >= append) ? value.at(p2 - append) >> (unitbits - lshift) : 0);
				}
				reduce();
			}
			return *this;
		}
		VarInt& operator>>=(size_t rhs)
		{
			if (rhs)
			{
				size_t rshift = rhs % unitbits;
				size_t remove = rhs / unitbits;
				for (size_t i = 0; i < value.size(); ++i)
					value.at(i) = (gvoe(value, i + remove) >> rshift) | (rshift ? (gvoe(value, i + 1 + remove) << (unitbits - rshift)) : 0);
				reduce();
			}
			return *this;
		}
		VarInt  operator<< (size_t rhs) const
		{
			VarInt temp(*this);
			temp.operator<<=(rhs);
			return temp;
		}
		VarInt  operator>> (size_t rhs) const
		{
			VarInt temp(*this);
			temp.operator>>=(rhs);
			return temp;
		}

		VarInt& operator<<=(VarInt rhs)
		{
			if (!rhs.isnull())
				operator<<=(rhs.value.at(0));
			return *this;
		}
		VarInt& operator>>=(VarInt rhs)
		{
			if (!rhs.isnull())
				operator>>=(rhs.value.at(0));
			return *this;
		}
		VarInt  operator<< (VarInt rhs) const
		{
			if (rhs.isnull())
				return *this;
			else
				return operator<<(rhs.value.at(0));
		}
		VarInt  operator>> (VarInt rhs) const
		{
			if (rhs.isnull())
				return *this;
			else
				return operator>>(rhs.value.at(0));
		}

		VarInt& operator&=(VarInt rhs)
		{
			if (isnull() == 1 || rhs.isnull() == 1)
				nullify();
			else
			{
				if (value.size() > rhs.value.size())
					value.resize(rhs.value.size());
				for (size_t i = 0; i < value.size(); ++i)
					value.at(i) &= rhs.value.at(i);
				reduce();
			}
			return *this;
		}
		VarInt& operator|=(VarInt rhs)
		{
			if (isnull() == 1)
				value = rhs.value;
			else if (rhs.isnull() == 1) {}
			else
			{
				if (value.size() < rhs.value.size())
					value.resize(rhs.value.size());
				for (size_t i = 0; i < value.size(); ++i)
					value.at(i) |= rhs.value.at(i);
				reduce();
			}
			return *this;
		}
		VarInt& operator^=(VarInt rhs)
		{
			if (isnull() == 1)
				value = rhs.value;
			else if (rhs.isnull() == 1) {}
			else
			{
				if (value.size() < rhs.value.size())
					value.resize(rhs.value.size());
				for (size_t i = 0; i < value.size(); ++i)
					value.at(i) ^= rhs.value.at(i);
				reduce();
			}
			return *this;
		}
		friend VarInt operator&(VarInt lhs, const VarInt& rhs)
		{
			lhs &= rhs;
			return lhs;
		}
		friend VarInt operator|(VarInt lhs, const VarInt& rhs)
		{
			lhs |= rhs;
			return lhs;
		}
		friend VarInt operator^(VarInt lhs, const VarInt& rhs)
		{
			lhs ^= rhs;
			return lhs;
		}

		// ====={ Comparison ops }=====
		bool operator> (const VarInt& rhs) const
		{
			size_t maxize = max(value.size(), rhs.value.size());
			for (size_t i = maxize; i > 0; --i)
			{
				if (gvoe(value, i - 1) > gvoe(rhs.value, i - 1))
					return true;
				if (gvoe(value, i - 1) < gvoe(rhs.value, i - 1))
					return false;
			}
			return false;
		}
		bool operator>=(const VarInt& rhs) const
		{
			size_t maxize = max(value.size(), rhs.value.size());
			for (size_t i = maxize; i > 0; --i)
			{
				if (gvoe(value, i - 1) > gvoe(rhs.value, i - 1))
					return true;
				if (gvoe(value, i - 1) < gvoe(rhs.value, i - 1))
					return false;
			}
			return true;
		}
		bool operator< (const VarInt& rhs) const
		{
			size_t maxize = max(value.size(), rhs.value.size());
			for (size_t i = maxize; i > 0; --i)
			{
				if (gvoe(value, i - 1) < gvoe(rhs.value, i - 1))
					return true;
				if (gvoe(value, i - 1) > gvoe(rhs.value, i - 1))
					return false;
			}
			return false;
		}
		bool operator<=(const VarInt& rhs) const
		{
			size_t maxize = max(value.size(), rhs.value.size());
			for (size_t i = maxize; i > 0; --i)
			{
				if (gvoe(value, i - 1) < gvoe(rhs.value, i - 1))
					return true;
				if (gvoe(value, i - 1) > gvoe(rhs.value, i - 1))
					return false;
			}
			return true;
		}
		bool operator==(const VarInt& rhs) const
		{
			size_t maxize = max(value.size(), rhs.value.size());
			for (size_t i = maxize; i > 0; --i)
			{
				if (gvoe(value, i - 1) != gvoe(rhs.value, i - 1))
					return false;
			}
			return true;
		}
		bool operator!=(const VarInt& rhs) const
		{
			size_t maxize = max(value.size(), rhs.value.size());
			for (size_t i = maxize; i > 0; --i)
			{
				if (gvoe(value, i - 1) != gvoe(rhs.value, i - 1))
					return true;
			}
			return false;
		}
		int cmp(const VarInt& rhs) const
		{
			size_t maxize = max(value.size(), rhs.value.size());
			for (size_t i = maxize; i > 0; --i)
			{
				if (gvoe(value, i - 1) > gvoe(rhs.value, i - 1))
					return 1;
				if (gvoe(value, i - 1) < gvoe(rhs.value, i - 1))
					return -1;
			}
			return 0;
		}

		// ====={ Math ops }=====
		VarInt& operator+=(const VarInt& rhs)
		{
			if (isnull() == 1)
				value = rhs.value;
			else if (rhs.isnull() == 1)
				{}
			else
				for (size_t i = 0; i < rhs.value.size(); ++i)
					increment(gvoe(rhs.value, i), i);
			reduce();
			return *this;
		}
		VarInt& operator-=(const VarInt& rhs)
		{
			int ltgtoe = cmp(rhs);
			if (ltgtoe == 0)
				nullify();
			else if (ltgtoe > 0)
				for (size_t i = 0; i < rhs.value.size(); ++i)
					decrement(gvoe(rhs.value, i), i);
			else if (ltgtoe < 0)
			{
				VarInt subtrahend(*this);
				value = rhs.value;
				for (size_t i = 0; i < subtrahend.value.size(); ++i)
					decrement(gvoe(subtrahend.value, i), i);
			}
			reduce();
			return *this;
		}
		VarInt& operator*=(const VarInt& rhs)
		{
			if (isnull() || rhs.isnull())
				nullify();
			else if (operator==(1))
				value = rhs.value;
			else if (rhs.operator==(1))
			{}
			else
			{
				VarInt multiplicand, multiplier;
				if (operator>=(rhs))
				{
					multiplicand = *this;
					multiplier = rhs;
				}
				else
				{
					multiplicand = rhs;
					multiplier = *this;
				}
				nullify();
				value.resize((multiplier.highestbit() + multiplicand.highestbit() - 1) / unitbits + 1);
				size_t highbit = multiplier.highestbit();
				for (size_t b = 0; b <= highbit; ++b)
				{
					if (multiplier.getbit(b))
						operator+=(multiplicand);
					multiplicand <<= 1;
				}
//				reduce();
			}
			return *this;
		}
		VarInt& operator/=(const VarInt& rhs)
		{
			if (rhs.isnull() == 1)
				throw invalid_argument("Divisor canot be zero!");
			else if (operator<(rhs))
				nullify();
			else
			{
				VarInt divident(*this);
				VarInt divisor(rhs);
				VarInt divisorshftd = divisor << (divident.highestbit() - divisor.highestbit());
				nullify();
				while (divisorshftd >= divisor)
				{
					if (divident >= divisorshftd)
					{
						divident -= divisorshftd;
						increment(1);
					}
					operator<<=(1);
					divisorshftd >>= 1;
				}
				operator>>=(1);
			}
			return *this;
		}
		VarInt& operator%=(const VarInt& rhs)
		{
			VarInt divisor(rhs);
			divisor <<= highestbit() - divisor.highestbit();
			while (operator>=(rhs))
			{
				if (operator>=(divisor))
					*this -= divisor;
				divisor >>= 1;
			}
			return *this;
		}

		friend VarInt operator+(VarInt lhs, const VarInt& rhs)
		{
			lhs += rhs;
			return lhs;
		}
		friend VarInt operator-(VarInt lhs, const VarInt& rhs)
		{
			lhs -= rhs;
			return lhs;
		}
		friend VarInt operator*(VarInt lhs, const VarInt& rhs)
		{
			lhs *= rhs;
			return lhs;
		}
		friend VarInt operator/(VarInt lhs, const VarInt& rhs)
		{
			lhs /= rhs;
			return lhs;
		}
		friend VarInt operator%(VarInt lhs, const VarInt& rhs)
		{
			lhs %= rhs;
			return lhs;
		}

		pair<VarInt, VarInt> divrem(const VarInt& rhs)
		{
			if (rhs.isnull() == 1)
				throw invalid_argument("Divisor canot be zero!");
			else if (operator<(rhs))
				return pair<VarInt, VarInt>(0, *this);
			else
			{
				VarInt divident(*this);
				VarInt divisor(rhs);
				VarInt quotient;
				VarInt divisorshftd = divisor << (divident.highestbit() - divisor.highestbit());
				while (divisorshftd >= divisor)
				{
					if (divident >= divisorshftd)
					{
						divident -= divisorshftd;
						quotient.increment(1);
					}
					quotient <<= 1;
					divisorshftd >>= 1;
				}
				quotient >>= 1;
				return pair<VarInt, VarInt>(quotient, divident);
			}
		}

		// ====={ Indecr ops }=====
		VarInt& operator++()
		{
			return increment(1);
		}
		VarInt  operator++(int)
		{
			VarInt tmp(*this);
			operator++();
			return tmp;
		}
		VarInt& operator--()
		{
			return decrement(1);
		}
		VarInt  operator--(int)
		{
			VarInt tmp(*this);
			operator--();
			return tmp;
		}

		// ====={ Other math }=====
		VarInt  pow(VarInt n = 2) const
		{
			if (n == 0)
				return 1;
			else if (n == 1)
				return *this;
			else
			{
				VarInt x(*this);
				VarInt y(1);
				while (true)
				{
					if (n.getbit(0))
						y *= x;
					n >>= 1;
					if (n == 0)
						break;
					x *= x;
				}
				return y;
			}
		}
		VarInt root(VarInt n = 2) const
		{
			auto t1 = chrono::high_resolution_clock::now();

			// Base cases
			// done in this order, as 0th root of 0 is 0, of 1 is 1 and anything else gives infinity
			if (operator<=(1))
				return *this;
			if (n == 0)
				throw invalid_argument("Degree of root canot be zero!");

			// Do Binary Search for floor(sqrt(x))

			VarInt beg = VarInt(1) << (highestbit() / n);
			VarInt end = beg << 1;

			if (operator==(beg.pow(n)))
			{
				cout << "Beg is gud!" << endl;
				return beg;
			}
			if (operator==(end.pow(n)))
			{
				cout << "End is gud!" << endl;
				return end;
			}

			VarInt ans;
			while (beg <= end)
			{
				VarInt mid = (beg + end) >> 1;
				cout << "Mid:" << endl << mid.bin() << endl;

				int gtltoe = cmp(mid.pow(n));
				if (gtltoe == 0)
					return mid;
				else if (gtltoe > 0)
				{
					beg = mid + 1;
					ans = mid;
				}
				else //if (gtltoe < 0)
					end = mid - 1;
			}
			auto t2 = chrono::high_resolution_clock::now();
			auto duration = chrono::duration_cast<chrono::microseconds>(t2 - t1).count();
			cout << duration/1000 << "ms" << endl;
			return ans;
		}
		VarInt log2() const
		{
			return VarInt(highestbit());
		}

		// ====={ Print num }=====
		string hex() const
		{
			string result = /*string(pstv ? "+" : "-") +*/ "0x\n";
			for (size_t i = value.size(); i > 0; --i)
				result.append(num2hex(value.at(i - 1)) + '\n');
			return result;
		}
		string dec() const
		{
			string result = /*string(pstv ? "+" : "-") +*/ "0d\n";
			for (size_t i = value.size(); i > 0; --i)
				result.append(to_string(value.at(i - 1)) + '\n');
			return result;
		}
		string bin() const
		{
			string result = /*string(pstv ? "+" : "-") +*/ "0b\n";
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

	VarInt num(hex, 16);
	//VarInt num(2);

	VarInt snd(2);

	cout << "A:\n" << num.hex() << num.bin() << endl << endl;
	cout << "B:\n" << snd.hex() << snd.bin() << endl << endl;

	VarInt wyn = num.pow(7).root(13);

	cout << "Wyn:\n" << wyn.hex() << wyn.bin() << endl << endl;
}

