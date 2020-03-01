template <typename T>
pair<bool, size_t> findVecEl(const vector<T>& vecOfElements, const T& element)
{
	pair<bool, size_t> temp = {false, 0};
	auto it = find(vecOfElements.begin(), vecOfElements.end(), element);
	if (it != vecOfElements.end())
	{
		temp.first = true;
		temp.second = distance(vecOfElements.begin(), it);
	}
	return temp;
}
