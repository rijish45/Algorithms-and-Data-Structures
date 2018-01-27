#include <iostream>
#include <string>
#include "MyBloomFilter.h"
#include "BloomFilter.h"

//Spencer Liu and Rijish Ganguly

using namespace std;

//Adjusting filter size
MyBloomFilter::MyBloomFilter(int len) : BloomFilter(len)


{
	filter.resize(length);

}


void MyBloomFilter::insert(string item)

{

	//Using 32 bit values because long is 4 bytes
	unsigned long offset = 2166136261;
	unsigned long prime = 16777619;

	//Hash function
	for(unsigned int i = 0; i < item.size(); i++){
		offset *= prime;
		offset = offset ^ item[i];
	}

	int index = offset % (8*length);		//Calculating the overall index of the bit of the filter to be set to 1
	int charIndex = index/8;				//Calculating which char the bit belongs to
	int bitIndex = index % 8;				//Calculating which bit of the char the bit belongs to

	//Creating a char with only the bitIndex bit set
	int bit = 0;
	if(bitIndex != 0)

	{
		bit = 1;
		for(int j = 1; j < bitIndex; j++)
		{
			bit *= 2;
		}
	}

	filter.at(charIndex) = filter.at(charIndex) | bit;		//bitwise OR to set the appropriate bit to 1, if not already set to 1
															//Won't change other bits since the other bits are ORed by 0

}


bool MyBloomFilter::exists(string item)

{

	//Same calculation step as insert
	unsigned long offset = 2166136261;
	unsigned long prime = 16777619;

	//Hash function
	for(unsigned int i = 0; i < item.size(); i++)

	{
		offset *= prime;
		offset = offset ^ item[i];
	}
	int index = offset % (8*length);
	int charIndex = index/8;
	int bitIndex = index % 8;

	char bit = 0;
	if(bitIndex != 0)

	{
		bit = 1;
		for(int j = 1; j < bitIndex; j++)
		{
			bit *= 2;
		}
	}


	char result = filter.at(charIndex) & bit;			//bitwise AND to check if appropriate bit is set
														//All irrelevant bits will be reset to 0 since they will be ANDed with 0
														//Relevant bit will be set to 1 if the relevant filter bit is also 1, 0 otherwise
														//Therefore result = 0 if relevant bit is 0; result > 0 otherwise
	if(result > 0)

	{
		return true;
	}
	else

	{
		return false;
	}
}

//Converting each char of the filter directly into a char to be concatenated into the output string
string MyBloomFilter::output()

{
	string filterString = "";
	for(vector<char>::iterator it = filter.begin(); it != filter.end(); it++)
	{
		filterString += *it;
	}
	return filterString;
}
