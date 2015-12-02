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
    TokenList keywordTkoens;

	Tokenizer tokenizer;

	//Read in a file line-by-line and tokenize each line
	sourceFile.open("/Users/arlene/Desktop/251Final/251Final/test.vhd");
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
	Token *t = tokens.getFirst();
    while(t) {
        cout << t->getStringRep() << " ";
        t = t->getNext();
    }
    
    
//--------------------------------------test code for part 1-------------------------------------------
    /*Token *t = tokens.getFirst();
    while(t){
        tokens.findAndSetTokenDetails(t);
        if (t->isOperator()){
            Token *temp = new Token(*t);
            temp->setNext(nullptr);
            operatorTokens.append(temp);
        }
        if (t->isLiteral()){
            Token *temp = new Token(*t);
            temp->setNext(nullptr);
            literalTokens.append(temp);
        }
        if (t->isKeyword()){
            Token *temp = new Token(*t);
            temp->setNext(nullptr);
            keywordTkoens.append(temp);
        }
        if (t->isComment()){
            Token *temp = new Token(*t);
            temp->setNext(nullptr);
            commentBodyTokens.append(temp);
        }
        if (t->isIdentifier()){
            Token *temp = new Token(*t);
            temp->setNext(nullptr);
            identifierTokens.append(temp);
        }
        if (t->isOther()){
            Token *temp = new Token(*t);
            temp->setNext(nullptr);
            otherTokens.append(temp);
        }
        t = t->getNext();
    }
     t = identifierTokens.getFirst();
     while(t){
     cout << t->getStringRep() << " ";
     if (t->getTokenDetails() != nullptr){
     cout << t->getTokenDetails()->width << " ";
     cout << t->getTokenDetails()->type << " "<<endl;}
     t = t->getNext();}*/
 
    
//---------------------------------------test remove Token Type-----------------------------------------
  // cout << removeTokensOfType(tokens, T_CommentBody) <<endl;
    
    
    
//---------------------------------------test removeComments--------------------------------------------
    /*removeComments(tokens);
    t=tokens.getFirst();
    while(t){
        cout << t->getStringRep() << " ";
        t = t->getNext();
   }*/
    
    
//--------------------------------------------test part 2-----------------------------------------------
 /* TokenList *p = findAllConditionalExpressions(tokens);
    Token *t = p->getFirst();
    while(t!= nullptr) {
        cout << t->getStringRep() << " ";
        t = t->getNext();
    }*/
    
	return 0;
}
