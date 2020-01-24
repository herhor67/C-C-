
int get_int(int = INT_MIN, int = INT_MAX);


int get_int(int minVal, int maxVal)
{
	int numInput;
	while (true)
	{
		string textInput;
		try
		{
			getline(cin, textInput);
			if (cin.fail())
				throw invalid_argument("Failed to read input.");
			numInput = stoi(textInput);
			if (to_string(numInput) != textInput)
				throw invalid_argument("Input is not an integer.");
			if (numInput < minVal)
				throw domain_error("Input is too small.");
			if (numInput > maxVal)
				throw domain_error("Input is too large.");
			break;
		}
		catch (const exception & err)
		{
			cout << err.what() << endl;
			cout << "Please enter an integer";
			if (minVal != INT_MIN)
			{
				cout << " >=" << minVal;
				if (maxVal != INT_MAX)
					cout << " and";
			}
			if (maxVal != INT_MAX)
				cout << " <=" << maxVal;
			cout << "." << endl;
			cin.clear();
		}
	}
	return numInput;
}
