
string get_string(size_t = 0, size_t = 4294967295);


string get_string(size_t minLen, size_t maxLen)
{
	while (true)
	{
		string textInput;
		try
		{
			getline(cin, textInput);
			if (cin.fail())
				throw invalid_argument("Failed to read input.");
			if (textInput.length() < minLen)
				throw domain_error("Input is too short.");
			if (textInput.length() > maxLen)
				throw domain_error("Input is too long.");
			return textInput;
		}
		catch (const exception & err)
		{
			cout << err.what() << endl;
			cout << "Please enter a text";
			if (minLen > 0)
			{
				cout << " longer than " << minLen;
				if (maxLen < 4294967295)
					cout << " and";
			}
			if (maxLen < 4294967295)
				cout << " shorter than " << maxLen;
			cout << " characters." << endl;
			cin.clear();
		}
	}
	return "-";
}
