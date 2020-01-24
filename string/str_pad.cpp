
string str_pad(string, size_t, char = ' ', bool = true);


string str_pad(string inp, size_t numlen, char ch, bool front)
{
	if (numlen > inp.length())
	{
		int padlen = numlen - inp.length();
		if (front)
			inp.insert(0, padlen, ch);
		else
			inp.append(padlen, ch);
	}
	return inp.substr(0, numlen);
}
