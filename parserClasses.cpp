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
{
    if (offset == str->length()) //when you traverse to the end of the list
		{complete = true;}
    if (!complete){

    }
}

//Sets the current string to be tokenized
//Resets all Tokenizer state variables
//Calls Tokenizer::prepareNextToken() as the last statement before returning.
void Tokenizer::setString(string *str) {/*Fill in implementation */ }

//Returns the next token. Hint: consider the substr function
//Updates the tokenizer state
//Updates offset, resets tokenLength, updates processingABC member variables
//Calls Tokenizer::prepareNextToken() as the last statement before returning.
string Tokenizer::getNextToken() {/*Fill in implementation */ }



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
  else {return 0;} //when list is null
}

//Removes all comments from the tokenList including the -- marker
//Returns the number of comments removed
int removeComments(TokenList &tokenList) {/*Fill in implementation */ }


