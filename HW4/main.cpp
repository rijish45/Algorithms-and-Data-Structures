//Rijish Ganguly
//Spencer Liu


#include "InputArray.h"
#include <iostream>
#include <list>

using namespace std;

/*

If we think of the array as a complete array (none missing) of binary numbers and then sort them, the least significant
bit should alternate between 0 and 1 starting with 0. Therefore, there is either and equal amount of 0's and 1's or 0's 
exceed 1's by 1. 

If we remove a number that ends in 1 (odd), there will either be two less 1's or one less 1's depending on the relative 
amount of 1's and 0's from the complete array. If we remove a number that ends in 0 (even), there will either be an equal 
amount of 1's and 0's or there will be one more 1's than 0's.

Since all of the possible results are distinct for removing an even or odd number, we can know for sure whether the 
missing number is odd or even by counting the number of 1's and 0's. While you count the number of 1's and 0's by
Iterating through the LSB, store the index of the element in either a list of indices of even number or a list of indices
of odd numbers. 

If there are less 1's than 0's, then an odd number (number with 1 as LSB) was removed. Otherwise, the removed number is even.
If the number is odd, then add the value of the bit to the running sum that will give you the missing number. For example, 
if the least significant bit of the missing number is a 1, then you add 2^0 (value of LSB) to the running sum. Otherwise, 
you don't add anything because the bit is a 0.

Repeat this process for the bits increasing in significance. The only difference is that you add 2^k where k is the bit you
are looking at. k=0 is the LSB, and k=log(n) rounded down is the MSB. When have the missing number when we have iterated 
through all of the bits

When you discard the bits that are irrelevant (discard all odd bits if the missing number is even and vice versa), the next
larger bit will also have the property mentioned in the first and second comment paragraphs. Therefore, the process can be
repeated for larger bits until we build the missing number.


Ex: numbers from 0-5 missing 4

000
001
010
011
missing num
101

Though there are many more zeros than ones for the MSB, this will not matter when we reach the MSB.

LSB: 3 ones, 2 zeros   ->  1 more ones than zeros
	-> missing number must end in a zero bit
	missing number = 0 + 0*2^0 = 0
	discard numbers with LSB = 1

000
010
missing num

Look at the next bit:
1 ones, 1 zeros -> equal zeros and ones
	-> missing number must have zero in this bit
	missing number = 0 + 0*2^1 = 0
	discard numbers with 2nd smallest bit = 1

000
missing num

MSB:
0 ones, 1 zeros	-> 1 less ones than zeros
	-> missing number must have one in this bit
	missing number = 0 + 1*2^2 = 4
	discard numbers with MSB = 0

missing num

Changing the order of numbers doesn't change the total number of 1's and 0's for a bit, so the order of the numbers doesn't
affect my algorithm.




Runtime:


Before nested for loop:

Up to the first for loop, everything takes constant time except for the while loop. The while loop finds the number of bits
by doubling from 1 until we have a number larger or equal to the largest number. Since the operations inside the while loop are
constants, this is Θ(log n). The first isolated for loop just initializes the temp list with all of the indices of the IndexArray.
Since push_back for lists always takes linear time, this for loop is Θ(n).

Nested for loop:

The remaining part of the function has a for loop with another for loop and other instructions nested inside. The outer for loop runs
log n times because it iterates through the number of relevant bits for all of the elements in the InputArray. The inner for loop iterates
through the size of the list temp. Calling findBit, incrementing either ones or zeros, and using a push_back on either odd or even each takes
constant time. Therefore, the inner for loop takes 3*size(temp) + constant time.

The if-else block following the inner for loop takes size(temp)/2 + constant time because temp copies either odd or even, both of which is
at most half the size of what temp was originally. The worst case is copying the even list in the case where the number of 1's equals 0's. We
copy the smaller list in all other cases, which would be less than half the size.

The last part of the outer for loop is clearing lists even and odd, and then doing constant time calculations.
Size(odd) + size(even) = size(temp), so the lart part takes size(temp) + constant time.
The each iteration of the outer for loop takes [3*size(temp) + constant] + [.5*size(temp) + constant] + [size(temp) + constant]
												= 4.5*size(temp) + constant

Since with each iteration the size of temp is halved starting from size n, the nested for loop is 
the sum from k=0 to log(n) of [4.5*n/2^k + constant] 
	4.5n[sum from k=0 to log(n) of (1/2)^k] + [sum from k=0 to log(n) of constant], which is big O of
	4.5n[sum from k=0 to n of (1/2)^k] + [sum from k=0 to n of constant]
	9n + constant*n
	Θ(n)

*/

void  missingNum(InputArray* array)

{

	//Lists to store the index of the odd and even numbers and a temporary list to store the one we select
	
	list<int> even;
	list<int> odd;
	list<int> temp;
	list<int>::iterator it;

	int result = 0;									//Stores the running sum that will be the final result
	
	//Stores the counter of odd and even numbers
	int zeros = 0;
	int ones = 0;

	int size = array->n;

	//Finding the number of bits needed to store the largest number
	int largestBit = 1;
	int bitCount = 1;
	while(array->n >= largestBit)

	{					//Doubling largest bit since shifting left = x2
		largestBit = largestBit*2;
		bitCount++;
	}
	bitCount--;

	int bitValue = 1;								//Storing the value of the bit being looked at

	//Initializing temp to all of the indices of the original array
	for(int i = 0; i < size; i++)

	{
		temp.push_back(i);
	}

	//Iterating through all log(n) bits
	for(int j = 0; j <= bitCount-1 ; j++)

	{

		//Iterating through the specific bit for each element whose index is saved in temp
		//Temp should be halved after each iteration of the outer for loop
		for(it = temp.begin(); it != temp.end(); it++)

		{
			//Counting the number of "even" and "odd" numbers and storing their index in either odd or even
			//Even is defined loosely by the bit checked being a 0; bit=1 for odd
			if(array->findBit(*it, j))

			{
				ones++;
				odd.push_back(*it);
			}
			else

			{
				zeros++;
				even.push_back(*it);
			}
		}
		//Temp either copies odd or even depending on whether the missing number has a 1 or 0 in this bit
		if(ones < zeros)

		{
			result += bitValue;
			temp = odd;
		}
		else

		{
			temp = even;
		}

		//Reseting lists and counters for next iteration
		even.clear();
		odd.clear();
		zeros = 0;
		ones = 0;
		bitValue = bitValue*2; 		//Setting the value of the bit for the next larger bit
	}
	cout << result << endl;
	return;
	
}


int main()

{

	InputArray* array = new InputArray();
	missingNum(array);

}
