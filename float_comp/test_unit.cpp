#include <algorithm>
#include <chrono>
#include <fstream> 
#include <iostream>
#include <limits>
#include <vector>

using namespace std;

template<class T>
typename std::enable_if<!std::numeric_limits<T>::is_integer, int>::type
cmp(T x, T y, unsigned int doia = 0)
{
	if (fabs(x - y) < std::numeric_limits<double>::min())
		return 0;
	bool ltoeq = (x - y) <= std::numeric_limits<T>::epsilon() * max(fabs(x), fabs(y)) * pow(10, doia + 1);
	bool gtoeq = (y - x) <= std::numeric_limits<T>::epsilon() * max(fabs(x), fabs(y)) * pow(10, doia + 1);
	return gtoeq - ltoeq;
}


int main()
{
	ofstream file("results.txt");

	vector<double> as, asn, cs, csn;
	as = asn = { -15, -15, -15, -15, -15 };
	cs = csn = {   5,   4,   3,   2,   1 };

	file << "Diff\t\t";
	for (unsigned int i = 0; i < as.size(); i++)
		file << cs[i] << "e" << as[i] << "\t";
	
	file << endl << "=======================================================" << endl;

	auto t1 = chrono::high_resolution_clock::now();
	for (int p = 308; p >= -307; p--)
	{
		file << p << "\t\t";
		double v = pow(10, p);
		for (unsigned int i = 0; i < as.size(); i++)
		{
			double a = v + pow(10, p + as[i]) * cs[i];
			int res = cmp(a, v);
			file << res << "\t";

		}
		file << endl;
	}
	auto t2 = chrono::high_resolution_clock::now();
	auto duration = chrono::duration_cast<chrono::microseconds>(t2 - t1).count();
	cout << duration;

	return 0;
}
