
bool str_empty(const string&);


bool str_empty(const string& str)
{
	return str.empty() || all_of(str.begin(), str.end(), isspace);
}
