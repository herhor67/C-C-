
string implode(const vector<string>&, string);


string implode(const vector<string>& pieces, string glue)
{
	string result;
	int len = pieces.size();
	for (int i = 0; i < len; i++)
	{
		result += pieces[i];
		if (i < (len - 1))
			result += glue;
	}
	return result;
}
