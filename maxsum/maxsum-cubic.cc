#include <stdio.h>

int maxsumCubic(int *x, int N) 
{
    int maxsum = 0;
    for (int i=0; i<N; i++) {
        for (int j=i; j<N; j++) {
            int thissum = 0;
            for (int k=i; k<=j; k++) {
                thissum += x[k];
            }
            if (thissum > maxsum) {
                maxsum = thissum;
                // printf("%d: %d %d\n", maxsum, i, j);
            }
        }
    }
    return maxsum;
}

int main() 
{
    // Read in the number of values in array as first entry
    int N;
    scanf("%d", &N);
    int *x = new int[N];

    // Read in the data
    for (int i=0; i<N; i++) {
        scanf("%d", &x[i]);
    }

    // Compute the max subsequence sum
    int maxsum = maxsumCubic(x, N);
    printf("%d\n", maxsum);

    return 0;
}
