/*Description:
Language: C++
Author: Archana P
Classes : Scanner, SymbolTable
Descriptiion : Tokenizes the input and looks in the Symbol Table , if not found adds it to SymbolTable. Outputs the token type of the lexeme.
main() - entry point to the program. Takes user input from stdin and calls Scanner for further processing.
*/

#include "Scanner.h"

int main()
{
	string input = "";
	Scanner scannerObj;
	smatch mpos;
	//Input from the user until 'exit' is entered.
	cout << "Enter the input or 'exit' to Exit" << endl;
	do {
		string line("");
		cin >> line;
		transform(line.begin(), line.end(), line.begin(), ::tolower);
		if (line != "exit")
		{
			int flag = 0;
			while (regex_search(line, mpos, scannerObj.op))
			{
				int i,start=0;
				flag = 1;
				input = input + " " + line.substr(start, mpos.position(0));
				input = input + " " + line.substr(mpos.position(0), mpos[0].length());
					start = mpos.position(0) + mpos[0].length();
				
				line = line.substr(start);
			}
			if (flag==0)
				input = input + " " + line;
		}
		else
			break;
	} while (true);
	scannerObj.getTokens(input);

	getchar();
	system("pause");
	return 0;
}