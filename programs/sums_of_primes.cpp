#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

int integerSqrt(int n)
{
	if (n < 2)
		return n;
	else
	{
		int sC = integerSqrt(n >> 2) << 1;
		int lC = sC + 1;
		if (lC * lC > n)
			return sC;
		else
			return lC;
	}
}

int get_integer(int minVal = INT_MIN, int maxVal = INT_MAX)
{
	int numInput;
	while (true)
	{
		string textInput;
		try
		{
			getline(cin, textInput);
			if (cin.fail())
				throw invalid_argument("Failed to read input.");
			numInput = stoi(textInput);
			if (to_string(numInput) != textInput)
				throw invalid_argument("Input is not an integer.");
			if (numInput < minVal)
				throw domain_error("Input is too small.");
			if (numInput > maxVal)
				throw domain_error("Input is too large.");
			break;
		}
		catch (const exception & err)
		{
			cout << err.what() << endl;
			cout << "Please enter an integer";
			if (minVal != INT_MIN)
			{
				cout << " >=" << minVal;
				if (maxVal != INT_MAX)
					cout << " and";
			}
			if (maxVal != INT_MAX)
				cout << " <=" << maxVal;
			cout << "." << endl;
			cin.clear();
		}
	}
	return numInput;
}

vector<int> primes(int limit)
{
	vector<int> primes = { 2 };
	for (int number = 3; number <= limit; number += 2)
	{
		int maxDiv = integerSqrt(number) + 1;
		bool isPrime = true;
		for (const int& prime : primes)
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

int main()
{
	cout << "Give me a number:" << endl;

	int limit = get_integer(1);
	vector<int> primesv = primes(limit);
	reverse(primesv.begin(), primesv.end());

	cout << "Sorted primes:" << endl;
	for (const int& el : primesv)
		cout << el << " ";
	cout << endl << endl;

	unsigned prmlen = primesv.size()-1;
	vector<unsigned> maxposes = { 0 };

	int count = 0;

	while (maxposes[0] <= prmlen)
	{
		vector<int> solution;
		int remainder = limit;
		int level = 0;
		while (remainder > 1)
		{
			if (level >= maxposes.size())
				maxposes.push_back(maxposes[level - 1]);

			for (int i = maxposes[level]; i <= prmlen; ++i)
				if (primesv[i] <= remainder)
				{
					remainder -= primesv[i];
					solution.push_back(primesv[i]);
					maxposes[level] = i;
					break;
				}
			++level;
		}

		if (remainder == 0)
		{
			++count;
/*			cout << "Solution: " << limit << "=";
			int s = 0;
			while (s + 2 <= solution.size())
				cout << solution[s++] << "+";
			cout << solution[s] << endl;*/
		}

		int i = maxposes.size() - 1;
		while (i > 0 && maxposes[i] == prmlen)
			maxposes.erase(maxposes.begin() + i--);
		++maxposes[i];

	}
	cout << endl << "Total solutions: " << count << endl;
}
