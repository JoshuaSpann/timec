/**
 * timec: THE TIME CONVERTER
 *  author:   Joshua Spann
 *  date:     20180608:1035
 *  liscense: GPLv2
 **/
#include <stdio.h>
#include <inttypes.h>


int main(int argcount, char **args) {
//	printf("Time converter\n");
	printf("\n");

	// Hold marker where seconds start //
	int secondsi = 0;

	// Go through each argument //
	for (int i=1; i<argcount; i++) {
		// Temp segregated time values for later conversion //
		char minuteschar[10];
		char secondschar[10];

		// Converted numbers go here //
		float minutes = 0;
		float seconds = 0;
		float totaltime = 0;

		// Iterate through all characters in argument //
		for (int j=0;j<sizeof(args[i])+2;j++) {
			char argch = args[i][j];

			// Find minutes-seconds marker //
			if (argch == ':') {
				secondsi = j;
				continue;
			}

			// Print out minutes only //
			if (j > secondsi && secondsi != 0) {
				secondschar[j-secondsi-1] = argch;
				continue;
			}
			minuteschar[j] = argch;
		}

		// Convert minutes and seconds placeholders into floats //
		minutes = strtoumax(minuteschar, NULL, 10);
		seconds = strtoumax(secondschar, NULL, 10);
		totaltime = minutes+(seconds/60);

		// Output converted, total value to user //
		//printf("%s minutes, %s seconds:\t", minuteschar, secondschar);
		printf("%s = ", args[i]);
		printf("%f minutes\n", totaltime);
	}
	printf("\n");

	return 0;
}
