/* Name : Rohithram R | EE16B031
   Date : 11-11-2018
   Question : 5
   Aim  : To find the maximum possible returns for initial investment.
*/

#include <stdlib.h>
#include <stdio.h>

using namespace std;

/*
Algorithm Used : Iterative Dynammic Programming based

* It can be shown that investing all the amount in one of the investment option is better than 
investing partially amoong them in each year.

* We start 1st year by investing all the money in each of the option and calculate return at end of
year 1 store it an array and we find the all n^2 possible ways of investing in second year
i.e we find which investment option in year will give max return at end of year2's jth option
and we store that option to backtrack later, we do this for all n options in year2.

* Extending this approach to ith year and (i+1)th year, we store which investment option in 
year i gives max return at the end of (i+1)th year , jth option, so like this we store these indexes
of options in year i for all N investment options in year (i+1) and for t years 
we get a matrix 'track_inv_options' of indices of investment option which is used for bactracking best path of 
investment options in t years.

* So for finding which option gives max return for each option in year i+1, takes O(N^2), 
like this for t years , so time Complexity : O(t*N^2).

* We maintain retTable[i][j] to store optimal investment amounts at start of year i option j.
 Its initially started with 'S' investment in year 1 and all others 0.

* We update retTable[i+1][j] with return from ith year kth option to jth option in year i+1 which
gives maximum possible return at jth option in year (i+1).

* Finally we calculate the max return from last row of retTable and find the best option at year
t and then we back track using 2D matrix 'track_inv_options' from year t to 1. Thus finding the best strategy

*/

// Function to find the optimal strategy
int plan_investment_strategy(float** rr,double s, int t, int n, float f){
    
    float max_return = 0;
    // Optimal investment returns at each year for each investment option. 
    float *retTable[t];  
    
    // Matrix which stores indexes of best options each year
    int *track_inv_options[t];

    // Temp Arrays to store returns at year i and i+1
    float *inv_return_i = (float*) malloc(sizeof(float)*n);
    float *inv_return_iplus1 = (float*) malloc(sizeof(float)*n);
        
    int i,l,j,k;

    // Initialising arrays.
    for(i=0;i<t;i++){
        retTable[i] = (float*) malloc(sizeof(float)*n);
        track_inv_options[i] = (int*) malloc(sizeof(int)*n);

        for(j=0;j<n;j++){

            track_inv_options[i][j] = 0;
            inv_return_iplus1[j] = 0;
            inv_return_i[j] = 0;

            if(i==0)
                retTable[i][j] = s;  // initialising 1st year with S
            else
                retTable[i][j] = 0;
        }
    }


    // Looping through t years
	for(i=0; i<t-1; i++){

        //loop to find investment option in ith year which gives max return in
        // (i+1)th year, kth option. Repeat for all options in year i+1.
        for(k=0;k<n;k++){

            for(j=0; j<n; j++){
                
                // return in year i
                inv_return_i[j] = retTable[i][j]*rr[i][j];

                // Applied fee if investment option is changed
                if(j!=k && ((inv_return_i[j]-f)*rr[i+1][k] > inv_return_iplus1[k])){

                    inv_return_iplus1[k] = (inv_return_i[j]-f)*rr[i+1][k];
                    //tracking index of investment option in year i which gives max return
                    // in kth option. at year i+1
                    track_inv_options[i+1][k] = j;   

                }else if( j==k && (inv_return_i[j]*rr[i+1][k] > inv_return_iplus1[k])){

                    inv_return_iplus1[k] = inv_return_i[j]*rr[i+1][k];
                    track_inv_options[i+1][k] = j;
                }

            }
        }
        
        // To update investments in year i+1 in retTable
        for(l=0;l<n;l++){

            // Fee applied when invest option changed
            if(l!=track_inv_options[i+1][l]){
                retTable[i+1][l] = inv_return_i[track_inv_options[i+1][l]] - f;
            }else if(l==track_inv_options[i+1][l]){
                retTable[i+1][l] = inv_return_i[track_inv_options[i+1][l]];                
            }
        }

    }

    // To find the max return at the end of t years
    max_return = retTable[t-1][0]*rr[t-1][0];
    int max_return_index = 0;

    for(l=0;l<n;l++){
        if(retTable[t-1][l]*rr[t-1][l]>max_return){
            max_return = retTable[t-1][l]*rr[t-1][l];
            max_return_index = l;
        }
    }

    // Back tracking from year t to 1.
    // max_return_index stores best option at year t
    for(i=t-1;i>=0;i--){
        for(l=0;l<n;l++){
            if(l!=max_return_index){
                retTable[i][l] = 0;
            }
        }
        // updates to best option using backtracking
        max_return_index = track_inv_options[i][max_return_index];  
    }
    
    //printing the investments at each year.
    for(int i=0; i<t; i++){
		for(int j=0; j<n; j++)
            printf("%.5f ",retTable[i][j]);
        
        printf("\n");
	}

   return 0;
}

int main(){

	double s;
	scanf("%lf\n",&s); 	  // total investment
	int t,n;
	float f;
	scanf("%d\n",&t);	  // no of years to invest
	scanf("%d\n",&n);     // no of instruments to invest in
	scanf("%f\n",&f);     // fee to be leived if investment instrument is changed (Assumption : for the first year, no fee)


	float *rr[t];         // array to store all the returns for investment options

	// taking the returns for given years and for no of investments
	for(int i=0; i<t; i++){

        rr[i] = (float*) malloc(sizeof(float)*n);

		for(int j=0; j<n; j++){
			scanf("%f",&rr[i][j]);	// (i+1)th year and (j+1)th investment option
		}
        
	}
	
    plan_investment_strategy(rr,s,t,n,f);

    return 0;
}
