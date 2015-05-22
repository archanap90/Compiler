/*Description:
Language: C++
Author: Archana P
File - SymbolTable.cpp
Definition of SymbolTable class.
03-20-2015: Archana P,	- Implementation of SymbolTable, attributeTable, nameTable, auxillaryTable

*/


#include "SymbolTable.h"
/*
Function - hashCode
Arguments - string
Return Type - int
Description : Calculates the hashCode for the token using ascii value and returns the value.
*/

int SymbolTable::hashCode(string str)
{
	int       i, numshifts, startchar, length;
	unsigned  code;
	length = str.length();
	numshifts = (int)min(length, (8 * sizeof(int) - 8));
	startchar = ((length - numshifts) % 2);
	code = 0;
	for (i = startchar;
		i <= startchar + numshifts - 1;  i++)
		code = (code << 1) + str[i];
	code = code %MAX;
//	while (hashtable[code] != -1 )
	//	code = (code+1)%MAX;
	return(code);
}


/*
Function - isPresent
Arguments - string, int&, int&
Return Type - bool
Description : Checks if the token string is present in the table and assigns the index in the nameTable.
*/
bool SymbolTable::isPresent(string keystring, int &code, int &nameindex)
{
	bool found = false;
	int oldnameindex, k;
	//  Initialize the old name's index to -1; it may not be there
	oldnameindex = -1;
	// Hash value for the string is calculated
	code = hashCode(keystring);
	// Starting with the entry in the hash table, trace through the name table's link list for that hash value.
	for (nameindex = hashtable[code];!found && nameindex != -1;	oldnameindex = nameindex,nameindex = nametable[nameindex].nextname)
	{
		k = nametable[nameindex].strstart;
		found = !(keystring.compare(stringtable+k));
	}
	// If it's there, we actually went right past it.
	if (found)
		nameindex = oldnameindex;
	return(found);
}


/*
Function - installAttrib
Arguments - int
Return Type - int
Description : Initializes the attributes in Attribute Table for the entry in nameTable & records the auxillary table start position and number of values.
*/

int SymbolTable::installAttrib(int nameindex)
{
	int  tabindex;
	tabindex = nametable[nameindex].symtabptr = attribtablen++;
	attribtable[tabindex].thisname = nameindex;
	attribtable[tabindex].smtype = stunknown;
	attribtable[tabindex].dataclass = dtunknown;
	attribtable[tabindex].auxTabStart = 0;
	attribtable[tabindex].auxTabCount = auxTable[attribtablen - 1].values.length();
	return(tabindex);
}

/*
Function - installName
Arguments - string, int&
Return Type - bool
Description : Returns the index to the entry of present, else inserts into name table, stringtable and attribute table.
*/

bool SymbolTable::installName(string keystring, int &tabindex)
{
	int  i, code, length, nameindex;
	// Check if the token string is in the table.  If so, return a pointer to its attribute table entry.
	length = keystring.length();
	if (isPresent(keystring, code, nameindex)) 
	{
		if (nametable[nameindex].symtabptr == -1)   
		{
			tabindex = installAttrib(nameindex);
			return(false);
		}
		else {
			tabindex = nametable[nameindex].symtabptr;
			return(true);
		}
	}
	// If not create entries in the name table, copy the name into the string table and create a hash table
	// entry(linking it to its previous entry if necessary) and create an entry in the attribute table with the bare essentials.
	nametable[nameindex = namtablen++].strstart =	strtablen;
	for (i = 0; i < length; i++)
		stringtable[strtablen++] = keystring[i];
	stringtable[strtablen++] = '\0';
	nametable[nameindex].nextname = hashtable[code];
	hashtable[code] = nameindex;
	tabindex = installAttrib(nameindex);
	
	return(false);
}

/*
Function - setAttrib
Arguments - int, semantictype, tokentype
Return Type - void
Description : Sets the attributes in the attribute table.
*/

void SymbolTable::setAttrib(int index, semantictype stype, tokentype toktype)
{
	attribtable[index].smtype = stype;
	attribtable[index].tok_class = toktype;
	if (attribtable[index].smtype == stkeyword || attribtable[index].smtype == stoperator)
		attribtable[index].dataclass = dtnone;
	else
		attribtable[index].dataclass = dtunknown;
}
/*
Function - SymbolTable (Constructor)
Description : Initializes SymbolTable class memebers and initializes the SymbolTable with entries for keywords and operators.
*/

SymbolTable::SymbolTable()
{
	int  i, nameindex=0;
	namtablen = strtablen = attribtablen =0;
	
	// Initialize the hash table, the name table's next field and the attribute table's fields as -1.
	for (i = 0; i < MAX; i++)
		hashtable[i] = -1;
	for (i = 0; i < MAX; i++)
		nametable[i].nextname = -1; for (i = 0; i < MAX; i++)   {
		attribtable[i].smtype = stunknown;
		attribtable[i].tok_class = tokentype::tokunknown;
		attribtable[i].thisname = -1;
		attribtable[i].value = "";
		attribtable[i].owningprocedure = -1;
		attribtable[i].outerscope = -1;
		attribtable[i].scopenext = -1;
		attribtable[i].label[0] = '\0';
	}
	// Install the keywords in the name table and
	//          set their attributes to keyword
	for (i = 0; i < NUM_OF_KEYWORDs; i++)    {
		installName(keystring[i], nameindex);
		setAttrib(nameindex, stkeyword, (tokentype)i);
	}
	// Install the operators in the name table and
	//          set their attributes to operator
	for (i = NUM_OF_KEYWORDs; i < NUM_OF_KEYWORDs+NUM_OF_OP; i++)  {
		installName(operators[i-NUM_OF_KEYWORDs], nameindex);
			setAttrib(nameindex, stoperator, (tokentype)i);
	}

	cout << "Symbol table is initiallized with keywords and operators." << endl;
}


