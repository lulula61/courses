#include<stdio.h>
#include<unistd.h>
#include<sys/types.h>

int main() {
	pid_t pid;
	int input;
	scanf("%d", &input);
	pid = fork();
	if (pid < 0) {
		fprintf(stderr, "Fork Failed");
		return 1;
	}
	else if (pid == 0) {
		printf("input: %d", input);
		int i = input;
		while (i > 1) {
			printf("%d ", i);
			if (i % 2) {  // i is odd
				i = 3 * i + 1;
			}
			else {  // i is even
				i /= 2;
			}
		}
	}
	else {
		wait(NULL);
	}
	return 0;
}
