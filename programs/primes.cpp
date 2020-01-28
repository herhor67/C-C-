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
	vector<int> primes;
	if (limit < 2)
		return primes;
	primes.push_back(2);
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

	int limit = get_integer();
	vector<int> primesv = primes(limit);

	cout << "Primes:" << endl;
	for (const int& el : primesv)
		printf("%i ", el);
	cout << endl;
}
