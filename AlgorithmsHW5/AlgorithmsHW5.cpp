// AlgorithmsHW5.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <fstream>
#include <map>

using namespace std;

string performSubstitution(const string source, const string target, const size_t i, vector<string>& steps)
{
	ostringstream ss;

	string result(source);

	if (result[i] != target[i]) {
		result[i] = target[i];

		ss << "substitute(" << i << ") = " << result;
		steps.push_back(ss.str());
	}

	return result;
}

string performFlip(const string source, const string target, const size_t begin, const size_t end, vector<string>& steps)
{
	ostringstream ss;

	string result(source);
	for (size_t i = begin, j = end; i <= j; i++, j--)
	{
		const char temp = result[i];
		result[i] = result[j];
		result[j] = temp;
	}

	ss << "flip(" << begin << ", " << end << ") = " << result;
	steps.push_back(ss.str());

	for (size_t i = begin; i <= end; i++)
	{
		if (result[i] != target[i])
		{
			result[i] = target[i];

			ss.str("");
			ss << "substitute(" << i << ") = " << result;
			steps.push_back(ss.str());
		}
	}

	return result;
}

vector<string> transform(const string source, const string target)
{
	if (source.length() != target.length())
	{
		throw "ERROR: Source and target strings are not the same length!";
	}

	const size_t srcLen = source.length();

	vector<string>* memoized = new vector<string>[srcLen+1];
	memoized[srcLen] = vector<string>();

	ostringstream ss;

	for (int i = srcLen - 1; i >= 0; i--)
	{
		ss.str("");

		if (i > 0)
		{
			ss << target.substr(0, i) << source.substr(i);
		}
		else
		{
			ss << source;
		}
		
		string step = ss.str();

		// Check if this is an issue
		if (step[i] == target[i])
		{
			memoized[i] = memoized[i + 1];
			continue;
		}

		vector<vector<string>> options;

		// First, try a simple substitution
		vector<string> steps = vector<string>();
		string afterSubs = performSubstitution(step, target, i, steps);
		vector<string> subsHistory = vector<string>(memoized[i + 1]);
		subsHistory.insert(subsHistory.begin(), steps.begin(), steps.end());
		options.push_back(subsHistory);
		
		// Next, try flips
		for (size_t j = srcLen - 1; j > i; j--)
		{
			steps = vector<string>();
			string afterFlip = performFlip(step, target, i, j, steps);
			vector<string> flipHistory = vector<string>(memoized[j+1]);
			flipHistory.insert(flipHistory.begin(), steps.begin(), steps.end());
			options.push_back(flipHistory);
		}

		// Find the smallest version we can
		vector<string> smallestHistory;
		int smallestHistoryLength = -1;
		for (auto elem : options)
		{
			size_t hLen = elem.size();
			if (smallestHistoryLength < 0 || hLen < smallestHistoryLength)
			{
				smallestHistoryLength = hLen;
				smallestHistory = elem;
			}
		}

		memoized[i] = smallestHistory;
		cout << "Step[" << i << "]:\t" << step << " = " << smallestHistoryLength << endl;
	}

	return memoized[0];
}

int main(int argc, char *argv[])
{
	if (argc != 2 && argc != 3)
	{
		cout << "To use this program, run it with either one or two argument:" << endl;
		cout << "\t1) (required) the name of the input file" << endl;
		cout << "\t2) (optional) the name of the desired output file" << endl;
		cout << "If the second argument is not present, output will be rendered to the terminal." << endl;
		return EXIT_FAILURE;
	}

	ifstream infile(argv[1]);

	// string input = "mzthamjqsxtlzwaviksubsjdihbpuxvoremgchqrqstsvrjfnapsqqmanecfrfvlsaulrobiwvywvqttezwieholrzngtkgvccfvgomotzksnugrytuqlmydrbfynpbzylyrdaabcgrgwcrzndxwcmgbnddderzxxengcoyoctdncjfzhxxyxkqbymxsxsmrfupvixxzbrjimjpfwqozrmgyicxzfqviqkfycjpthlkgdldkhepbdtfqrpdfmknypiezheiliistiedfrctsywpvbjmtvecuwukohkiieaiaycxxhznwdvfxoxkpdjaaicmgliowepseitglrvdhpcndlsmgcmmifdjdaantmclgnxluobkvngpovaxlnwakotxjjgeywxjoefzmwtflbftoaorqfhtkvgkkubbvettxpoxoqratyrprtjvswfsepnpqwjamkbndvdcsgravngdmrnzhtjbmtencewmioidlzqwcmnxw";
	// string output = "lxwslbalmfkplvstxdaqhqhnqyzeyqdsdyszjavsitwxgzrcwaflgzvkxdwgnprvpqtysaruopcspbdpbrjgtngfgoefxgnldlujbhxythqwwgpgfcqzxdryponilvlpssfujlhpbefufcghxhwcbmpzwmaptuhqnebbcjfmxxhirdyibkrknbpvztogaorofyrtwvfmsdfvatkvdwqvscktjjsevlsgeevrbqupwyipnhnczzptbolbqqhuxmfhlluqlwmzmsxlmftpcpgmysiajpwuqyylwvuhhdegdhweqckztuyvqjioidabcorlfmtemvoqxxpdbdckzseizlgvaysvxtfvqmihfiqcaztuamcxqczwrsuaxkfguzvjiaiucqxjjqjjdmrsmajalankfxfradagafsnsrphuusmixyleexnfkjieifoaxeqwwykgvjofvbjguxjwvnfyutwjbqhhckylotuhikqhvruqteuumqw";

	// string input = "timeflieslikeanarrow";
	// string output = "tfemiliilzejeworrbna";

	string input;
	string output;

	std::getline(infile, input);
	std::getline(infile, output);

	input = input.substr(3);
	output = output.substr(3);

	vector<string> steps = transform(input, output);

	if (argc == 3)
	{
		ofstream outfile(argv[2]);

		outfile << "Transformation took " << steps.size() << " steps:" << endl;
		outfile << "\tSOURCE: " << input << endl;
		for (size_t i = 0; i < steps.size(); i++)
		{
			outfile << "\tStep " << (i + 1) << ": " << steps[i] << endl;
		}
		outfile << "\tTARGET: " << output << endl;
	}
	else
	{
		cout << "Transformation took " << steps.size() << " steps:" << endl;
		cout << "\tINPUT: " << input << endl;
		for (size_t i = 0; i < steps.size(); i++)
		{
			cout << "\tStep " << (i + 1) << ": " << steps[i] << endl;
		}
		cout << "\tOUTPUT: " << output << endl;
		getchar();
	}

    return EXIT_SUCCESS;
}

