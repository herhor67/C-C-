

vector<int> primes(int limit)
{
	vector<int> primes = { 1, 2 };
	for (int number = 3; number <= limit; number += 2)
	{
		int maxDiv = integerSqrt(number) + 1;
		bool isPrime = true;
		for (const int& prime : primes)
		{
			if (prime > maxDiv)
				break;
			if (prime != 1 && number % prime == 0)
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
