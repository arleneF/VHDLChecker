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
    if (offset == len) //when you traverse to the end of the list
		{complete = true;}
    else if (!complete)
    {
        if(comment) //if next token is comment
        {   tokenLength = len-offset;
            return;
        }
        else
        {       //meet this condition when what we read is not comment
            while (!found && index<len)
            {
                switch (str->at(index))
                {
                case ' ':
                    found = true;
					if (index == offset)
					{ //when index and offset represent the same position
						if (index == len){ //at the end of the string
							complete = true; //make two boolean be true to get out of the loop
							found = true;}
						else {
							offset++;
							index++;
							found = false;}
                    }
                    tokenLength=index-offset;
					break;
                case '<':
                    if(tokenLength==0 && str->at(index+1)=='='&& str->at(index+1)!=len)
                    break;
                case '+':
                    if(tokenLength==0){
                        tokenLength=1;
                    }
                    found = true;
                    break;
                case '&':
                    if(tokenLength==0){
                        tokenLength=1;
                    }
                    found = true;
                    break;
                case '#':
                    if(tokenLength==0){
                        tokenLength=1;
                    }
                    found = true;
                    break;
                default:
                    index++;
                }

            }

        }
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
	else {
		//if the string has characters in it then you initialize some variables and prepare the next token for the get next token funtion
		complete = false;
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

	if (comment) {comment = false;}
	if (temp == "--") {comment = true;}
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
	}
  else {return;} //when list is null
}

//Removes all comments from the tokenList including the -- marker
//Returns the number of comments removed
int removeComments(TokenList &tokenList) {/*Fill in implementation */ }


