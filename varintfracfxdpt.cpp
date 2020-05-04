#include <algorithm> 
#include <cstdlib>
#include <iostream>
#include <limits>
#include <map>
#include <stdexcept>
#include <string>
#include <vector>

#include <chrono>

using namespace std;

typedef unsigned long long ull;
constexpr size_t unitbyte = sizeof(ull);
constexpr size_t fracunits = 10;

constexpr size_t unitbits = CHAR_BIT * unitbyte;
constexpr size_t fracbits = fracunits * unitbits;


template<typename Integral, typename enable_if<is_integral<Integral>::value, Integral>::type * = nullptr>
Integral ceildiv(Integral x, Integral y)
{
	return x / y + (x % y != 0);
}

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
vector<char>   str_explode(const string& str)
{
	vector<char> out;
	out.reserve(str.length());
	for (size_t i = 0; i < str.length(); ++i)
		out.push_back(str[i]);
	return out;
}

string num2bin(ull num)
{
	string temp;
	for (ull i = (ull)1 << (unitbits - 1); i != 0; i >>= 1)
		temp.push_back((num & i) ? '1' : '0');
	return temp;
}
string num2hex(ull num)
{
	string temp;
	for (ull i = ull(1) << (unitbits - 1); i != 0; i >>= 1)
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

template<typename T>
void tajp(const T& var)
{
	cout << typeid(var).name() << endl;
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
	class UVarInt
	{
	public:
		vector<ull> value;
		// ====={ Constructors }=====
		UVarInt() {}
		template<typename Integral, typename enable_if<is_integral<Integral>::value && !is_signed<Integral>::value, Integral>::type * = nullptr>
		UVarInt(Integral num)
		{
			if (num)
				value.push_back(num);
		}
		template<typename Integral, typename enable_if<is_integral<Integral>::value &&  is_signed<Integral>::value, Integral>::type * = nullptr>
		UVarInt(Integral num)
		{
			if (num)
				value.push_back(abs(num));
		}
		UVarInt(string num, size_t base = 16)
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
		UVarInt& reduce()
		{
			for (size_t i = value.size(); i > 0; --i)
				if (value.at(i - 1) == 0)
					value.pop_back();
				else
					break;
			return *this;
		}
		UVarInt& nullify()
		{
			value.clear();
			return *this;
		}
		bool isnull() const
		{
			return !static_cast<bool>(value.size());
		}

		ull gvoe(size_t pos) const
		{
			if (pos >= value.size())
				return 0;
			return value.at(pos);
		}

		bool operator!() const
		{
			return !static_cast<bool>(value.size());
		}

		UVarInt& increment(ull incr, size_t pos = 0)
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
		UVarInt& decrement(ull decr, size_t pos = 0)
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

		size_t highestbit() const
		{
			size_t r = 0;
			if (isnull())
				return -1;
			ull temp = value.back();
			ull mask = ull(1) << (unitbits - 1);
			while (!(temp & mask))
			{
				++r;
				temp <<= 1;
			}
			return unitbits * value.size() - r - 1;
		}
		size_t  lowestbit() const
		{
			if (value.size() == 0)
				return -1;
			size_t el = 0;
			while (el < value.size())
			{
				if (value.at(el))
					break;
				++el;
			}
			size_t r = 0;
			ull temp = value.at(el);
			while (!(temp & 1))
			{
				++r;
				temp >>= 1;
			}
			return unitbits * el + r;
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
		UVarInt  operator~() const
		{
			UVarInt temp;
			temp.value.resize(value.size());
			for (size_t i = temp.value.size(); i > 0; --i)
				temp.value.at(i - 1) = ~value.at(i - 1);
			temp.reduce();
			return temp;
		}

		UVarInt& operator<<=(size_t rhs)
		{
			if (rhs && !isnull())
			{
				size_t higbit = highestbit();
				size_t append = rhs / unitbits;
				size_t lshift = rhs % unitbits;
				size_t old = value.size();
				size_t higbyte = ceildiv(higbit + 1 + rhs, unitbits) - 1;
				value.resize(higbyte + 1);
				if (lshift == 0)
					for (size_t i = higbyte; i + 1 > 0; --i)
						value.at(i) = gvoe(i - append);
				else
					for (size_t i = higbyte; i + 1 > 0; --i)
						value.at(i) = gvoe(i - append) << lshift | gvoe(i - 1 - append) >> (unitbits - lshift);
			}
			return *this;
		}
		UVarInt& operator>>=(size_t rhs)
		{
			if (rhs && !isnull())
			{
				size_t higbit = highestbit();
				if (rhs > higbit)
					nullify();
				else
				{
					size_t remove = rhs / unitbits;
					size_t rshift = rhs % unitbits;
					size_t higbyte = ceildiv(higbit + 1 - rhs, unitbits) - 1;
					if (rshift == 0)
						for (size_t i = 0; i <= higbyte; ++i)
							value.at(i) = gvoe(i + remove);
					else
						for (size_t i = 0; i <= higbyte; ++i)
							value.at(i) = gvoe(i + remove) >> rshift | gvoe(i + remove + 1) << (unitbits - rshift);
					value.resize(higbyte + 1);
				}
			}
			return *this;
		}
		UVarInt  operator<< (size_t rhs) const
		{
			UVarInt temp(*this);
			temp.operator<<=(rhs);
			return temp;
		}
		UVarInt  operator>> (size_t rhs) const
		{
			UVarInt temp(*this);
			temp.operator>>=(rhs);
			return temp;
		}

		UVarInt& operator<<=(const UVarInt& rhs)
		{
			if (!rhs.isnull())
				operator<<=(rhs.value.at(0));
			return *this;
		}
		UVarInt& operator>>=(const UVarInt& rhs)
		{
			if (!rhs.isnull())
				operator>>=(rhs.value.at(0));
			return *this;
		}
		UVarInt  operator<< (const UVarInt& rhs) const
		{
			UVarInt temp(*this);
			temp.operator<<=(rhs);
			return temp;
		}
		UVarInt  operator>> (const UVarInt& rhs) const
		{
			UVarInt temp(*this);
			temp.operator>>=(rhs);
			return temp;
		}

		UVarInt& operator&=(const UVarInt& rhs)
		{
			if (isnull() || rhs.isnull())
				nullify();
			else
			{
				if (value.size() > rhs.value.size())
					value.resize(rhs.value.size());
				for (size_t i = 0; i < value.size(); ++i)
					value.at(i) &= rhs.gvoe(i);
				reduce();
			}
			return *this;
		}
		UVarInt& operator|=(const UVarInt& rhs)
		{
			if (isnull())
				value = rhs.value;
			else if (rhs.isnull()) {}
			else
			{
				if (value.size() < rhs.value.size())
					value.resize(rhs.value.size());
				for (size_t i = 0; i < value.size(); ++i)
					value.at(i) |= rhs.gvoe(i);
//				reduce();
			}
			return *this;
		}
		UVarInt& operator^=(const UVarInt& rhs)
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
		friend UVarInt operator&(UVarInt lhs, const UVarInt& rhs)
		{
			lhs &= rhs;
			return lhs;
		}
		friend UVarInt operator|(UVarInt lhs, const UVarInt& rhs)
		{
			lhs |= rhs;
			return lhs;
		}
		friend UVarInt operator^(UVarInt lhs, const UVarInt& rhs)
		{
			lhs ^= rhs;
			return lhs;
		}

		// ====={ Comparison ops }=====
		bool operator> (const UVarInt& rhs) const
		{
			return cmp(rhs) > 0;
		}
		bool operator>=(const UVarInt& rhs) const
		{
			return cmp(rhs) >= 0;
		}
		bool operator< (const UVarInt& rhs) const
		{
			return cmp(rhs) < 0;
		}
		bool operator<=(const UVarInt& rhs) const
		{
			return cmp(rhs) <= 0;
		}
		bool operator==(const UVarInt& rhs) const
		{
			return cmp(rhs) == 0;
		}
		bool operator!=(const UVarInt& rhs) const
		{
			return cmp(rhs) != 0;
		}
		int cmp(const UVarInt& rhs) const
		{
			size_t maxize = max(value.size(), rhs.value.size());
			for (size_t i = maxize; i > 0; --i)
			{
				if (gvoe(i - 1) > rhs.gvoe(i - 1))
					return 1;
				if (gvoe(i - 1) < rhs.gvoe(i - 1))
					return -1;
			}
			return 0;
		}

		// ====={ Math ops }=====
		UVarInt& operator+=(const UVarInt& rhs)
		{
			if (isnull() == 1)
				value = rhs.value;
			else if (rhs.isnull() == 1)
			{
			}
			else
				for (size_t i = 0; i < rhs.value.size(); ++i)
					increment(rhs.gvoe(i), i);
			reduce();
			return *this;
		}
		UVarInt& operator-=(const UVarInt& rhs)
		{
			int ltgtoe = cmp(rhs);
			if (ltgtoe == 0)
				nullify();
			else if (ltgtoe > 0)
				for (size_t i = 0; i < rhs.value.size(); ++i)
					decrement(rhs.gvoe(i), i);
			else if (ltgtoe < 0)
			{
				UVarInt subtrahend(*this);
				value = rhs.value;
				for (size_t i = 0; i < subtrahend.value.size(); ++i)
					decrement(subtrahend.gvoe(i), i);
			}
			reduce();
			return *this;
		}
		UVarInt& operator*=(const UVarInt& rhs)
		{
			if (isnull() || rhs.isnull())
				nullify();
			else if (operator==(1))
				value = rhs.value;
			else if (rhs.operator==(1))
			{
			}
			else
			{
				UVarInt multiplicand;
				UVarInt multiplier;
				size_t lhshigbit = highestbit();
				size_t lhslowbit = lowestbit();
				size_t rhshigbit = rhs.highestbit();
				size_t rhslowbit = rhs.lowestbit();
				size_t mpdhigbit, mprhigbit, mprlowbit;
				if (lhshigbit - lhslowbit >= rhshigbit - rhslowbit)
				{
					multiplicand = *this;
					multiplier = rhs;
					mpdhigbit = lhshigbit; mprhigbit = rhshigbit; mprlowbit = rhslowbit;
				}
				else
				{
					multiplicand = rhs;
					multiplier = *this;
					mpdhigbit = rhshigbit; mprhigbit = lhshigbit; mprlowbit = lhslowbit;
				}
				multiplier >>= mprlowbit;
				multiplicand <<= mprlowbit;
				mpdhigbit += mprlowbit; mprhigbit -= mprlowbit; mprlowbit = 0;

				nullify();
				value.resize((mprhigbit + mpdhigbit - 1) / unitbits + 1);

				for (size_t b = 0; b < mprhigbit; ++b)
				{
					if (multiplier.getbit(b))
						operator+=(multiplicand);
					multiplicand <<= 1;
				}
				operator+=(multiplicand);
				//				reduce();
			}
			return *this;
		}
		UVarInt& operator/=(const UVarInt& rhs)
		{
			if (rhs.isnull() == 1)
				throw invalid_argument("Divisor canot be zero!");
			else if (operator<(rhs))
				nullify();
			else
			{
				UVarInt divident(*this);
				UVarInt divisor(rhs);
				size_t shift = min(divident.lowestbit(), divisor.lowestbit());
				divident >>= shift;
				divisor >>= shift;
				UVarInt divisorshftd = divisor << (divident.highestbit() - divisor.highestbit());
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
		UVarInt& operator%=(const UVarInt& rhs)
		{
			UVarInt divisor(rhs);
			divisor <<= highestbit() - divisor.highestbit();
			while (operator>=(rhs))
			{
				if (operator>=(divisor))
					operator-=(divisor);
				divisor >>= 1;
			}
			return *this;
		}

		friend UVarInt operator+(UVarInt lhs, const UVarInt& rhs)
		{
			lhs += rhs;
			return lhs;
		}
		friend UVarInt operator-(UVarInt lhs, const UVarInt& rhs)
		{
			lhs -= rhs;
			return lhs;
		}
		friend UVarInt operator*(UVarInt lhs, const UVarInt& rhs)
		{
			lhs *= rhs;
			return lhs;
		}
		friend UVarInt operator/(UVarInt lhs, const UVarInt& rhs)
		{
			lhs /= rhs;
			return lhs;
		}
		friend UVarInt operator%(UVarInt lhs, const UVarInt& rhs)
		{
			lhs %= rhs;
			return lhs;
		}

		pair<UVarInt, UVarInt> divrem(const UVarInt& rhs)
		{
			if (rhs.isnull() == 1)
				throw invalid_argument("Divisor canot be zero!");
			else if (operator<(rhs))
				return pair<UVarInt, UVarInt>(0, *this);
			else
			{
				UVarInt divident(*this);
				UVarInt divisor(rhs);
				UVarInt quotient;
				UVarInt divisorshftd = divisor << (divident.highestbit() - divisor.highestbit());
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
				return pair<UVarInt, UVarInt>(quotient, divident);
			}
		}
		UVarInt operator- () const
		{
			return *this;
		}

		// ====={ Indecr ops }=====
		UVarInt& operator++()
		{
			return increment(1);
		}
		UVarInt& operator--()
		{
			return decrement(1);
		}
		UVarInt  operator++(int)
		{
			UVarInt tmp(*this);
			operator++();
			return tmp;
		}
		UVarInt  operator--(int)
		{
			UVarInt tmp(*this);
			operator--();
			return tmp;
		}

		// ====={ Other math }=====
		UVarInt  pow(UVarInt n = 2) const
		{
			if (n.isnull())
				return 1;
			else if (n == 1)
				return *this;
			else
			{
				UVarInt x(*this);
				UVarInt y(1);
				size_t lastb = n.highestbit();
				for (size_t b = 0; b < lastb; ++b)
				{
					if (n.getbit(b))
						y *= x;
					x *= x;
				}
				y *= x;
				return y;
			}
		}
		UVarInt root(UVarInt n = 2) const
		{
			// Base cases
			// done in this order, as 0th root of 0 is 0, of 1 is 1 and anything else gives infinity
			if (operator<=(1))
				return *this;
			if (n == 0)
				throw invalid_argument("Degree of root canot be zero!");

			// Do Binary Search for floor(sqrt(x))

			UVarInt beg = UVarInt(1) << (highestbit() / n);
			UVarInt end = beg << 1;

			if (operator==(beg.pow(n)))
				return beg;
			if (operator==(end.pow(n)))
				return end;

			UVarInt ans;
			while (beg <= end)
			{
				UVarInt mid = (beg + end) >> 1;
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
			return ans;
		}
		UVarInt log2() const
		{
			return UVarInt(highestbit());
		}

		UVarInt gcd(UVarInt v)
		{
			if (v.isnull())
				return *this;
			if (isnull() || operator==(v))
				return v;
			UVarInt u(*this);
			size_t shift = min(u.lowestbit(), v.lowestbit());
			v >>= shift;
			u >>= u.lowestbit();
			do
			{
				v >>= v.lowestbit();
				if (u > v)
					swap(u, v);
				v -= u;
			}
			while (!v.isnull());
			return u << shift;
		}
		UVarInt lcm(UVarInt v)
		{
			if (isnull() || v.isnull())
				return 0;
			if (operator==(v))
				return v;
			return (*this) / gcd(v) * v;
		}
		pair<UVarInt, pair<UVarInt, UVarInt>> lcmcoeffs(UVarInt v)
		{
			if (isnull() || v.isnull())
				return { 0, {0, 0} };
			if (operator==(v))
				return { v, {1, 1} };
			UVarInt dgcd = gcd(v);
			return { *this / dgcd * v, {*this / dgcd, v / dgcd} };
		}

		// ====={ Print num }=====
		string hex(string sep = "\n") const
		{
			string result = "0x" + sep;
			for (size_t i = value.size(); i > 0; --i)
				result.append(num2hex(value.at(i - 1)) + sep);
			return result;
		}
		string dec(string sep = "\n") const
		{
			string result = "0d" + sep;
			for (size_t i = value.size(); i > 0; --i)
				result.append(to_string(value.at(i - 1)) + sep);
			return result;
		}
		string bin(string sep = "\n") const
		{
			string result = "0b" + sep;
			for (size_t i = value.size(); i > 0; --i)
				result.append(num2bin(value.at(i - 1)) + sep);
			return result;
		}
	};

	class SVarInt : public UVarInt
	{
	public:
		bool negative = false;
		// ====={ Constructors }=====
		SVarInt() {}
		template<typename Integral, typename enable_if<is_integral<Integral>::value && !is_signed<Integral>::value, Integral>::type * = nullptr>
		SVarInt(Integral num) : UVarInt(num)
		{
		}
		template<typename Integral, typename enable_if<is_integral<Integral>::value &&  is_signed<Integral>::value, Integral>::type * = nullptr>
		SVarInt(Integral num) : UVarInt(num)
		{
			negative = (num < 0);
		}
		SVarInt(string num, size_t base = 16, bool neg = false) : UVarInt(num, base)
		{
			negative = neg;
		}
		SVarInt(UVarInt num, bool neg = false) : UVarInt(num)
		{
			negative = neg;
		}

		// ====={ Operational }=====
		SVarInt& reduce()
		{
			UVarInt::reduce();
			if (isnull())
				negative = false;
			return *this;
		}
		SVarInt& nullify()
		{
			UVarInt::nullify();
			negative = false;
			return *this;
		}
		int  sign() const
		{
			if (isnull())
				return 0;
			return 1 - 2 * negative;
		}

		// ====={ Bitwise ops }=====
		SVarInt  operator~() const
		{
			SVarInt temp(UVarInt::operator~());
			temp.negative = !negative;
			return temp;
		}

		SVarInt& operator<<=(size_t rhs)
		{
			UVarInt::operator<<=(rhs);
			return *this;
		}
		SVarInt& operator>>=(size_t rhs)
		{
			UVarInt::operator>>=(rhs);
			return *this;
		}
		SVarInt  operator<< (size_t rhs) const
		{
			SVarInt temp(*this);
			temp.operator<<=(rhs);
			return temp;
		}
		SVarInt  operator>> (size_t rhs) const
		{
			SVarInt temp(*this);
			temp.operator>>=(rhs);
			return temp;
		}

		SVarInt& operator<<=(const SVarInt& rhs)
		{
			switch (rhs.sign())
			{
			case 1:
				operator<<=(rhs.gvoe(0));
				break;
			case -1:
				operator>>=(rhs.gvoe(0));
			}
			return *this;
		}
		SVarInt& operator>>=(const SVarInt& rhs)
		{
			switch (rhs.sign())
			{
			case 1:
				operator>>=(rhs.gvoe(0));
				break;
			case -1:
				operator<<=(rhs.gvoe(0));
			}
			return *this;
		}
		SVarInt  operator<< (const SVarInt& rhs) const
		{
			SVarInt temp(*this);
			temp.operator<<=(rhs);
			return temp;
		}
		SVarInt  operator>> (const SVarInt& rhs) const
		{
			SVarInt temp(*this);
			temp.operator>>=(rhs);
			return temp;
		}

		SVarInt& operator&=(const SVarInt& rhs)
		{
			negative &= rhs.negative;
			UVarInt::operator&=(rhs);
			return *this;
		}
		SVarInt& operator|=(const SVarInt& rhs)
		{
			negative |= rhs.negative;
			UVarInt::operator|=(rhs);
			return *this;
		}
		SVarInt& operator^=(const SVarInt& rhs)
		{
			negative ^= rhs.negative;
			UVarInt::operator^=(rhs);
			return *this;
		}
		friend SVarInt operator&(SVarInt lhs, const SVarInt& rhs)
		{
			lhs &= rhs;
			return lhs;
		}
		friend SVarInt operator|(SVarInt lhs, const SVarInt& rhs)
		{
			lhs |= rhs;
			return lhs;
		}
		friend SVarInt operator^(SVarInt lhs, const SVarInt& rhs)
		{
			lhs ^= rhs;
			return lhs;
		}

		// ====={ Comparison ops }=====
		bool operator> (const SVarInt& rhs) const
		{
			return cmp(rhs) > 0;
		}
		bool operator>=(const SVarInt& rhs) const
		{
			return cmp(rhs) >= 0;
		}
		bool operator< (const SVarInt& rhs) const
		{
			return cmp(rhs) < 0;
		}
		bool operator<=(const SVarInt& rhs) const
		{
			return cmp(rhs) <= 0;
		}
		bool operator==(const SVarInt& rhs) const
		{
			return cmp(rhs) == 0;
		}
		bool operator!=(const SVarInt& rhs) const
		{
			return cmp(rhs) != 0;
		}
		int cmp(const SVarInt& rhs) const
		{
			int lsgn = sign();
			int rsgn = rhs.sign();
			if (lsgn == 0 && rsgn == 0)
				return 0;
			else if (lsgn < rsgn)
				return -1;
			else if (lsgn > rsgn)
				return 1;
			return sign() * UVarInt::cmp(rhs);
		}

		// ====={ Math ops }=====
		SVarInt& operator+=(const SVarInt& rhs)
		{
			if (negative == rhs.negative)
			{
				UVarInt::operator+=(rhs);
			}
			else
			{
				UVarInt::operator-=(rhs);
				if (UVarInt::operator<(rhs))
					negative = rhs.negative;
			}
			reduce();
			return *this;
		}
		SVarInt& operator-=(const SVarInt& rhs)
		{
			if (negative == rhs.negative)
			{
				UVarInt::operator-=(rhs);
				if (UVarInt::operator<(rhs))
					negative = !rhs.negative;
			}
			else
			{
				UVarInt::operator+=(rhs);
			}
			reduce();
			return *this;
		}
		SVarInt& operator*=(const SVarInt& rhs)
		{
			UVarInt::operator*=(rhs);
			negative = (negative != rhs.negative);
			return *this;
		}
		SVarInt& operator/=(const SVarInt& rhs)
		{
			UVarInt::operator/=(rhs);
			negative = (negative != rhs.negative);
			reduce();
			return *this;
		}
		SVarInt& operator%=(const SVarInt& rhs)
		{
			UVarInt::operator%=(rhs);
			reduce();
			return *this;
		}

		friend SVarInt operator+(SVarInt lhs, const SVarInt& rhs)
		{
			lhs += rhs;
			return lhs;
		}
		friend SVarInt operator-(SVarInt lhs, const SVarInt& rhs)
		{
			lhs -= rhs;
			return lhs;
		}
		friend SVarInt operator*(SVarInt lhs, const SVarInt& rhs)
		{
			lhs *= rhs;
			return lhs;
		}
		friend SVarInt operator/(SVarInt lhs, const SVarInt& rhs)
		{
			lhs /= rhs;
			return lhs;
		}
		friend SVarInt operator%(SVarInt lhs, const SVarInt& rhs)
		{
			lhs %= rhs;
			return lhs;
		}

		pair<SVarInt, SVarInt> divrem(const SVarInt& rhs)
		{
			return UVarInt::divrem(rhs);
		}
		SVarInt  operator-() const
		{
			SVarInt temp(*this);
			temp.negative = !negative;
			return temp;
		}

		// ====={ Indecr ops }=====
		SVarInt& operator++()
		{
			int sgn = sign();
			if (sgn == -1)
				decrement(1);
			else // sgn == [0|1]
				increment(1);
			reduce();
			return *this;
		}
		SVarInt& operator--()
		{
			int sgn = sign();
			if (sgn == 0)
			{
				negative = true;
				increment(1);
			}
			else if (sgn == -1)
				increment(1);
			else // if sgn == 1
				decrement(1);
			reduce();
			return *this;
		}
		SVarInt  operator++(int)
		{
			UVarInt tmp(*this);
			operator++();
			return tmp;
		}
		SVarInt  operator--(int)
		{
			UVarInt tmp(*this);
			operator--();
			return tmp;
		}

		// ====={ Other math }=====
		SVarInt  pow(SVarInt n = 2) const
		{
			if (n.negative)
				throw invalid_argument("Cannot calculate power with negative exponent!");
			return SVarInt(UVarInt::pow(n), n.getbit(0) ? negative : 0);
		}
		SVarInt root(SVarInt n = 2) const
		{
			if (n.negative)
				throw invalid_argument("Cannot calculate root with negative degree!");
			if (negative && !n.getbit(0))
				throw invalid_argument("Cannot calculate even root of negative number!");
			return SVarInt(UVarInt::root(n), negative);
		}
		UVarInt log2() const
		{
			if (negative)
				throw invalid_argument("Cannot calculate logarithm of negative number!");
			return UVarInt::log2();
		}

		UVarInt abs()
		{
			return static_cast<UVarInt>(*this);
		}

		// ====={ Print num }=====
		string hex(string sep = "\n") const
		{
			return string(negative ? "-" : "+") + UVarInt::hex(sep);
		}
		string dec(string sep = "\n") const
		{
			return string(negative ? "-" : "+") + UVarInt::dec(sep);
		}
		string bin(string sep = "\n") const
		{
			return string(negative ? "-" : "+") + UVarInt::bin(sep);
		}
	};

	class RationFrac
	{
	public:
		UVarInt numerator = 0;
		UVarInt denominator = 1;
		bool negative = 0;
		// ====={ Constructors }=====
		RationFrac()
		{}
		template<typename Integral, typename enable_if<is_integral<Integral>::value && !is_signed<Integral>::value, Integral>::type * = nullptr>
		RationFrac(Integral num)
		{
			numerator = num;
		}
		template<typename Integral, typename enable_if<is_integral<Integral>::value &&  is_signed<Integral>::value, Integral>::type * = nullptr>
		RationFrac(Integral num)
		{
			numerator = num;
		}
		template<typename Floating, typename enable_if<is_floating_point<Floating>::value, Floating>::type * = nullptr>
		RationFrac(Floating num)
		{
			if (isnan(num))
				throw new invalid_argument("Cannot create fraction from NaN!");
			if (isinf(num))
				throw new invalid_argument("Cannot create fraction from Inf!");
			if (num < 0)
			{
				negative = true;
				num = fabs(num);
			}
			if (num != 0)
			{
				int exp;
				long double mantissa = frexp(num, &exp);
				mantissa = ldexp(mantissa, numeric_limits<Floating>::digits);
				numerator = static_cast<ull>(mantissa);
				exp -= numeric_limits<Floating>::digits;
				if (exp > 0)
					numerator <<= exp;
				else
					denominator <<= -exp;
				reduce();
			}
		}
		RationFrac(SVarInt num)
		{
			bool neg = false;
			numerator = num;
			negative = (num.negative != neg); // 2-in XOR
		}
		RationFrac(SVarInt num, SVarInt den, bool neg = false)
		{
			numerator = num.abs();
			denominator = den.abs();
			negative = ((num.negative != den.negative) != neg); // 3-in XOR
			reduce();
		}

		// ====={ Operational }=====
		RationFrac& reduce()
		{
//			numerator.reduce();
//			denominator.reduce();
			UVarInt gcd = numerator.gcd(denominator);
			numerator /= gcd;
			denominator /= gcd;
			return *this;
		}
		RationFrac& nullify()
		{
			numerator.nullify();
			denominator = 1;
			return *this;
		}
		bool isnull() const
		{
			return numerator.isnull();
		}
		int  sign() const
		{
			if (isnull())
				return 0;
			return 1 - 2 * negative;
		}

		// ====={ Comparison ops }=====
		bool operator> (const RationFrac& rhs) const
		{
			return cmp(rhs) > 0;
		}
		bool operator>=(const RationFrac& rhs) const
		{
			return cmp(rhs) >= 0;
		}
		bool operator< (const RationFrac& rhs) const
		{
			return cmp(rhs) < 0;
		}
		bool operator<=(const RationFrac& rhs) const
		{
			return cmp(rhs) <= 0;
		}
		bool operator==(const RationFrac& rhs) const
		{
			return cmp(rhs) == 0;
		}
		bool operator!=(const RationFrac& rhs) const
		{
			return cmp(rhs) != 0;
		}
		int cmp(const RationFrac& rhs) const
		{
			int lsgn = sign();
			int rsgn = rhs.sign();
			if (lsgn == 0 && rsgn == 0)
				return 0;
			else if (lsgn < rsgn)
				return -1;
			else if (lsgn > rsgn)
				return 1;
			//else lsgn == rsgn
			int numcmp = numerator.cmp(rhs.numerator);
			int dencmp = rhs.denominator.cmp(denominator); // notice reversed order for speed & simplicity
			if (dencmp == 0)
				return lsgn * numcmp;
			if (numcmp == 0)
				return lsgn * dencmp;
			if (numcmp == dencmp)
				return lsgn * numcmp;
			return lsgn * (numerator * rhs.denominator).cmp(rhs.numerator * denominator);
		}

		// ====={ Math ops }=====

		RationFrac& operator+=(const RationFrac& rhs)
		{
			pair<UVarInt, pair<UVarInt, UVarInt>> lcms = denominator.lcmcoeffs(rhs.denominator);
			numerator *= lcms.second.second;
			UVarInt rhsnum = rhs.numerator * lcms.second.first;
			if (negative == rhs.negative)
				numerator += rhsnum;
			else
			{
				negative = (numerator >= rhsnum) ? negative : rhs.negative;
				numerator -= rhsnum;
			}
			denominator = lcms.first;
			reduce();
			return *this;
		}
		RationFrac& operator-=(const RationFrac& rhs)
		{
			pair<UVarInt, pair<UVarInt, UVarInt>> lcms = denominator.lcmcoeffs(rhs.denominator);
			numerator *= lcms.second.second;
			UVarInt rhsnum = rhs.numerator * lcms.second.first;
			if (negative != rhs.negative)
				numerator += rhsnum;
			else
			{
				negative = (numerator >= rhsnum) ? negative : !rhs.negative;
				numerator -= rhsnum;
			}
			denominator = lcms.first;
			reduce();
			return *this;
		}
		RationFrac& operator*=(const RationFrac& rhs)
		{
			UVarInt ndgcd = numerator.gcd(rhs.denominator);
			numerator /= ndgcd;
			denominator *= rhs.denominator / ndgcd;
			ndgcd = denominator.gcd(rhs.numerator);
			denominator /= ndgcd;
			numerator *= rhs.numerator / ndgcd;
			negative = (negative != rhs.negative);
			return *this;
		}
		RationFrac& operator/=(const RationFrac& rhs)
		{
			UVarInt ndgcd = numerator.gcd(rhs.numerator);
			numerator /= ndgcd;
			denominator *= rhs.numerator / ndgcd;
			ndgcd = denominator.gcd(rhs.denominator);
			denominator /= ndgcd;
			numerator *= rhs.denominator / ndgcd;
			negative = (negative != rhs.negative);
			return *this;
		}

		friend RationFrac operator+(RationFrac lhs, const RationFrac& rhs)
		{
			lhs += rhs;
			return lhs;
		}
		friend RationFrac operator-(RationFrac lhs, const RationFrac& rhs)
		{
			lhs -= rhs;
			return lhs;
		}
		friend RationFrac operator*(RationFrac lhs, const RationFrac& rhs)
		{
			lhs *= rhs;
			return lhs;
		}
		friend RationFrac operator/(RationFrac lhs, const RationFrac& rhs)
		{
			lhs /= rhs;
			return lhs;
		}

		RationFrac  operator-() const
		{
			RationFrac temp(*this);
			temp.negative = !negative;
			return temp;
		}

		// ====={ Indecr ops }=====
		RationFrac& operator++()
		{
			if (negative)
			{
				if (denominator > numerator)
					negative = false;
				numerator -= denominator;
			}
			else // sgn == [0|1]
				numerator += denominator;
			return *this;
		}
		RationFrac& operator--()
		{
			if (negative)
				numerator += denominator;
			else // sgn == [0|1]
			{
				if (denominator > numerator)
					negative = true;
				numerator -= denominator;
			}
			reduce();
			return *this;
		}
		RationFrac  operator++(int)
		{
			RationFrac tmp(*this);
			operator++();
			return tmp;
		}
		RationFrac  operator--(int)
		{
			RationFrac tmp(*this);
			operator--();
			return tmp;
		}

		// ====={ Other math }=====
		RationFrac  pow(SVarInt n = 2) const
		{
			if (n.negative)
				throw invalid_argument("Cannot calculate power with negative exponent!");
			return RationFrac(numerator.pow(n), denominator.pow(n), n.getbit(0) ? negative : 0);
		}

		RationFrac abs()
		{
			RationFrac temp(*this);
			temp.negative = false;
			return temp;
		}
		RationFrac inv()
		{
			if (isnull())
				throw new logic_error("Cannot inverse the Zero!");
			RationFrac temp(denominator, numerator);
			temp.negative = negative;
			return temp;
		}

		// ====={ Print num }=====
		string hex(string sep = "\n") const
		{
			return string(negative ? "-" : "+") + numerator.hex(sep) + "----------------" + sep + denominator.hex(sep);
		}
		string dec(string sep = "\n") const
		{
			return string(negative ? "-" : "+") + numerator.dec(sep) + "--------------------" + sep + denominator.dec(sep);
		}
		string bin(string sep = "\n") const
		{
			return string(negative ? "-" : "+") + numerator.bin(sep) + "----------------------------------------------------------------" + sep + denominator.bin(sep);
		}
	};

	class FixedPFrac : public SVarInt
	{
	public:
		// ====={ Constructors }=====
		FixedPFrac() {}
/*		FixedPFrac(UVarInt num) : SVarInt(num)
		{
			operator<<=(fracbits);
		} */
		FixedPFrac(SVarInt num) : SVarInt(num)
		{
			operator<<=(fracbits);
		}

		// ====={ Bitwise ops }=====
		FixedPFrac  operator~() const
		{
			FixedPFrac temp(UVarInt::operator~());
			return temp;
		}

		FixedPFrac& operator<<=(size_t rhs)
		{
			SVarInt::operator<<=(rhs);
			return *this;
		}
		FixedPFrac& operator>>=(size_t rhs)
		{
			SVarInt::operator>>=(rhs);
			return *this;
		}
		FixedPFrac  operator<< (size_t rhs) const
		{
			FixedPFrac temp(*this);
			temp.operator<<=(rhs);
			return temp;
		}
		FixedPFrac  operator>> (size_t rhs) const
		{
			FixedPFrac temp(*this);
			temp.operator>>=(rhs);
			return temp;
		}

		FixedPFrac& operator<<=(const SVarInt& rhs)
		{
			switch (rhs.sign())
			{
			case 1:
				operator<<=(rhs.gvoe(0));
				break;
			case -1:
				operator>>=(rhs.gvoe(0));
			}
			return *this;
		}
		FixedPFrac& operator>>=(const SVarInt& rhs)
		{
			switch (rhs.sign())
			{
			case 1:
				operator>>=(rhs.gvoe(0));
				break;
			case -1:
				operator<<=(rhs.gvoe(0));
			}
			return *this;
		}
		FixedPFrac  operator<< (const SVarInt& rhs) const
		{
			FixedPFrac temp(*this);
			temp.operator<<=(rhs);
			return temp;
		}
		FixedPFrac  operator>> (const SVarInt& rhs) const
		{
			FixedPFrac temp(*this);
			temp.operator>>=(rhs);
			return temp;
		}

		FixedPFrac& operator&=(const FixedPFrac& rhs)
		{
			SVarInt::operator&=(rhs);
			return *this;
		}
		FixedPFrac& operator|=(const FixedPFrac& rhs)
		{
			SVarInt::operator|=(rhs);
			return *this;
		}
		FixedPFrac& operator^=(const FixedPFrac& rhs)
		{
			SVarInt::operator^=(rhs);
			return *this;
		}

		friend FixedPFrac operator&(FixedPFrac lhs, const FixedPFrac& rhs)
		{
			lhs &= rhs;
			return lhs;
		}
		friend FixedPFrac operator|(FixedPFrac lhs, const FixedPFrac& rhs)
		{
			lhs |= rhs;
			return lhs;
		}
		friend FixedPFrac operator^(FixedPFrac lhs, const FixedPFrac& rhs)
		{
			lhs ^= rhs;
			return lhs;
		}

		// ====={ Math ops }=====
		FixedPFrac& operator+=(const FixedPFrac& rhs)
		{
			SVarInt::operator+=(rhs);
			return *this;
		}
		FixedPFrac& operator-=(const FixedPFrac& rhs)
		{
			SVarInt::operator-=(rhs);
			return *this;
		}
		FixedPFrac& operator*=(const FixedPFrac& rhs)
		{
			SVarInt::operator*=(rhs);
			operator>>=(fracbits);
			return *this;
		}
		FixedPFrac& operator/=(const FixedPFrac& rhs)
		{
			operator<<=(fracbits);
			SVarInt::operator/=(rhs);
			return *this;
		}
		FixedPFrac& operator%=(const FixedPFrac& rhs)
		{
			operator<<=(fracbits);
			SVarInt::operator%=(rhs);
			return *this;
		}

		friend FixedPFrac operator+(FixedPFrac lhs, const FixedPFrac& rhs)
		{
			lhs += rhs;
			return lhs;
		}
		friend FixedPFrac operator-(FixedPFrac lhs, const FixedPFrac& rhs)
		{
			lhs -= rhs;
			return lhs;
		}
		friend FixedPFrac operator*(FixedPFrac lhs, const FixedPFrac& rhs)
		{
			lhs *= rhs;
			return lhs;
		}
		friend FixedPFrac operator/(FixedPFrac lhs, const FixedPFrac& rhs)
		{
			lhs /= rhs;
			return lhs;
		}
		friend FixedPFrac operator%(FixedPFrac lhs, const FixedPFrac& rhs)
		{
			lhs %= rhs;
			return lhs;
		}

		pair<FixedPFrac, FixedPFrac> divrem(const FixedPFrac& rhs)
		{
			FixedPFrac temp(*this);
			temp  <<= fracbits;
			return temp.SVarInt::divrem(rhs);
		}

		// ====={ Indecr ops }=====
		FixedPFrac& operator++()
		{
			FixedPFrac one(1);
			operator+=(one);
			return *this;
		}
		FixedPFrac& operator--()
		{
			FixedPFrac one(1);
			operator-=(one);
			return *this;
		}
		FixedPFrac  operator++(int)
		{
			FixedPFrac tmp(*this);
			operator++();
			return tmp;
		}
		FixedPFrac  operator--(int)
		{
			FixedPFrac tmp(*this);
			operator--();
			return tmp;
		}

		// ====={ Other math }=====
		FixedPFrac  pow(SVarInt n = 2) const
		{
			FixedPFrac temp(SVarInt::pow(n));
			temp >>= (n * fracbits);
			return temp;
		}
		FixedPFrac root(SVarInt n = 2) const
		{
			FixedPFrac temp(operator<<((n-1) * fracbits));
			tajp(temp);
			cout << "Shifted: " << temp.hex() << endl;
			tajp(temp.SVarInt::root(n));
			FixedPFrac temp2 = temp.SVarInt::root(n);
			return temp2 >> fracbits;
		}
/*		FixedPFrac log2() const
		{
			if (negative)
				throw invalid_argument("Cannot calculate logarithm of negative number!");
			return UVarInt::log2();
		}//*/

		FixedPFrac abs()
		{
			FixedPFrac temp(*this);
			temp.negative = false;
			return temp;
		}

		// ====={ Print num }=====
		string hex(string sep = "\n") const
		{
			return string(negative ? "-" : "+") + UVarInt::hex(sep);
		}
		string dec(string sep = "\n") const
		{
			return string(negative ? "-" : "+") + UVarInt::dec(sep);
		}
		string bin(string sep = "\n") const
		{
			return string(negative ? "-" : "+") + UVarInt::bin(sep);
		}

	};
}

using namespace Meth;





int main()
{
	auto t1 = chrono::high_resolution_clock::now();

	//*
	string hex = "0123456789ABCDEFFEDCBA9876543210FFF2FDAFF3FFF9FC";
	string oct = "22150531704653633677766713523035452062041777777777777777777777";
	string bin = "0111010101011101010";

	//UVarInt num(hex, 16);
	FixedPFrac num(5);
	FixedPFrac snd(3);


//	cout << "A:\n" << A.hex() << endl;// << A.bin() << endl << endl;
//	cout << "B:\n" << B.hex() << endl;// << B.bin() << endl << endl;

	FixedPFrac wyn = num.root(2).pow(2);

	tajp(wyn);

	cout << "W:\n" << wyn.hex() << endl << wyn.bin() << endl << endl;


	//*/

	auto t2 = chrono::high_resolution_clock::now();
	auto duration = chrono::duration_cast<chrono::microseconds>(t2 - t1).count();
	cout << endl << duration << "us" << endl;

}


/*

	auto t1 = chrono::high_resolution_clock::now();

	auto t2 = chrono::high_resolution_clock::now();
	auto duration = chrono::duration_cast<chrono::microseconds>(t2 - t1).count();
	cout << duration / 1000 << "ms" << endl;

*/
