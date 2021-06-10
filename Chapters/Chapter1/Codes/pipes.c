#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char* argv[]) {
    
    int arr[] = {1, 2, 3, 4, 3, 2, 1};
    int arrSize = sizeof(arr) /  sizeof(int);
    int start, end;

    int fd[2];
    if (pipe(fd) == -1) {
	printf("An error ocurred with opening the pipe\n");
	return 1;
    }

    int id = fork();

    if (id == -1) {
	printf("An error ocurred with child process\n");
	return 2;
    }

    if (id == 0) {
	// Child process
	start = 0;
	end = start + arrSize / 2;
    } else {
	// Parent process
	start = arrSize / 2;
	end = arrSize;
    }

    int sum = 0;

    for (int i = start; i < end; i++) {
	sum += arr[i];

    }

    printf("Calculated partial sum: %d\n", sum);

    if (id == 0) {
	close(fd[0]);
	write(fd[1], &sum, sizeof(sum));
	close(fd[0]);
    } else {
	int sumFromChild;
	close(fd[1]);
	read(fd[0], &sumFromChild, sizeof(sumFromChild));
	close(fd[0]);

	int totalSum = sum + sumFromChild;
	printf("Total sum is %d\n", totalSum);
	wait(NULL);
    }

    // Calculated partial sum: 10   
    // Calculated partial sum: 6
    // Total sum is 16     

    return 0;
}


