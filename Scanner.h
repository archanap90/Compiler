/*Description:
Language: C++
Author: Archana P
File - Scanner.h
Declaration of Scanner class.
03-20-2015: Archana P, - Added SymbolTable Object to Scannner class
					   - Changed getTokenType member function's signature to adapt symbolTable implemetation.
*/

#include <iostream>
#include <string>
#include <sstream>
#include <regex> 
#pragma once

#include "SymbolTable.h"
using namespace std;


//Scanner class : Tokenizes input string and outputs the tokenType of lexeme.
class Scanner
{
	//Regular expression to match the token category
public:
	static const regex number, identifier, op;
	//Tokenizes the input string
	void getTokens(string line);
private:
	string getTokenTypeString(tokentype toktype);
	//Returns the tokentType
	tokentype getTokenType(string token, semantictype &stype);
	//Reference to SymbolTable
	SymbolTable symTab;
	void parse();
	
};

