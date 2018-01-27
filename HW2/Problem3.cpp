/* Rijish Ganguly
   Spencer Liu

The algorithm is composed of two parts. We have two vectors of vector of ints which serve two different purposes.
The first vector of vector is used to find the longest increasing sequence in the input array and the second vector keeps track 
of the largest sequence among the longest sequences.

Part A:

The first vector contains vectors which are potential candidates for the longest sequence. Three convenient features of this vector of vector of arrays 
is that 
1) Each vector is always going to be one larger than the previous vector in size with its largest element larger than the previous vector
2) The first vector will be size 1, and it will always be the smallest number so far.
3) The largest number of each vector of ints will be at the end of the vector.

Condition 1:

Start by setting the first element of the array in a vector of size 1 as the first vector in the vector of vectors of seq.
For every following number, if the new number is bigger than the largest element of the longest array, copy the longest array 
and place this new number at the end of the copied vector. This is because there is no guarantee that the new vector will necessarily 
be longer than the prior longest one.

Condition 2: 
If the next number is smaller than the largest element of the longest vector, iterate backwards until you find the first the vector whose largest 
element is smaller than the next number. Copy the vector and place the copy right after the vector you stopped on. Then add the new number 
to the end. Then delete the following vector which must be the same length as the newly created vector.
The vector can be deleted because it has the same length as the newly created vector, but the newly created vector has a smaller 
largest element, so it covers all the possible cases the deleted vector can cover plus more.

Condition 3:

Lastly, if the next number is smaller than all of the previous numbers (which is conveniently just the first vector (size 1) in the vector 
of vectors), replace the first vector with a vector of this new number (size 1). This also maintains the 3 convenient features mentioned above.

Part B: 

In the second vector of vector of ints, keep track of the largest of the longest sequence of increasing numbers.

Start the same way as part 1 by using the first element of the array as a vector of size 1 for the first vector of vectors.

If condition 1 occurs, add the new number to all vectors in which the largest element is smaller than the new number.
When executing condition 1, a vector is replaced by another vector that is the same length but smaller.
The smaller vector is ideal for finding the longest vector, but if no more longer vectors are found, the largest one has been deleted.
Adding the new number ensures that the largest element that can be used is still remembered until it becomes useless.
For the vectors in which the largest number are not larger than this new number, delete this vector because it has no potential 
to be the longest.This ensures that the vector of vectors only contains the longest vectors, since the shorter ones are being replaced.

If condition 2 occurs, you want to hold on to the vector that is being replaced only if the vector is the longest (last vector) of the vectors 
from part 1. The vector from part 1 being replaced only if its largest element is larger than the current number, so it has the potential to 
be the largest longest sequences, which is why it is being saved. If its not the last vector, it cannot be the largest of the longest, 
so it is irrelevant. If further replacements of the last vector are made, these vectors are going to be just as long but smaller.
These still need to be saved even if they are smaller because if a new largest number is found in the future, it may not work with some of 
the largest of the longest, but it can work with the smaller of the longest. Then the smaller of the longest will become the new largest of 
the longest.

*/

#include <iostream>
#include <vector>

using namespace std;

void MaxIncSeq(int *nums, int len)

{


    vector<vector<int> > seq;					//Vector of vectors of increasing length to find the longest sequence
    vector<vector<int> > maxSeq;					//Vector of vectors of the longest sequence to find the largest
    vector<int> insVec;							//Vector used for inserting into vector of vectors 
    
    vector <vector<int> >::iterator index;		//Iterators
    vector <vector<int> >::iterator index2;
    vector<vector<int> >::iterator maxSeqIndex;

    vector<int>::iterator test;

    seq.push_back(vector<int>());
    seq.front().push_back(nums[0]);

    maxSeq.push_back(vector<int>());
    maxSeq.front().push_back(nums[0]);
    
    index = seq.begin();
    

	//Starting with the second element because the first was already stored
    for(int i = 1; i < len; i++)
    {

		//Checking for condition 1
        if(nums[i] > seq.back().back())

        {

			//Duplicating the previous largest vector and adding the new number
            seq.push_back(seq.back());            
            seq.back().push_back(nums[i]);
            index++;
			
			//Updating maxSeq
            for(maxSeqIndex = maxSeq.begin(); maxSeqIndex != maxSeq.end();)
            
            {
				//Adding new number as long as the vector is still increasing, otherwise delete
                if(nums[i] > maxSeqIndex->back())

                {
                    maxSeqIndex->push_back(nums[i]);
                    maxSeqIndex++;
                }
                
                else

                {
                    maxSeqIndex = maxSeq.erase(maxSeqIndex);

                }
            }
       }
        else if(nums[i] < seq.front().back())

        {
			//Replacing the first (size 1 and smallest) vector with the new smallest number
            insVec.push_back(nums[i]);
            seq.insert(seq.begin(), insVec);
            insVec.clear();						//Clearing incase needed in the future
            index = seq.begin();
            index++;
            seq.erase(index);					//Erasing the previous shortest vector
            index = seq.end();					//Setting the iterator to point at end
            index--;
        }


        else

        {
            bool last = true;		//Corner case to avoid seg faults if the last (longest) vector is being replaced
            index = seq.end();		//Points to the element past the last element
            index -= 2;				//Else can only execute if the largest of the longest is larger than the next num, so no need to check again. Also makes updating the boolean easier.

			//Finds the vector with the largests element that is smaller than the next number.
			//This is the vector that will be copied, but it is the vector that is one larger that will be deleted to maintain the smallest possible largest element
            while(index->back() >= nums[i])

            {
                index--;
                last = false;				//If this while loop iterates at least once, we will not be deleting the last vector
            }

			//Saving index2 for the vector to be copied and index to be the vector to be deleted
            index2 = index;
			index++;
			//Copying
            index = seq.insert(index, *index2);
			//Conditioned deleting to avoid seg fault
			if(last)

			{
				index--;
				seq.pop_back();
				index++;
			}
			else

			{
				index++;
				seq.erase(index);
				index--;
			}
            
			//Only add to maxSeq if the vector being replaced is one of the longest length so far
            index->push_back(nums[i]);
            if(last)

            {
                maxSeq.push_back(*index);
            }
            index = seq.end();
            index--;
        }
    }


    for(test = maxSeq.front().begin(); test != maxSeq.front().end(); test++)

    {
        cout << *test << " ";
    }

    cout << endl;
         
}

/*int main () 

{
	int array [6] = {7, 4, 1, 10, 23, 2};
	MaxIncSeq(array, 6);

	int nums [5] = {1,5,3,2,4};
	MaxIncSeq(nums, 5);


    return 0;

}*/