
template <typename T>
T gvoe(const vector<T> &vec, size_t pos)
{
	if (pos >= vec.size())
		return T();
	return vec[pos];
}
