/*Description:
Language: C++
Author: Archana P
File - SymbolTable.h
Declaration of SymbolTable class.
03-20-2015: Archana P,	- Implementation of SymbolTable, attributeTable, nameTable, auxillaryTable

*/

#include<iostream>
#include<vector>
#include<map>
#include <iostream>
#include <string>
#include <sstream>
#include <regex> 

#define MAX 50
#define NUM_OF_KEYWORDs 11
#define NUM_OF_OP 8
using namespace std;
enum tokentype{
	tokprogram, tokbegin, tokend, tokconst, tokdo, tokwhile, tokif, tokthen, tokelse, tokcase, tokfor, tokequals, tokperiod,
	toksemicolon, tokop, tokidentifier, tokword, toknumber, tokunknown, tokfunction, tokrelop, tokopenbracket, tokclosebracket, tokcomma,
	tokcolon, tokplus, tokminus, tokepsilon		
};



// The semantic types, i.e, keywords, procedures,variables, constants
enum semantictype {
	stunknown, stkeyword, stprogram,stparameter, stvariable, stconstant, stenum, stprocedure, stfunction,stlabel, stliteral, stoperator};
// The data types, i.e, real and integer
enum datatype  {
	dtunknown, dtnone, dtprogram,
	dtprocedure, dtinteger, dtreal
};

const string keystring[] = { "program", "begin", "end", "const", "do", "while", "if", "then", "else", "case", "for" };
const string operators[] = { "=", ".", ";", "<>", "+", "-", "*", "/" };
// The structure for name table entries, i.e, a
// starting point in a long array, a pointer to the
// entry in the attribute table and the next lexeme
// with the same hash value.
typedef struct {
	int       strstart;
	int       symtabptr;
	int       nextname;
} nametabtype;
typedef struct {
	semantictype smtype;
	tokentype tok_class;
	datatype dataclass;
	int owningprocedure;
	int thisname;
	int outerscope, scopenext;
	string value;
	char label[MAX];
	//Starting pointer to Auxillary table and number of entries.
	int auxTabStart;
	int auxTabCount;

} attribtabtype;

struct auxTab{
	string values;
};

class SymbolTable :public
{

public:
	SymbolTable();
	
	// Functions that set up the symbol table
	bool installName(string keyword, int &tabindex);
	bool isPresent(string tok, int &code, int &nameindex);
	int installAttrib(int nameindex);
	void setAttrib(int tabindex, semantictype symbol, tokentype token);

	void installdatatype(int tabindex,semantictype stype, datatype dclass);
	// Functions that handle name scoping
	int openscope(int tabindex);
	void closescope(void);
	// Set and return the attribute table index for the current procedure
	void setproc(int thisproc, int tabindex);
	int getproc(int tabindex);
		
	//  Print the lexeme and token in user-friendly formats
	void printlexeme(int i);
	void printtoken(int i);

	// Returns basic information about symbols
	tokentype gettok_class(int tabindex)
	{
		return(attribtable[tabindex].tok_class);
	}
	inline enum datatype getdatatype(int tabindex)
	{
	return(attribtable[tabindex].dataclass);
	}
	inline semantictype getsmclass(int tabindex)
	{
	return(attribtable[tabindex].smtype);
	}
	inline bool isvalidtype(int tabindex)
	{
	return(attribtable[tabindex].dataclass == dtinteger	|| attribtable[tabindex].dataclass == dtreal);
	}
	// Returns the size of the attribute table
	inline int tablesize(void){ return(attribtablen); }

	private:

	int hashCode(string str);
	inline int min(int a, int b)
	{
		return ((a < b) ? a : b);
	}
	char stringtable[200];
	nametabtype nametable[MAX];
	attribtabtype attribtable[MAX];
	auxTab auxTable[MAX];
	int hashtable[MAX];

	int strtablen, namtablen, attribtablen;
	
};
