
//Rijish Ganguly
//EC330
//HW1


/*
When we notice the problem the first thing that strikes our mind is the process to create the grid. We start by creating the grid by using 
two loops (rows and columns) and printing underscores. Then we observe the pattern for the x and o. We realise that we have to replace 
all the underscores along the line y = x and y = x+1. The o's are printed in a different manner depending upon whether N is odd or even. 
If N is an even nummber, change the underscores at points (N+1, N+1) to an o. Otherwise if N is an odd number, change the underscores
at points (N+2, N) and (N+2, N+2) to an o.The algorithm can be derived by observing the patterns provided. By simple application of 
co-ordinate geometry we can formulate the oordinates of points where x and o should be printed. 
*/


#include <iostream>
#include <stdlib.h>
#include <stdio.h>

using namespace std;

int main (int argc, char * argv[])

{
  int num = atoi(argv[1]);   // converting character array into integer 

  if(num %2 == 0)

  {
        
        for(int i = 9; i >= 0; i--)   // the outer loop stands for the row and it keeps decreasing as we reach towards row 0
        {
            
            for(int j = 0; j <= 9; j++)  // the inner loop stands for the column and keeps increasing 
            {
                
                if((i == j && j <= num ) || ( i == j+1 &&  j <= num))  //conditions for printing x

                { 
                    
                    cout << " x ";
                }
                
                else if ( j == num+1 && i == num+1)  //conditions for printing o
                
                {
                    
                    cout << " o ";
                }
                
                else
                       
                {
                    cout << " _ ";
                }
            }
            
            cout << endl;
        }
    }


 else if ( num%2 != 0) //considering the case when num is odd, everything remains the same apart from the conditions for printing o

 	 {
 	 	 for(int i = 9; i >= 0; i--)
        {
            
            for(int j = 0; j <= 9; j++)
            {
                
                if((i == j && j <= num ) || ( i == j+1 &&  j <= num)) //conditions for printing x

                {
                    
                    cout << " x ";
                }
                
                else if( ( i == num && j == num +2) || (j == num +2 && i == num+2)) //conditions for printing o

                {
                    
                    cout << " o ";
                }
                
                else

                {
                    cout << " _ ";
                }
            }
            
            cout << endl;
 	    }
 	}


return 0;

}


