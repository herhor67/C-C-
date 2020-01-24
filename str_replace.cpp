
string str_replace(const string&, const string&, const string&);


string str_replace(const string& haystack, const string& needle, const string& straw)
{
	string result = haystack;
	size_t start_pos = 0;
	while ((start_pos = result.find(needle, start_pos)) != std::string::npos) {
		result.replace(start_pos, needle.length(), straw);
		start_pos += straw.length();
	}
	return result;
}
