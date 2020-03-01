
int sum_of_elems(vector<int>& vec)
{
	int result = 0;
	for (vector<int>::iterator it = vec.begin(); it != vec.end(); ++it)
		result += *it;
	return result;
}
