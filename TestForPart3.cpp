#include "parserClasses.h"
#include <iostream>
#include <fstream>
#include <vector>

using namespace std;


////////////////////////////////////////////
//////                               ///////
//////   Implementation for Part 3   ///////
//////                               ///////
////////////////////////////////////////////
//For Part 3 error checking type mismatch
//In order to print the whole line where has error
/*void printLine(Token *t) //t is the token which been detected error
{
    string a;
    Token *temp=t;
   // TokenList printErrorLine;
    //I dont know why if I use "\n" it won't work, but if I use "\\n" it works -------------------------------????????WHY?
    while (temp->getPrev()->getStringRep() != "\\n" && temp->getPrev() != nullptr) {
        temp=temp->getPrev();} //go to the first token of this error line
    for(int i=0; temp->getNext() != nullptr; i++){
        //printErrorLine.append(temp);
        //printErrorLine.append(" ");
        //a[i]=temp->getStringRep();
        cout<<temp->getStringRep()<<" ";
        temp=temp->getNext();}
    //return a;
}*/


int findMissingThen(const TokenList &tokenList, int mode)
{
    Token* temp;
    Token* temp1;
    int missing=0;
    bool FindThen = false;
    bool theEnd = false;
    
    temp = tokenList.getFirst();
    while(temp!= nullptr && theEnd == false)
    {
        FindThen = false;
        if(temp->getNext()->getNext() == nullptr)
            theEnd = true;
        if((temp->getStringRep() == "if" || temp->getStringRep()=="elsif" )&& (theEnd != true))
        {
            temp1= temp;
            temp=temp->getNext();
            cout<<temp->getStringRep()<<"see me?"<<endl;
            while(temp->getStringRep()!="if" &&temp->getStringRep()!="elsif" && temp->getStringRep()!="when")
            {
                cout<<temp->getStringRep()<<endl;
                if(temp->getStringRep()=="then")
                    FindThen= true;
                temp = temp->getNext();
            }
            if(FindThen==false && (theEnd != true))
            {
                missing++;
                if(mode==1)
                {
                    while(temp1!= temp){
                        cout <<temp1->getStringRep();
                        temp1= temp1->getNext();}
                }
            }
        }
    }
    return missing;
}


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
    
	bool verbose = false;
	vector<string> ErrorMessage;

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
    
    //Define detection mode by user through cin : Verbose Or Non-Verbose
    string modeDetection;
	cout << "What mode do you want to see the error information? 1 for Verbose, 2 for Non-Verbose";
	cin >> modeDetection;
	if (modeDetection == "1"){
        verbose = true;}

    //initialize token t for the future use
    Token *t = tokens.getFirst();
    
    //Traverse for numberOfTokens
    int numberOfTokens;
    for (numberOfTokens=1; t != nullptr && t->getNext() != nullptr; numberOfTokens++)
        t=t->getNext();
    //cout<< "numberOfTokens is "<<numberOfTokens<<endl;
    
    
    //find and set every token details in input file
    //and cout token's detail information
    //------------------------this is for test---------------------//
    t = tokens.getFirst();
    while(t)
    {
        tokens.findAndSetTokenDetails(t); ///test findAndSetTokenDetails
        //cout << t->getStringRep() << " "<<t->getTokenType()<<" ";
        //if (t->getTokenDetails() != nullptr){
          //  cout << t->getTokenDetails()->width << " ";
          //  cout << t->getTokenDetails()->type << " ";}
        t = t->getNext();
    }
    
    //Traverse for number of conditional expression
    //use function "findAllConditionalExpressions" which we defined in part 2
    TokenList *p = findAllConditionalExpressions(tokens);
    Token *CE = p->getFirst(); //CE represent for "conditional expression"
    int numberOfConditionalExp=0;
    while(CE!= nullptr) {
	    if(CE->getStringRep() == "\n") //since each conditional expression ends with "\n"
            numberOfConditionalExp++;
		CE = CE->getNext();
	}
    
  
    //Traverse for detecting type mismatches and width mismatches
    int numberOfTypeMismatch=0;
    int numberOfWidthMismatch=0;
    t = tokens.getFirst();
    Token *t_lhs = nullptr;
    Token *t_rhs = nullptr;
    tokenDetails *t_lhs_details = nullptr;
    tokenDetails *t_rhs_details = nullptr;
    while(t != nullptr)
    {
        //-----------------------------Bonus Part for checking operators-----------------------------//
        if(t->isOperator())//if token t is an operator, then check the next and previous token about t
        {
    //---------------------------------------DIDN'T PRINT OUT THE WHOLE LINE--------------------------------------//
            if(t->getPrev()!= nullptr && t->getNext() != nullptr){
                t_lhs = t->getPrev();
                t_rhs = t->getNext();}
            if (t->getPrev()!= nullptr && t->getNext() == nullptr){
                //if t is the last token
                t_lhs = t->getPrev();
                ErrorMessage.push_back("Error: Operator "+t->getStringRep() + " don't have right hand side operand");}
            if (t->getPrev() == nullptr && t->getNext() != nullptr){
                //if t is the first token
                t_rhs = t->getNext();
                ErrorMessage.push_back("Error: Operator "+t->getStringRep()+ " don't have left hand side operand");}
            if(t->getPrev()== nullptr && t->getNext() == nullptr){
                //if the T_Operator is a single token, show error
                ErrorMessage.push_back("Error: Operator "+t->getStringRep()+" stays alone and there is no other token beside");}
            if (t_lhs->getTokenDetails() != nullptr && t_rhs->getTokenDetails() != nullptr){
                t_lhs_details = t_lhs->getTokenDetails();
                t_rhs_details = t_rhs->getTokenDetails();}
            
            //----------------------------- Part for checking 2 kind of typr mismatch-----------------------------//
            //find "=" and then compare type and width
            if(t_lhs->getTokenType() != t_rhs->getTokenType())
            {   //----------------------------------//
                numberOfTypeMismatch++;
                ErrorMessage.push_back("Type mismatch: operator " +t->getStringRep()+ "'s left hand side's type is "+ t_lhs_details->type+" while right hand side's is "+t_rhs_details->type);
            }
            
            else if(t_lhs_details != nullptr && t_rhs_details != nullptr && t_lhs_details->type != t_rhs_details->type)
            {
                numberOfTypeMismatch++;
                ErrorMessage.push_back("Type mismatch: operator " +t->getStringRep()+ "'s left hand side's type is "+ t_lhs_details->type+" while right hand side's is "+t_rhs_details->type);
            }

            if (t_lhs_details != nullptr && t_rhs_details != nullptr && t_lhs_details->width != t_rhs_details->width)
            {
                numberOfWidthMismatch++;
                ErrorMessage.push_back("Width mismatch: operator " +t->getStringRep()+ "'s left hand side's width is "+ to_string(t_lhs_details->width)+" while right hand side's is "+to_string(t_rhs_details->width));
            }
        }
        t = t->getNext();
    }

    
    //cout for Non-Verbose mode and part of Verbose mode
	cout<<"The number of tokens: " << numberOfTokens <<endl;
	cout<<"The number of conditional expressions: " << numberOfConditionalExp <<endl;
	//cout<<"The number of missing end if: " << numberOfMissingEndIfs << endl;
	cout<<"The number of missing then: " << &findMissingThen << endl;
    cout<<"The number of type mismatches:"<<numberOfTypeMismatch<<endl;
    cout<<"The number of width mismatches:"<<numberOfWidthMismatch<<endl;
    
    //cout for remaining Verbose mode
    if(verbose)
    {
        if (ErrorMessage.size()==0)
            cout<<"There is no ErrorMessage for you! Well Done LOL!"<<endl;
        else
        {
            cout << "Error Messages: " << endl;
            for ( vector<string>::iterator i = ErrorMessage.begin() ; i != ErrorMessage.end(); i++)
                cout << *i << endl;
        }
        //cout<<"The number of missing if: " << numberOfMissingIfs << endl;

    }
    return 0;
}
