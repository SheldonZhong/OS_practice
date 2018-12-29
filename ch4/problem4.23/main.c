#include <omp.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <stdlib.h>

int hits = 0;

double frand()
{
    return (double)rand() / (double)RAND_MAX;
}

int main(int argc, char *argv[])
{
	/* sequential code */
	srand(time(NULL));
	int total = atoi(argv[1]);
	double x, y;

	#pragma omp parallel for
    for (int i = 0; i < total; i++)
    {
        x = frand();
        y = frand();

        if ((x * x + y * y) < 1)
        {
            hits++;
        }
    }

	/* sequential code */
	printf("Monte Carlo Pi: %f\n", 4 * (double)hits / (double)total);
	return 0;
}
