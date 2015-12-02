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
/*string printLine(Token *t) //t is the token which been detected error
{
    string a;
    Token *temp=t;
   // TokenList printErrorLine;
    while (temp->getPrev() != nullptr && temp->getPrev()->getStringRep() != "\n" ) {
        temp=temp->getPrev();} //go to the first token of this error line
    for(int i=0; temp->getNext()!= nullptr && temp->getNext()->getStringRep() != "\n"; i++){
        a.append(temp->getStringRep());
        a.append(" ");
        temp=temp->getNext();}
    return a;
}*/
string printLine(Token *t) //t is the token which been detected error
{
    string a;
    // TokenList printErrorLine;
    if(t->getNext()!= nullptr && t->getPrev() != nullptr){
        a.append(t->getPrev()->getStringRep());
        a.append(" ");
        a.append(t->getStringRep());
        a.append(" ");
        a.append(t->getNext()->getStringRep());
    }
    return a;
}

int findMissingThen(const TokenList &tokenList, int mode)
{
    Token* temp;
    Token* temp1;
    int missing=0;
    bool FindThen = false; //true if "then" is found
    bool space = false;// true if file end without "endif"
    bool endif = false;// true if file end with "endif"
    bool entered = false; // true when entering first if statment
    bool notendif = false;// true when end if exsit in the middle
    
    temp = tokenList.getFirst();
    while(temp!= nullptr&& endif == false){
        FindThen = false;
        space = false;
        endif = false;
        entered = false;
        notendif = false;
        if((temp->getStringRep() == "if" || temp->getStringRep()=="elsif" )){
            entered = true;
            temp1= temp;//setting location 1
            temp=temp->getNext();
            while(temp!= nullptr && temp->getStringRep()!="if" &&temp->getStringRep()!="elsif" && space == false){
                if(temp->getStringRep()=="then"){// checking to see if "then" exist
                    FindThen= true;}
                temp = temp->getNext();
                if(temp!= nullptr){// checking for end of file without "endif"
                    if(temp->getStringRep()=="\n" && temp->getNext()== nullptr){
                        space = true;}}}
            if(temp!= nullptr && temp->getPrev()!= nullptr){// checking to see if input is ending with "endif" or not
                if(temp->getStringRep()== "if" && temp->getPrev()->getStringRep()=="end"&&temp->getNext()->getNext()== nullptr)
                    {endif = true;}}
            if(temp1->getPrev()!= nullptr&& endif==false){// checking to see if theres any end if in the middle
                if(temp1->getStringRep()== "if" && temp1->getPrev()->getStringRep()=="end"){
                    notendif = true;}}
            if(FindThen==false&& notendif == false)
            {
                missing++;
                if(mode==1)//cout lines where error occured
                {
                    if(endif == true || notendif == false){//get rid of "end" when displaying error case(when dealing with "end if")
                        while(temp != nullptr && temp1!= temp->getPrev()){
                            cout <<temp1->getStringRep();
                            temp1= temp1->getNext();}
                        cout << "-- missing then"<<endl<<endl;}
                    else{//for input ending WITHOUT "end if"
                        while(temp1!= temp){
                            cout <<temp1->getStringRep();
                            temp1= temp1->getNext();}
                        cout << "-- missing then"<<endl<<endl;}
                }
            }
            if(space == true)// only move to next token if file is ending with "/n"
                temp = temp->getNext();
        }
        if(entered == false){
            temp = temp->getNext();}
    }
    return missing;
}


// count the number of conditional expression
int countNumCondExp(const TokenList &tokenList)
{
    int count1 = 0;  int count2=0;
    Token* t = tokenList.getFirst();
    while (t!= nullptr && t->getNext() != nullptr){
        if ( t->getStringRep()=="if" && t->getPrev()!=nullptr && t->getPrev()->getStringRep()=="end")count1++;
        if (t->getStringRep()=="if" && t->getPrev()==nullptr)count1++;
        if (t->getStringRep()=="if" && t->getPrev()!=nullptr && t->getPrev()->getStringRep()!="end")count1++;
        if (t->getStringRep()=="elsif" || t->getStringRep()=="when" )count1++;
        if (t->getStringRep()=="else" || t->getStringRep()=="then")count2++;
        t=t->getNext();}
    if(count1>count2)
        return count1;
    else
        return count2;
}

//------------------------------use vector to track balance of if and endif------------------------------//
int findMissingEndIf(const TokenList &tokenList)
{
    vector<string> EndIf;
    int numberOfEndIf=0, numberofIf=0;
    Token *t=tokenList.getFirst(); //create 3 token and initialize them
    Token *temp=tokenList.getFirst();
    Token *temp1=tokenList.getFirst();
    if (t!= nullptr && t->getNext() != nullptr){
        temp=t->getNext();
        temp1=t->getNext();}
    if (t != nullptr && t->getStringRep()=="if")
    {
        EndIf.push_back(t->getStringRep()); //once find push/store "if"
        while(temp1 != nullptr)
        {
            if (temp1->getPrev()!=nullptr && temp1->getNext()!=nullptr &&  temp1->getPrev()->getStringRep()=="end" && temp1->getStringRep()=="if"){
                numberOfEndIf=1;
                EndIf.pop_back();}
            temp1=temp1->getNext();
        }
        numberofIf=1;
    }
    while (t != nullptr){
        if (t->getPrev() != nullptr && t->getStringRep()=="if" && t->getPrev()->getStringRep()!="end"){
            while(temp != nullptr){
                if (temp->getPrev() != nullptr && temp->getPrev()->getStringRep()=="end"&&temp->getStringRep()=="if")
                    numberOfEndIf++;
                temp=temp->getNext();}
            numberofIf++;}
        t=t->getNext();
    }
    int numberOfMissing= (numberofIf-numberOfEndIf);
    if (numberOfMissing <0)
        numberOfMissing=-numberOfMissing;
    return numberOfMissing;
}

////////////////////////////////////////////
//////                               ///////
//////      Main Fuction Begin       ///////
//////                               ///////
////////////////////////////////////////////
int main() {
	ifstream sourceFile;
	TokenList tokens;
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
    int numberOfTokens=0;
    for (; t != nullptr && t->getNext() != nullptr;numberOfTokens++)
        t=t->getNext();
    if (t != nullptr && t->getNext() == nullptr)
        numberOfTokens++;
   
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
    
    /*
    //Traverse for number of conditional expression
    //use function "findAllConditionalExpressions" which we defined in part 2
    TokenList *p = findAllConditionalExpressions(tokens);
    Token *CE = p->getFirst(); //CE represent for "conditional expression"
    int numberOfConditionalExp=0;
    while(CE!= nullptr) {
	    if(CE->getStringRep() == "\n") //since each conditional expression ends with "\n"
            numberOfConditionalExp++;
		CE = CE->getNext();
	}*/
    
  
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
            
        //------------------------------find "=" and then compare type and width------------------------------//
            if(t_lhs_details != nullptr && t_rhs_details != nullptr && t_lhs_details->type != t_rhs_details->type)
            {
                numberOfTypeMismatch++;
                ErrorMessage.push_back(printLine(t) + " --  type mismatch");
                ErrorMessage.push_back("Type mismatch details: operator " +t->getStringRep()+ "'s left hand side's type is "+ t_lhs_details->type+" while right hand side's is "+t_rhs_details->type);
            }

            if (t_lhs_details != nullptr && t_rhs_details != nullptr && t_lhs_details->width != t_rhs_details->width)
            {
                numberOfWidthMismatch++;
                ErrorMessage.push_back(printLine(t) + " --  width mismatch");
                ErrorMessage.push_back("Width mismatch details: operator " +t->getStringRep()+ "'s left hand side's width is "+ to_string(t_lhs_details->width)+" while right hand side's is "+to_string(t_rhs_details->width));
            }
        }
        t = t->getNext();
    }

    
    
    
    //cout for Non-Verbose mode and part of Verbose mode
	cout<<"The number of tokens: " << numberOfTokens <<endl;
	cout<<"The number of conditional expressions: " << countNumCondExp(tokens) <<endl;
	cout<<"The number of missing end if: " << findMissingEndIf(tokens) << endl;
    cout<<"The number of type mismatches:"<<numberOfTypeMismatch<<endl;
    cout<<"The number of width mismatches:"<<numberOfWidthMismatch<<endl;
    
    //cout for remaining Verbose mode
    if(verbose)
    {
        //-----------This part is only for checking the balance between "if" and "end if"-------------//
        if (ErrorMessage.size()==0)
            cout<<"There is no ErrorMessage for you! Well Done LOL!"<<endl;
        else
        {
            cout << "Error Messages: " << endl;
            findMissingThen(tokens,verbose);
            for ( vector<string>::iterator i = ErrorMessage.begin() ; i != ErrorMessage.end(); i++)
                cout << *i << endl;
        }
    }
    return 0;
}
