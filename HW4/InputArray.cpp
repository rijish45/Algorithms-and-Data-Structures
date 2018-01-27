//
//  MissingNum.cpp
//  MissingNum
//
//  Copyright Â© 2017 Tali Moreshet. All rights reserved.
//

#include "InputArray.h"
#include <iostream>

using namespace std;

InputArray::InputArray()

{
    this->n = 7;
    this->Array = new int[7] {7, 5, 2, 0, 1, 3, 6};
}

InputArray::~InputArray()
{
    delete [] Array;
}

// return true if 'bit' in 'element' of Array is set to 1
bool InputArray::findBit(int element, int bit) 

{
    bitset<32> Ai = Array[element];
    return (Ai[bit] == 1);
    
}