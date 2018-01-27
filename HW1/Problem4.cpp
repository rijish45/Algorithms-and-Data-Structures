//Rijish Ganguly
//EC 330
//HW1


#include <iostream>
#include <sstream>
#include <string>
#include <fstream>


using namespace std;

int main(int argc, char* argv[])  //arg is the number of command line inputs and the character array stores the arguments

{
   char* filename = argv[1];  //storing the filename from the command line input
   ifstream inputfile(filename);
   string initial = " ";
   string result;

   while(getline(inputfile, result))
   {
     if (result.length() > initial.length()) //comparing the length of the read string with the initial one
     {
       initial = result;  //updating the initial string if necessary
     }
   }

   cout << initial  << endl;



}
