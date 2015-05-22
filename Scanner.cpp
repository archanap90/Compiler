/*Description:
Language: C++
Author: Archana P
File - Scanner.cpp
Definition of Scanner class. 
03-20-2015: Archana P,	- Added additional tokenTypes.
						- Changed getTokenType member function's signature to adapt symbolTable implemetation.
						- Called SymbolTable member functions to insert and look for tokens in SymbolTable.
*/

#pragma once
#include "Scanner.h"
#include "Parser.h"


const regex Scanner::number = regex("[0-9]+");
const regex Scanner::identifier = regex("\\w+");
const regex Scanner::op = regex("[\.,;(){}:=\*\\\+-]");
/*
	Function - getTokenTypeString
	Arguments - tokentype enum
	Return Type - string
	Description : Returns the corresponding string for the matching tokentype.
*/

string Scanner::getTokenTypeString(tokentype toktype)
{
	switch (toktype)
	{
	case 0: return "tokprogram";
	case 1: return "tokbegin";
	case 2: return "tokend";
	case 3: return "tokconst";
	case 4: return "tokdo";
	case 5: return "tokwhile";
	case 6: return "tokif";
	case 7: return "tokthen";
	case 8: return "tokelse";
	case 9: return "tokcase";
	case 10: return "tokfor";
	case 11: return "tokequals";
	case 12: return "tokperiod";
	case 13: return "toksemicolon";
	case 14: return "tokop";
	case 15: return "tokidentifier";
	case 16: return "tokword";
	case 17: return "toknumber";

	}
}
/*
Function - getTokenType
Arguments - string, semantictepe reference variable
Return Type - tokentype
Description : Returns the corresponding tokentype for the matching token and also assigns the semantic type of the token.
*/
tokentype Scanner::getTokenType(string token, semantictype &stype)
{

	stringstream str(token);
	if (regex_match(token, number)){
		stype = stconstant;
		return tokentype::toknumber;
	}
	else if (regex_match(token, identifier))
	{
		stype = stvariable;
		return tokentype::tokword;
	}
	else
	{
		stype = stoperator;
		return tokentype::tokop;
	}
}

/*
Function - getTokens
Arguments - string
Return Type - void
Description : - tokenizes the input into lexemes.
			  - Looks for token in the symbolTable and inserts if not found. 
			  - Displays the tokentype of the lexeme.
*/
void Scanner::getTokens(string input)
{
	stringstream ss(input);
	istream_iterator<string> beg(ss), end;
	int index;
	vector<string> tokens(beg, end);

	for (auto &itr : tokens)
	{
		if (symTab.installName(itr, index))
		{
			cout << "Found in the Symbol Table " << endl;
			cout << itr << " "<<getTokenTypeString(symTab.gettok_class(index)) <<"\n"<< endl;
		}
		else
		{
			semantictype stype = stunknown;
			cout << "Inserted in the symbol table" << endl;
			cout << itr << "\t" << getTokenTypeString(getTokenType(itr, stype)) <<"\n"<< endl;
			symTab.setAttrib(index, stype, getTokenType(itr, stype));
		}
	}
	parse();
}


/*
* Parse() -	This procedure checks the production table to
*		make certain that there is a production for
*		which this nonterminal can be expanded that
*		begins with this token.  If there isn't, this
*		is a fatal syntactic error; the compiler will
*		terminate execution.
*
*		Then it pushes its right sentential form on
*		the stack after linking them to their next
*		node.
*/

void Scanner::parse()
{
	int	i, lines = 0;

	initparsestack();
	parsetree = getparsenode(false, Program);
	parsepush(parsetree);
	int tableIndex = 0;

	do	{
		/*
		*  Look up the production in the production table.
		*  If not there, terminate with an error message.
		*/
		thisnode = parsepop();
		for (i = 0; i < thisnode->level; i++)
			printf("   ");
		printf("%d    ", thisnode->level);
		if (thisnode->isTerm == true)
			printToken(thisnode->ParseItem);
		else
			printnonterm(thisnode->ParseItem);
		putchar('\n');

		if (thisnode->isTerm == true)	{
			/* If its's a terminal, match it to the
			lookahead and get a new lookahead token */

	
		if (matchtoken(thistoken, thisnode->ParseItem))
				thisnode->symtabentry = tableIndex;
			else error("Parsing error - token does not required terminal");
			thistoken = (tokentype)symTab.gettok_class(tableIndex);
		}
		/* Expand the nonterminal and push the items on
		the right hand side in reverse order */
		else processnonterm(thisnode);
		if (++lines % 10 == 0)
			getchar();
	} while (!parseempty());
	
}

