//Use only the following three libraries:
#include "parserClasses.h"
#include <iostream>
#include <fstream>

using namespace std;

//Example Test code for interacting with your Token, TokenList, and Tokenizer classes
//Add your own code to further test the operation of your Token, TokenList, and Tokenizer classes
int main() {
	ifstream sourceFile;
	TokenList tokens;

  //Lists for types of tokens
  TokenList operatorTokens;
  TokenList identifierTokens;
  TokenList literalTokens;
  TokenList commentBodyTokens;
  TokenList otherTokens;

	Tokenizer tokenizer;

	//Read in a file line-by-line and tokenize each line
	sourceFile.open("/Users/arlene/Desktop/ENSC251Final_Project/ENSC251Final_Project/test.vhd");
	if (!sourceFile.is_open()) {
		cout << "Failed to open file" << endl;
		return 1;
	}

	while(!sourceFile.eof()) {
		string line;
		getline(sourceFile, line);

		tokenizer.setString(&line);
		while(!tokenizer.isComplete()) {
			tokens.append(tokenizer.getNextToken());
		}
	}


	/*Test your tokenization of the file by traversing the tokens list and printing out the tokens*/
    //cout<<"total comment delete is "<<removeComments(tokens)<<endl;
    removeComments(tokens);
    
    /*Test your tokenization of the file by traversing the tokens list and printing out the tokens*/
    Token *t = tokens.getFirst();
    while(t) {
        cout << t->getStringRep() << " ";
        t = t->getNext();
    }
    
    /*cout<<"after delete"<<endl;
     tokens.deleteToken(t);
     Token *a = tokens.getFirst();
     while(a) {
     cout << a->getStringRep() << " ";
     a = a->getNext();
     }*/

    
    
    
  /* For your testing purposes only */

  /* Ensure that tokens have all type information set*/

  /* Create operator,identifier,literal, etc. tokenLists from the master list of tokens */


	return 0;
}
