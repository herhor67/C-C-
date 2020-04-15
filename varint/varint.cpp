#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <map>
#include <stdexcept>
#include <string>
#include <vector>

#include <chrono>

using namespace std;

typedef unsigned long long ull;
typedef long long sll;
constexpr unsigned unitbyte = sizeof(ull);
constexpr unsigned unitbits = 8 * unitbyte;

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
		template<typename Integral, typename enable_if<is_integral<Integral>::value && !is_same<Integral, bool>::value && !is_signed<Integral>::value, Integral>::type * = nullptr>
		UVarInt(Integral num)
		{
			if (num)
				value.push_back(num);
		}
		template<typename Integral, typename enable_if<is_integral<Integral>::value && !is_same<Integral, bool>::value && is_signed<Integral>::value, Integral>::type * = nullptr>
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
		bool    isnull() const
		{
			return !static_cast<bool>(value.size());
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
			if (rhs && value.size())
			{
				size_t append = rhs / unitbits;
				size_t lshift = rhs % unitbits;
				size_t old = value.size() + append;
				value.resize(value.size() + append + !!lshift);
				if (lshift)
					for (size_t i = value.size(); i > 0; --i)
					{
						size_t p1 = i - 1;
						size_t p2 = i - 2;
						value.at(p1) = ((p1 <= old && p1 >= append) ? value.at(p1 - append) << lshift : 0) | ((p2 <= old && p2 >= append) ? value.at(p2 - append) >> (unitbits - lshift) : 0);
					}
				else
					for (size_t i = value.size() - 1; i > 0; --i)
						value.at(i) = gvoe(value, i - append);
				reduce();
			}
			return *this;
		}
		UVarInt& operator>>=(size_t rhs)
		{
			if (rhs && !isnull())
			{
				size_t higbit = highestbit();
				if (higbit < rhs)
					nullify();
				else
				{
					size_t remove = rhs / unitbits;
					size_t rshift = rhs % unitbits;
					size_t newsize = value.size() - remove - (rshift > higbit% unitbits);
					if (rshift == 0)
						for (size_t i = 0; i < newsize; ++i)
							value.at(i) = value.at(i + remove);
						//	value.at(i) = gvoe(value, i + remove);
					else
						for (size_t i = 0; i < newsize; ++i)
							value.at(i) = value.at(i + remove) >> rshift | gvoe(value, i + remove + 1) << (unitbits - rshift);
						//	value.at(i) = gvoe(value, i + remove) >> rshift | gvoe(value, i + 1 + remove) << (unitbits - rshift);
					value.resize(newsize);
					
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
			if (rhs.isnull())
				return *this;
			else
				return operator<<(rhs.value.at(0));
		}
		UVarInt  operator>> (const UVarInt& rhs) const
		{
			if (rhs.isnull())
				return *this;
			else
				return operator>>(rhs.value.at(0));
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
					value.at(i) &= rhs.value.at(i);
				reduce();
			}
			return *this;
		}
		UVarInt& operator|=(const UVarInt& rhs)
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
		bool operator>=(const UVarInt& rhs) const
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
		bool operator< (const UVarInt& rhs) const
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
		bool operator<=(const UVarInt& rhs) const
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
		bool operator==(const UVarInt& rhs) const
		{
			size_t maxize = max(value.size(), rhs.value.size());
			for (size_t i = maxize; i > 0; --i)
			{
				if (gvoe(value, i - 1) != gvoe(rhs.value, i - 1))
					return false;
			}
			return true;
		}
		bool operator!=(const UVarInt& rhs) const
		{
			size_t maxize = max(value.size(), rhs.value.size());
			for (size_t i = maxize; i > 0; --i)
			{
				if (gvoe(value, i - 1) != gvoe(rhs.value, i - 1))
					return true;
			}
			return false;
		}
		int cmp(const UVarInt& rhs) const
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
		UVarInt& operator+=(const UVarInt& rhs)
		{
			if (isnull() == 1)
				value = rhs.value;
			else if (rhs.isnull() == 1)
			{
			}
			else
				for (size_t i = 0; i < rhs.value.size(); ++i)
					increment(gvoe(rhs.value, i), i);
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
					decrement(gvoe(rhs.value, i), i);
			else if (ltgtoe < 0)
			{
				UVarInt subtrahend(*this);
				value = rhs.value;
				for (size_t i = 0; i < subtrahend.value.size(); ++i)
					decrement(gvoe(subtrahend.value, i), i);
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
			{}
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
				divisor  >>= shift;
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
			auto t1 = chrono::high_resolution_clock::now();

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
			{
				cout << "Beg is gud!" << endl;
				return beg;
			}
			if (operator==(end.pow(n)))
			{
				cout << "End is gud!" << endl;
				return end;
			}

			UVarInt ans;
			while (beg <= end)
			{
				UVarInt mid = (beg + end) >> 1;
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
			cout << duration / 1000 << "ms" << endl;
			return ans;
		}
		UVarInt log2() const
		{
			return UVarInt(highestbit());
		}

		UVarInt gcd(UVarInt v)
		{
			cout << "Debug 1" << endl;
			if (operator==(v))
				return v;
			if (isnull())
				return v;
			if (v.isnull())
				return *this;
			cout << "Debug 2" << endl;
			UVarInt u(*this);
			size_t shift = min(u.lowestbit(), v.lowestbit());
			v >>= shift;
			cout << "Shift: " << shift << endl;
			cout << "Lowest: " << u.lowestbit() << endl;
			u >>= u.lowestbit();
			do
			{
				v >>= v.lowestbit();
				if (u > v)
				{
					UVarInt t(v);
					v = u;
					u = t;
				}
				v -= u;
			}
			while (!v.isnull());
			return u << shift;
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

	class SVarInt : UVarInt
	{
	public:
		bool negative = false;
		// ====={ Constructors }=====
		SVarInt() {}
		template<typename Integral, typename enable_if<is_integral<Integral>::value && !is_same<Integral, bool>::value && !is_signed<Integral>::value, Integral>::type * = nullptr>
		SVarInt(Integral num) : UVarInt(num)
		{
		}
		template<typename Integral, typename enable_if<is_integral<Integral>::value && !is_same<Integral, bool>::value && is_signed<Integral>::value, Integral>::type * = nullptr>
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
		int sign()
		{
			if (isnull())
				return 0;
			else
				return 1 - 2 * negative;
		}

		// ====={ Bitwise ops }=====

		// ====={ Comparison ops }=====
		bool operator> (const SVarInt& rhs) const
		{
			if (negative)
			{
				if (rhs.negative)
					return UVarInt::operator<(rhs);
				else
					return false;
			}
			else
			{
				if (rhs.negative)
					return true;
				else
					return UVarInt::operator>(rhs);
			}
		}
		bool operator>=(const SVarInt& rhs) const
		{
			if (negative)
			{
				if (rhs.negative)
					return UVarInt::operator<=(rhs);
				else
					return false;
			}
			else
			{
				if (rhs.negative)
					return true;
				else
					return UVarInt::operator>=(rhs);
			}
		}
		bool operator< (const SVarInt& rhs) const
		{
			if (negative)
			{
				if (rhs.negative)
					return UVarInt::operator>(rhs);
				else
					return true;
			}
			else
			{
				if (rhs.negative)
					return false;
				else
					return UVarInt::operator<(rhs);
			}
		}
		bool operator<=(const SVarInt& rhs) const
		{
			if (negative)
			{
				if (rhs.negative)
					return UVarInt::operator>=(rhs);
				else
					return true;
			}
			else
			{
				if (rhs.negative)
					return false;
				else
					return UVarInt::operator<=(rhs);
			}
		}
		bool operator==(const SVarInt& rhs) const
		{
			return negative == rhs.negative && UVarInt::operator==(rhs);
		}
		bool operator!=(const SVarInt& rhs) const
		{
			return negative != rhs.negative || UVarInt::operator!=(rhs);
		}
		int cmp(const SVarInt& rhs) const
		{
			if (negative)
			{
				if (rhs.negative)
					return -UVarInt::cmp(rhs);
				else
					return -1;
			}
			else
			{
				if (rhs.negative)
					return 1;
				else
					return UVarInt::cmp(rhs);
			}
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
		UVarInt& operator++()
		{
			int sgn = sign();
			if (sgn == -1)
				decrement(1);
			else // sgn == [0|1]
				increment(1);
			reduce();
			return *this;
		}
		UVarInt& operator--()
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
			return *this;
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

	class Fraction
	{
	public:
		bool negative = false;
		UVarInt numerator = 0;
		UVarInt denominator = 1;
		// ====={ Constructors }=====
		Fraction()
		{}
		template<typename Integral, typename enable_if<is_integral<Integral>::value && !is_same<Integral, bool>::value && !is_signed<Integral>::value, Integral>::type * = nullptr>
		Fraction(Integral num)
		{
			numerator = num;
		}
		template<typename Integral, typename enable_if<is_integral<Integral>::value && !is_same<Integral, bool>::value && is_signed<Integral>::value, Integral>::type * = nullptr>
		Fraction(Integral num) : UVarInt(num)
		{
			numerator = num;
			negative = (num < 0);
		}
		Fraction(SVarInt nom)
		{
			numerator = nom.abs();
			negative  = nom.negative;
		}
		Fraction(SVarInt nom, SVarInt den)
		{
			numerator   = nom.abs();
			denominator = nom.abs();
			negative    = (nom.negative != den.negative);
			reduce();
		}

		// ====={ Operational }=====
		Fraction& reduce()
		{
			numerator.reduce();
			denominator.reduce();
			UVarInt gcd = numerator.gcd(denominator);
			cout << "GCD:\n" << gcd.hex() << gcd.bin() << endl;
			numerator /= gcd;
			denominator /= gcd;
			if (isnull())
				negative = false;
			return *this;
		}
		Fraction& nullify()
		{
			numerator.nullify();
			denominator = 1;
			negative = false;
			return *this;
		}
		int     sign()
		{
			if (isnull())
				return 0;
			else
				return 1 - 2 * negative;
		}
		bool    isnull() const
		{
			return numerator.isnull();
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
}

using namespace Meth;

int main()
{
	string hex = "0123456789ABCDEFFEDCBA9876543210FFF2FDAFF3FFF9FC";
	string oct = "22150531704653633677766713523035452062041777777777777777777777";
	string bin = "0111010101011101010";

	UVarInt num(hex, 16);
	//UVarInt num(1024);
	UVarInt snd(20);



	cout << "A:\n" << num.hex() << num.bin() << endl << endl;
	//cout << "B:\n" << snd.hex() << snd.bin() << endl << endl;

	UVarInt wyn = num >> 58;//= num.gcd(snd);
	//Fraction wyn(num, snd);

	cout << "W:\n" << wyn.hex() << wyn.bin() << endl << endl;

}
