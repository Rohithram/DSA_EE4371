// Name : Rohithram R | EE16B031
// Question : 3
// Date : 11-11-2018 

#include <stdlib.h>
#include <stdio.h>

// Algorithm used : Round Robin Scheduling Algorithm using N polygon method.
/*

Reference : https://nrich.maths.org/1443  
Algo is referred from idea given in this website

Why does this work?

The restriction that no vertex has more than one segment drawn to/from it ensures that no team is scheduled for more than
one game in each round.

Restricting ourselves to horizontal stripes ensures that no segment is a rotation or reflection of another segment. 
This means that no pairing will be repeated in a future round.

*/

/*
Implementation:
N teams are stored in an array of size N. values being 1,2.....N
We fix the Nth team and rotate the elements in 1,2,.....N-1 -> N-1,1,2,.....N-2.
And we pair up 1 & N, 2 & N-1,.... on Day 1
After rotating the array we again pair in the same manner, but just elements are
rotated so new teams will be paired up each time.
We repeat this for N-1 days and we get the correct schedule.
Complexity : O(N^2), since each day rotation takes O(N) and for N-1 days : O(N^2).
*/

//Function to rotate by keeping the last team fixed.
void rotate(int* teams, int N) 
{ 
    int x = teams[N - 2], i; 

    for (i = N - 2; i > 0; i--){
        teams[i] = teams[i - 1];  
    }

    teams[0] = x; 
} 

//Function to schedule matches
int schedule_matches(int *teams,int N,int M){

    int i=0,j=0,m=0,n=N-1;

    for(i=0;i<N-1;i++){
        m=0; n=N-1;
        for(j=0;j<N/2;j++){
            if(i<M){
                printf("%d %d\n",teams[m],teams[n]);
                // m and n are used for pairing the teams.
                m++;
                n--;
            }else{
                break;
            }
        }

        rotate(teams,N);
    }

    return 0;
}

int main(){

    int N,M,i=0;

    scanf("%d\n",&N);  // no of teams
    scanf("%d",&M);    // no of days of schedule to print

    int* teams = (int*) malloc(sizeof(int)*N);

    for(i=0;i<N;i++){
        teams[i] = i+1; //To iniatialise teams from 1,...N-1.
    }

    schedule_matches(teams,N,M);

}

