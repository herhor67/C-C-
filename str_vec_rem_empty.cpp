
void str_vec_rem_empty(vector<string>&);


void str_vec_rem_empty(vector<string>& vect)
{
	vect.erase(partition(vect.begin(), vect.end(), [](const string& str) { return !str_empty(str); }), vect.end());
}
