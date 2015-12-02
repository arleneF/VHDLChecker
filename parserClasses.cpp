//Use only the following libraries:
#include "parserClasses.h"
#include <string>
//****TokenList class function definitions******
//           function implementations for append have been provided and do not need to be modified

//Creates a new token for the string input, str
//Appends this new token to the TokenList
//On return from the function, it will be the last token in the list
void TokenList::append(const string &str) {
	Token *token = new Token(str);
	append(token);
}

Token::Token(const Token &token){
    next = token.getNext();
    prev = token.getPrev();
    stringRep = token.getStringRep();
    _isKeyword = token.isKeyword();
    type = token.getTokenType();
    details = token.getTokenDetails();
}

void Token::operator =(const Token& token){
    next = token.getNext();
    prev = token.getPrev();
    stringRep = token.getStringRep();
    _isKeyword = token.isKeyword();
    type = token.getTokenType();
    details = token.getTokenDetails();
}

void Token::setTokenDetails(const string &type, int width = 0){
    tokenDetails *newnode = new tokenDetails;
    newnode->width = width;
}

Token::~Token(){
    if (next)
        delete[] next;
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

//Computes a new tokenLength for the next token
//Modifies: size_t tokenLength, and bool complete
//(Optionally): may modify offset
//Does NOT modify any other member variable of Tokenizer
void Tokenizer::prepareNextToken()
{   size_t len = str->length();
    bool found =false;
    int index=offset;
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
                        temp_comment = index+2;
                    }
                    found = true;
                    return;
                }
                //all the delimiter with only itself are indicated here
                //delimieters include #();|&+.
                //tokenLength are all equal 1
                else if(str->at(index)=='#'||str->at(index)=='(' ||str->at(index)==')'||str->at(index)==';'||str->at(index)=='|'||str->at(index)=='&'
                    ||str->at(index)=='+'||str->at(index)=='.'||str->at(index)==','){
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
                        if(str->at(index-1)=='X'||str->at(index-1)=='x'||str->at(index-1)=='B'||str->at(index-1)=='b'||str->at(index-1)=='O'
                        ||str->at(index-1)=='o'){
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
                else {
                index ++;}
            }
        }
        if (index == len) {
			tokenLength = len - offset;
        }
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
