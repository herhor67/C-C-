
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
