#include <iostream>
#include <string>
#include <fstream>
#include <ctype.h>
#include <list>
#include <stdlib.h>

using namespace std;

//Spencer Liu and Rijish Ganguly
//Total 63 second runtime
//A - 44961				(5 sec)
//B - 443088589			(30 sec)
//C - "Y1 2   2 1Y"		(28 sec)


void fourA()

{

	unsigned long count = 0;					//Number of ID's that satisfy condition
	bool id = false;							//Whether the current character is potentially part of a BU ID
	int sum = 0;								//Sum of digits of potential BU ID
	int digitCount = 0;							//Number of digits in a row following a U

	ifstream file("BigData.txt");

	char c;
	int digit;



	while(file.get(c))

	{
		

		//Potential BU ID if 'U' found
		if(!id){	
			if(c == 'U'){
				id = true;
				digitCount = 0;
				sum = 0;

			}
		}
		else{
			//Update variables if next character is number in a potential BU ID
			//Resets if a condition fails
			if(isdigit(c)){
				digitCount++;
				digit = c - '0';
				sum += digit;
				if(digitCount > 8 || sum > 68){
					id = false;
				}
			
			}
			//Handles non-numbers characters
			else{
				//Check for successful BU ID
				if(!isdigit(c) && digitCount == 8 && sum >= 31 && sum <= 68){
					count++;
				}
				//Reset unless a 'U' occurs
				if(c != 'U'){
					id = false;
				}
				//Skip reset step and directly reinitialize if 'U' found
				else{
					id = true;
					digitCount = 0;
					sum = 0;
				}
			
			}
		}
	}
	
	file.close();

	cout << "Part a: " << count << endl;
	return;
}


class trieNode
{
	public:

	trieNode* letters[52];				//0-25 = A-Z; 26-51 = a-z
	bool end;							//Marks if this node completes a word

	trieNode(){
		end = false;
		for(int i = 0; i < 52; i++){
			letters[i] = NULL;
		}
	}
};

void fourB()

{

	trieNode* trie = new trieNode();
	trieNode* triePtr = trie;
	
	ifstream dict("dictionary.txt");
	string word;

	unsigned long count = 0;

	//Creating trie

	int i;										//Index to loop through word
	int trieIndex;								//Index of letter into trie
	while(getline(dict, word)){
		i = 0;
		//Iterate through trie with each letter of each word
		//Initilalize or mark as end of word as necessary
		while(word[i]){
			//Ignore words that start with my initial
			//if(word[0] == 'S' || word[0] == 's'){
			//	break;
			//}
			//Calculating index of relevant child
			if(isupper(word[i])){
				trieIndex = word[i] - 'A';
			}
			else{
				trieIndex = word[i] - 'a' + 26;
			}
			
			//Initialize if needed
			if(triePtr->letters[trieIndex] == NULL){
				triePtr->letters[trieIndex] = new trieNode();
			}

			triePtr = triePtr->letters[trieIndex];		//Iterate to appropriate child
			if(!word[i+1]){								//Mark end if last letter of word
				triePtr->end = true;
				triePtr = trie;
			}
			i++;
		}
	}

	dict.close();



	string input;
	ifstream file("BigData.txt");

	//Searching trie using string from BigData.txt

	getline(file, input);
	unsigned long index = 0;
	unsigned long iterator = 0;
	int charIndex;


	trieNode* trieIterator = trie;
	
	while(input[index])

	{
		//Skipping non-letters and my initial
		if(input[index] == 'S' || input[index] == 's'){
			index++;
			continue;
		}
		else if(!isalpha(input[index])){
			index++;
			continue;
		}
		else{
			//Loop until fall off trie or find an invalid character
			while(1){
				//Checking for invalid characters
				
				if(!isalpha(input[index+iterator])){
					break;
				}

				//Calculating index of relevant child
				if(isupper(input[index+iterator])){
					charIndex = input[index+iterator] - 'A';
				}
				else if(islower(input[index+iterator])){
					charIndex = input[index+iterator] - 'a' + 26;
				}

				//Break loop if fall off trie
				if(trieIterator->letters[charIndex] == NULL){
					//index++;
					//iterator = 0;
					//trieIterator = &trie;
					break;
				}
				
				//Increment word counter if we come across the end of a word
				if(trieIterator->letters[charIndex]->end){
					//cout << "index " << index << "	iterator " << iterator << endl;
					count++;
				}
				trieIterator = trieIterator->letters[charIndex];
				iterator++;
			
			}
		}
		index++;
		iterator = 0;
		trieIterator = trie;
	
	}


	cout << "Part b: " << count << endl;
	return;
}




void fourC()

{

	ifstream file("BigData.txt");
	string input;
	getline(file, input);


	//The longest palindrome I found was size 11
	//The following commented out part looks for palindromes of size 12 and 13
	//If there does not exist a palindrome of length x, then there must be no palindrome of length x+2n where n is any non-negative integer because you can always make a smaller palindrome out of a larger palindrome by cutting off the ends


	unsigned long size = input.size();							//Size of BigData.txt	604501071
	unsigned long lowIndex = (size-1)/2;						//Index of the lower center of palindromes
	unsigned long highIndex;									//Index of the higher center of palindromes
	unsigned long longest = 1;									//Size of longest palindrome found so far, must be at least 1
	unsigned long tempLength;									//Length of current palindrome being checked
	string palindrome;											//Longest palindrome found so far
	string tempPalin;											//Current palindrome
	bool odd = size%2;											//Whether current palindrome will have an odd or even size
	unsigned long n = 0;										//Used to iterate through string without changing index values

	//Corner case
	if(size == 1){
		cout << input << endl;
		return;
	}
		
	//Special case for the first center because highIndex = lowIndex
	if(odd){
		palindrome = input[lowIndex];
		lowIndex--;
		highIndex = lowIndex + 2;
		tempLength = 1;
		
	}
	else{
		palindrome = "";
		highIndex = lowIndex + 1;
		tempLength = 0;	
	}

	while(input[lowIndex-n] == input[highIndex+n]){
		palindrome = input[lowIndex-n] + palindrome + input[highIndex+n];
		tempLength += 2;
		if(lowIndex-n <= 0){
			break;
		}
		n++;
	}
	if(tempLength > longest){
		longest = tempLength;
	}
	odd = !odd;
	

	while(lowIndex > (longest-1)/2)

	{			//>= or > depends on if we need to find first occurence of longest palindrome or just a longest palindrome

	//Searching for longeset palindrome with center at the lowIndex
		//If palindrome is odd (single character center)
		if(odd){
			tempPalin = input[lowIndex];
			n = 1;
			tempLength = 1;

			//Looking for longest palindrome possible with lowIndex as center
			while(input[lowIndex-n] == input[lowIndex+n]){
				tempPalin = input[lowIndex-n] + tempPalin + input[lowIndex+n];
				tempLength += 2;
				if(lowIndex-n <= 0){
					break;
				}
				n++;
			}
			//Update longest palindrome if necessary
			if(tempLength > longest){
				palindrome = tempPalin;
				longest = tempLength;
			}

		}

		//If palindrome is even (double character center)
		else{
			tempPalin = "";
			n = 0;
			tempLength = 0;

			//Looking for longest palindrome possible with highIndex as center
			while(input[lowIndex-n] == input[lowIndex+1+n]){
				tempPalin = input[lowIndex-n] + tempPalin + input[lowIndex+1+n];
				tempLength += 2;
				if(lowIndex-n <= 0){
					break;
				}
				n++;
			}
			//Update longest palindrome if necessary
			if(tempLength > longest){
				palindrome = tempPalin;
				longest = tempLength;
			}
		}

		
	//Searching for longest palindrome with center at the highIndex
	//Similar implementation with minor changes as implementation for lowIndex
		if(odd)

		{
			tempPalin = input[highIndex];
			n = 1;
			tempLength = 1;

			while(input[highIndex-n] == input[highIndex+n]){
				tempPalin = input[highIndex-n] + tempPalin + input[highIndex+n];
				tempLength += 2;
				if(highIndex+n >= size){
					break;
				}
				n++;
			}
			if(tempLength > longest){
				palindrome = tempPalin;
				longest = tempLength;
			}
		}
		else{
			tempPalin = "";
			n = 0;
			tempLength = 0;

			while(input[highIndex-1-n] == input[highIndex+n]){
				tempPalin = input[highIndex-1-n] + tempPalin + input[highIndex+n];
				tempLength += 2;
				if(lowIndex+n >= size){
					break;
				}
				n++;
			}
			if(tempLength > longest){
				palindrome = tempPalin;
				longest = tempLength;
			}
		}
		
		if(odd){
			lowIndex--;
			highIndex++;
		}
		
		odd = !odd;
		

	}
		

	file.close();

	cout << "Part c: " << palindrome << endl;
	return;
}


int main(){
	
	fourA();
	fourB();
	fourC();

	return 0;
}


