//Use only the following libraries:
#include "parserClasses.h"
#include <string>
#include<iostream>
//****TokenList class function definitions******
//           function implementations for append have been provided and do not need to be modified

//Creates a new token for the string input, str
//Appends this new token to the TokenList
//On return from the function, it will be the last token in the list
void TokenList::append(const string &str) {
	Token *token = new Token(str);
	append(token);
}


//Copy constructor
Token::Token(const Token &token){
    next=token.next;
    prev=token.prev;
    stringRep=token.stringRep;
    _isKeyword=token._isKeyword;
    type=token.type;
    if(token.details ==nullptr) {//allocate and copy contents
       details =nullptr;//new tokenDetails;
       //details->type="";
    }
    else {
           details =new tokenDetails;

        details->type=token.details->type;
        details->width=token.details->width;
        }
}

//Destructor, free any memory owned by this object
Token::~Token(){
    if (details!=nullptr)
        delete details;
}


//Assignment operator
void Token::operator =(const Token& token){
    next=token.next;
    prev=token.prev;
    stringRep=token.stringRep;
    _isKeyword=token._isKeyword;
    type=token.type;
    if(token.details ==nullptr) {//allocate and copy contents
       if(details) {
            delete details;
            details =nullptr;//new tokenDetails;
        }
       //details->type="";
    }
    else {
        if(!details) {
           details =new tokenDetails;
           }

        details->type=token.details->type;
        details->width=token.details->width;
        }
}



//Appends the token to the TokenList if not null
//On return from the function, it will be the last token in the list
void TokenList::append(Token *token) {
	if (!head) {
		head = token;
		tail = token;
	}
	else {
		tail->setNext(token);
		token->setPrev(tail);
		tail = token;
	}
}

//This function returns TRUE if the input is integer literal else FALSE
bool TokenList::isIntegerLiteral(const string & input)
{
    int length;
    length = (int)input.length();
    //get the length of input string, and cast it to int
    for (int i=0; i<length; i++)
        //comfirm each char of my string is number
    {
        if ( input[i] <48 || input[i] >57) //use ascii table to decide if it is an integer
            return false;
    }
    return true;
}


//find token details and type and update token.  May require examining properties of neighbouring tokens
void TokenList::findAndSetTokenDetails(Token *token){
    //first to find Keywords and set boolean type as true
    const int length_Keyword= 100; //97 //get the length of arrayOfKeywords
    for (int i=0; i<length_Keyword; i++)
    {
        if (token->stringRep == arrayOfKeywords[i]){
            //compare tableOfKeywords
            token->_isKeyword=true;}
        else{
        token->_isKeyword=false;}
    }

    //tracersing for a comment body
    if(token->prev->stringRep=="--")
    {
        token->setTokenType(T_CommentBody);
        return;
    }


    //traversing for operator
    //Didnt consider token like "=>, <=, ==" here, this should be done in prepareNextToken
    for (int i=0; i < 28; i++) {
        if (token->stringRep.find(arrayOfOperators1[i])!=std::string::npos){
            token->setTokenType(T_Operator);
            return;
        }
    }



    //traverse for literal
    //cases where it has bit vectors "0010101", x"1234ABCD"
    if ((token->stringRep).find_first_of("\"")!=std::string::npos){ //can find "
        if ((token->stringRep).find_first_of("\"")==0){// if " starts the first position of token
            token->details->width=static_cast<int>((token->stringRep).find_last_of("\"")-(token->stringRep).find_first_of("\""))-1;
            token->details->type="std_logic_vector";
            token->setTokenType(T_Literal);
        }
        if ((token->stringRep).find_first_of("\"")==1){// if " is the second position of token
            if ((token->stringRep).at(0)=='X'||(token->stringRep).at(0)=='x')
            {
                token->details->width=(static_cast<int>((token->stringRep).find_last_of("\"")-(token->stringRep).find_first_of("\""))-1)*4;
                token->details->type="std_logic_vector";
                token->setTokenType(T_Literal);}
            else if ((token->stringRep).at(0)=='B'||(token->stringRep).at(0)=='b')
            {
                token->details->width=static_cast<int>((token->stringRep).find_last_of("\"")-(token->stringRep).find_first_of("\""))-1;
                token->details->type="std_logic_vector";
                token->setTokenType(T_Literal);}
            else if ((token->stringRep).at(0)=='O'||(token->stringRep).at(0)=='o')
            {
                token->details->width=(static_cast<int>((token->stringRep).find_last_of("\"")-(token->stringRep).find_first_of("\""))-1)*3;
                token->details->type="std_logic_vector";
                token->setTokenType(T_Literal);}
        }
        return;
    }

    //traverse for std_logic e.g. '0','1'
    if ((token->stringRep).find_first_of("\'")==0 && (token->stringRep).find_first_of("\'")==2){
        if ((token->stringRep).at(1)=='0'||(token->stringRep).at(1)=='1'){
            token->details->type="std_logic";
            token->details->width=1;
            token->setTokenType(T_Literal);}
        return;
    }

    //traverse for boolean literal
    if (token->stringRep=="true" || token->stringRep=="false"){
        token->setTokenType(T_Literal);
        token->details->width=1;
        return;
    }

    //traverse for integer literal
    if (isIntegerLiteral(token->stringRep)){
        token->setTokenType(T_Identifier);
        token->details->width=0;
        token->details->type="integer";
        return;
    }


    //traverse for identifier
    //--------------------need to be classified as variable or signal!!! which is not been done yet!--------------------
    //--------------------also didn't do tolower!----------------
    //all start from letters except  operator, comment, and literals should be identifier
    if((arrayOfLetter[0]).find_first_of((token->stringRep).at(0))==0)
    {
        token->setTokenType(T_Identifier);
        if (token->getNext() != nullptr && token->getNext()->getNext() != nullptr && token->getNext()->getStringRep() == ":")
        {
            //------------------not sure this way to check variable or signals are right or wrong-----------------------
            token->details->type = token->getNext()->getNext()->getStringRep();//Set detail type
            if (token->getNext()->getNext()->getNext() != nullptr && token->getNext()->getNext()->getNext()->getStringRep() == "(" &&token->getNext()->getNext()->getNext()->getNext() != nullptr)//Find width
            {
                int length = std::atoi(token->getNext()->getNext()->getNext()->getNext()->getStringRep().c_str()) - std::atoi(token->getNext()->getNext()->getNext()->getNext()->getNext()->getNext()->getStringRep().c_str());
                if (length < 0)
                    length = - length;
                token->details->width = length + 1;
            }
        }
        return;
    }


    //traversing for other_type
    else
    {
        token->setTokenType(T_Other);
    }
    return;
}


//Computes a new tokenLength for the next token
//Modifies: size_t tokenLength, and bool complete
//(Optionally): may modify offset
//Does NOT modify any other member variable of Tokenizer
void Tokenizer::prepareNextToken()
{   size_t len = str->length();
    bool found =false;
    size_t index=offset;
    //consider the case of the comment
    if(comment == true){
        tokenLength = len-temp_comment;
        comment = false;
    }
    else if (offset == len) //when you traverse to the end of the list
		{complete = true;}
    else if(((*str).find_first_not_of(" ", offset))==std::string::npos){
        complete = true;
    }
    else if (!complete)
    {
           //meet this condition when what we read is not comment
            while (!found && index<len)
            {
                if(str->at(index)==' ' || str->at(index)=='\t'){
                    tokenLength = index-offset;
                    found = true;
                    if(index ==len){
                        complete = true;
                        return;
                    }
                    if (tokenLength == 0) {
                        if (index == len){
                            complete = true;
                            found = true;
                            return;
                            //end of string condition, you hit the end of line and are thus complete
                        }
                        else {
                            offset++;
                            index++;
                            found = false;
                            //you should keep moving forward till you hit the a character of importance or you end
                        }
                    }
                }
                //one case that indicates delimiter '-' and comment
                else if(str->at(index)=='-'){
                    tokenLength = index - offset;
                    if(tokenLength==0){
                        tokenLength =1;
                    }
                    //if the next index is still '-' then, all the string behind -- is comment
                    if(index < len-1 && str->at(index+1)=='-'&& (index - offset)==0){
                        tokenLength = 2;
                        found = true;
                        comment = true;
                        temp_comment = (int)index+2;
                    }
                    found = true;
                    return;
                }
                //all the delimiter with only itself are indicated here
                //delimieters include #();|&+.,
                //tokenLength are all equal 1
                else if(str->at(index)=='#'||str->at(index)=='(' ||str->at(index)==')'||str->at(index)==';'||str->at(index)=='|'||str->at(index)=='&'
                    ||str->at(index)=='+'||str->at(index)=='.' ||str->at(index)==','){
                    tokenLength = index- offset;
                    if(tokenLength ==0){
                        tokenLength = 1;
                    }
                    found = true;
                    return;
                }
                //indicates delimeter * and **
                //* has tokenLengt =1 and ** has tokenlength =2
                else if(str->at(index)=='*'){
                    tokenLength = index - offset;
                    if(tokenLength ==0){
                        tokenLength =1;
                    }
                    if(index < len-1 && str->at(index+1)=='*'&& (index - offset) ==0 ){
                        tokenLength =2;
                    }

                    found = true;
                    return;
                }
                //indicates delimeter / and /=
                // / has tokenLengt =1 and /= has tokenlength =2
                else if(str->at(index)=='/'){
                    tokenLength = index -offset;
                    if(tokenLength ==0){
                        tokenLength =1;
                    }
                    if((index< len-1) && str->at(index+1)=='=' &&(index - offset) ==0){
                        tokenLength =2;
                    }

                    found = true;
                    return;
                }
                //indicates delimeter : and :=
                //: has tokenLengt =1 and := has tokenlength =2
                else if(str->at(index)==':'){
                    tokenLength = index - offset;
                    if(tokenLength ==0){
                        tokenLength = 1;
                    }if((index < len-1)&& str->at(index+1)=='=' &&(index - offset) ==0){
                        tokenLength = 2;
                    }
                    found = true;
                    return;
                }
                //indicates delimeter <, <= and <>
                //< has tokenlength =1 where <= and <> have tokenlength =2
                else if(str->at(index)=='<'){
                    tokenLength = index -offset;
                    if(tokenLength ==0){
                        tokenLength =1;
                    }
                    if((index< len-1)&& str->at(index+1)=='=' && (index - offset) ==0){
                        tokenLength =2;
                    }
                    if((index< len-1)&& str->at(index+1)=='>' && (index - offset)==0){
                        tokenLength =2;
                    }
                    found = true;
                    return;
                }
                //indicates delimeter > and >=
                // >has tokenlength =1 where >= has tokenlength =2
                else if(str->at(index)=='>'){
                    tokenLength = index -offset;
                    if(tokenLength ==0){
                        tokenLength =1;
                    }
                    if((index< len-1)&& str->at(index+1)=='=' && (index - offset) ==0){
                        tokenLength =2;
                    }
                    found = true;
                    return;
                }
                //indicates delimeter =, ==
                //= has tokenlength =1 where == has tokenlength =2
                else if(str->at(index)=='='){
                    tokenLength = index -offset;
                    if(tokenLength ==0){
                        tokenLength =1;
                    }
                    if((index< len-1)&& str->at(index+1)=='>' && (index - offset) ==0){
                        tokenLength =2;
                    }
                    found = true;
                    return;
                }
                //corner cases where it has '0', '1' or 'itself
                //indicates ' itself with tokenLength=1, '0' or '1' etc with tokenLength =3
                else if (str->at(index)=='\'' ){
                    tokenLength = index -offset;
                    if(tokenLength ==0){
                        tokenLength =1;
                    }
                    if(index < len-2 && str->at(index+2)=='\'' && (index -offset)==0){
                        tokenLength =3;
                    }
                    found = true;
                    return;
                }

                //Corner cases where it has bit vectors "0010101", x"1234ABCD",
                else if(str->at(index)=='"'){
                    tokenLength = index-offset;
                    //includes all the cases where it has x"0010101", X"0010101", O"0010101", o"0010101", B"0010101", b"0010101"
                    if (index!=0)
                    {
                        if(str->at(index-1)=='X'||str->at(index-1)=='x'||str->at(index-1)=='B'||str->at(index-1)=='b'||str->at(index-1)=='O'||str->at(index-1)=='o'){
                            size_t temp_index = (*str).find_first_of("\"",index+1);
                            tokenLength = temp_index - (index-2);
                            index = temp_index+1;
                        }
                        else {
                            size_t temp_index = (*str).find_first_of("\"",index+1);
                            tokenLength = temp_index - (index-1);
                            index = temp_index+1;
                        }
                    }
                    else{   size_t temp_index = (*str).find_first_of("\"",index+1);
                            tokenLength = temp_index - (index-1);
                            index = temp_index+1;}
                    found = true;
                    return;
                }
                else
                    index ++;
            }
        }
        if (index == len)
			tokenLength = len - offset;
    }


//Sets the current string to be tokenized
//Resets all Tokenizer state variables
//Calls Tokenizer::prepareNextToken() as the last statement before returning.
void Tokenizer::setString(string *str) {
    if (str->length() == 0) {
		//this is executed if a blank string is passed in
		complete = true;
	}
	//if the string has characters in it then you initialize some variables and prepare the next token for the get next token funtion
	else {
        complete = false;
        comment = false;
        offset = 0;
        tokenLength = 0;
        this->str = str;
        prepareNextToken();
	}
}

//Returns the next token. Hint: consider the substr function
//Updates the tokenizer state
//Updates offset, resets tokenLength, updates processingABC member variables
//Calls Tokenizer::prepareNextToken() as the last statement before returning.
string Tokenizer::getNextToken() {
    string temp = str->substr(offset, tokenLength);
	//substr takes a string and outputs a string of a specified length starting at a point
	offset = offset + tokenLength;
	//move to the new position to seek forward
	tokenLength = 0;
	prepareNextToken();
    return temp;
}


//****Challenge Task Functions******

//Removes the token from the linked list if it is not null
//Deletes the token
//On return from function, head, tail and the prev and next Tokens (in relation to the provided token) may be modified.
void TokenList::deleteToken(Token *token)
{ if (token)
   {
		if (head == token && tail == token) { //if tokenlist only have one element and is equal to our target
			head = NULL;
			tail = NULL;
		}
		else if (token ==head) {//if the head of the list is our target
			head = head->getNext();
			head->setPrev(NULL);
		}
		else if (token==tail) {//if the tail of the list is our target
			tail = token->getPrev();
			tail->setNext(NULL);
		}
		else { //if the target is in the middle of the list
			token->next->prev = token->prev; //make prev of "token->next" points to "token->previous"
            token->prev->next = token->next; //make next of "token->prev" points to "token->next"
		}
		delete token;
	}
  else {return;} //when list is null
}

//Removes all comments from the tokenList including the -- marker
//Returns the number of comments removed
int removeComments(TokenList &tokenList) {
    Token *temp=tokenList.getFirst(); //create a temp token and initialize it to first element in tokenList
    int count=0;
    while (temp!=NULL && temp->getNext() != nullptr){ //if token is not the last element in tokenLisk
        //cout<<"read"<<temp->getStringRep()<<"as the current token"<<endl;
            if(temp->getStringRep()=="--")//once detect --
            {
                count++; //update comment number
                //delete token
                temp=temp->getNext()->getNext();//point to next token
                tokenList.deleteToken(temp->getPrev()->getPrev());
                tokenList.deleteToken(temp->getPrev());
            }
            else{//traverse untill it find comment signal
                temp=temp->getNext();}
        }
    delete temp; //free the space which we occupied
    return count;//return total comment number in 1 file
}


//Removes all tokens of the given tokenType
//Returns the number of tokens removed
int removeTokensOfType(TokenList &tokenList,tokenType type) {
    Token *temp=tokenList.getFirst();
    while(temp!=NULL)
    {
        if(temp->getTokenType()==type) //if find target type, delete and go to next token
        {
            tokenList.deleteToken(temp);
            temp=temp->getNext();
        }
        else
        {
            temp=temp->getNext();
        }
    }
    delete temp;
    return 0;
}

//Creates a new TokenList, and returns a pointer to this list
//Searches for all conditional expressions in tokenList and appends them to the new list
//Format is as follows:
//Each token that is part of a condtional expression is appended sequentially
//At the end of a conditional expression a newline character is appened
//Example: if (a = true) then
//Your list should include "(", "a", "=", "true", ")" and "\n"
//tokenList is NOT modified
TokenList* findAllConditionalExpressions(const TokenList &tokenList)
{
        //create a new tokenlist and new temp token
        TokenList* pointer = new TokenList;
        Token* temp;
        //TokenList object;

        //make the temp token equal to the first token the tokenlist
        temp = tokenList.getFirst();

        //while the first token is not null, then goes into the loop
        while(temp != nullptr)
        {
            //to check the when to meet the conditional statement, begins with "if", "else if" and "when"
            if(temp->getStringRep() == "if" || temp->getStringRep() == "elseif" || temp->getStringRep() == "when")
            {
                temp = temp -> getNext();
                //to check when the conditional statement is ended, which ends with "then" or "else"
                while(temp ->getStringRep()!= "then" && temp ->getStringRep()!= "else")
                {
                    //if not meat with then and else, append the current token to the tokenlist and
                    //make the token points to the next
                    pointer->append(temp ->getStringRep());
                    temp = temp -> getNext();
                }
                //append the "\n" at the end of the conditional statement
                pointer->append("\n");
            }
            temp = temp -> getNext();
        }

        // pointer = object.getFirst();

        return pointer;
}
