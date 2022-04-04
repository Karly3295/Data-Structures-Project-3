#include <iostream>
#include <fstream>
#include "BinarySearchTree.h"
#include "MyMap.h"
#include "ScannerForIdents.h"
#include <vector>
#include <set>

using namespace std; 

//Karly Latkiewicz
bool isReserved(string lexeme) {
	static const int NUM_RESERVED = 73;
	static string reservedWords[NUM_RESERVED] = {
		"and", "and_eq", "asm", "auto", "bitand", "bitor",
		"bool", "break", "case", "catch", "char", "class",
		"compl", "const", "const_cast", "continue", "default",
		"delete", "do", "double", "dynamic_cast", "else", "enum",
		"explicit",  "extern", "false", "float", "for", "friend",
		"goto", "if", "inline", "int",  "long",  "mutable",
		"namespace",  "new", "not", "not_eq",  "operator",
		"or", "or_eq", "private", "protected", "public",  "register",
		"reinterpret-cast", "return", "short", "signed", "sizeof",
		"static", "static_cast", "struct", "switch", "template",
		"this", "throw", "true", "try", "typedef", "typeid", "typename",
		"union", "unsigned", "using", "virtual", "void",
		"volatile", "wchar_t", "while", "xor", "xor_eq",
	};
	//Do a binary search
	int low = 0;
	int high = NUM_RESERVED - 1; 
	int index = NUM_RESERVED / 2;
	while (low <= high) {
		if (lexeme < reservedWords[index]) {
			high = index - 1;
			index = (high + low) / 2;
		}
		else if (lexeme > reservedWords[index]) {
			low = index + 1;
			index = (high + low) / 2;
		}
		else {
			return true;
		}
	}
	return false;
			
}
ofstream &printCrossRefTable(MyMap<string, set<unsigned>> &xRefTable, ofstream &out) {
	
	for (auto itr = xRefTable.begin(); itr != xRefTable.end(); itr++) {
		auto it = *itr;
		out << it.first << "\t"; 
		for (unsigned int i : it.second) {
			out << i << "\t";
		}
		out << "\n";
	}
	return out;
}

unsigned main() {
	
	string inFileName;
	string outFileName;
	MyMap<string, set<unsigned>> xRefTable;
	unsigned currentLineNumber;
	cout << "Please enter file name to read (full name including extension): ";
	cin >> inFileName;
	fstream infile = fstream(inFileName, fstream::in);
	//check if file in there
	if (!(inFileName.find(".txt") == (inFileName.length() - 4) || inFileName.find(".cpp") == (inFileName.length() - 4))) {
		cerr << "Problem with filename: " << inFileName;
		exit(1);
	}
	if (infile.fail()) {
		cerr << "Problem opening filename: " << inFileName;
		exit(1);
	}
	cout << "Please enter file name for output of cross-reference (full name including extension): ";
	cin >> outFileName;
	ofstream outfile(outFileName, fstream::out);

	if (!(outFileName.find(".txt") == (outFileName.length() - 4))) {
		cerr << "Problem with filename: " << outFileName;
		exit(2);
	}
	//check if file in there
	if (outfile.fail()) {
		cerr << "Problem opening filename: " << outFileName;
		exit(2);
	}
	string lexeme;
	ScannerForIdents scan = ScannerForIdents(infile);
	int nextToken;
	while ((nextToken = scan.getIdent()) == 1) {
		lexeme = scan.getLastLexeme();
		if (!isReserved(lexeme)) {
			currentLineNumber = scan.getNumberOfLines();
			xRefTable[lexeme].insert(currentLineNumber);
		}
	}
	printCrossRefTable(xRefTable, outfile);
	infile.close();
	outfile.close();
	
	return 0; 
}