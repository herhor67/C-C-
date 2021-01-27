#include <iostream>
#include <iomanip>
#include <cmath>

using namespace std;

int main()
{
	double frac = 0.30103;
	int maxden;
	cout << "Give max denominator:" << endl;
//	cin >> maxden;
	maxden = 127;
	cout << fixed << setprecision(4);
	for (int den = 1; den <= maxden; ++den)
	{
		double aprnom = frac * den;
		int lwrnom = floor(aprnom);
		int uprnom = ceil(aprnom);
		double lwrfrac = (double)lwrnom / den;
		double uprfrac = (double)uprnom / den;
		cout << lwrnom << " / " << den << "   \t" << (frac-lwrfrac)/frac*100 << "%\t\t";
		cout << uprnom << " / " << den << "   \t" << (uprfrac-frac)/frac*100 << "%\n";
	}

	return 0;
}
