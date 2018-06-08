/**
 * timec: THE TIME CONVERTER
 *  author:   Joshua Spann
 *  date:     20180608:1035
 *  liscense: GPLv2
 **/
#include <stdio.h>
#include <inttypes.h>
#include <string.h>

#define VERSION "1.0"

void hours_minutes_to_float(int argcount, char **args)
{
	// Hold marker where seconds start //
	int secondsi = 0;

	// Go through each argument //
	for (int i=0; i<argcount; i++) {
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
}

void print_help()
{
	char *help = 	"timec: The Simple Time Converter v"
					VERSION
					"\nUsage:\n"
					"\ttimec [<time>...]\n"
					"\n";
	printf("%s", help);
}

int main(int argcount, char **args)
{
	//printf("Time converter\n");
	printf("\n");

	// Hold total number of arguments vs options passed in //
	int total_args = 0;
	int total_opts = 0;

	// Get total amount of arguments vs options from parameters //
	for (int i=1;i<argcount;i++) {
		if (args[i][0] == '-') {
			total_opts++;
			continue;
		}
		total_args++;
	}

	// Hold separate arrays of arguments and options //
	char *arguments[total_args];
	char *options[total_opts];

	// Reuse counters to save memory //
	total_args = 0;
	total_opts = 0;

	// Split arguments and options to their own arrays //
	for (int i=1;i<argcount;i++) {
		// Assign options //
		if (args[i][0] == '-') {
			options[total_opts] = args[i];
			total_opts++;
			continue;
		}
		// Assign arguments //
		arguments[total_args] = args[i];
		total_args++;
	}

	for (int i=0;i<sizeof(options);i++) {
		if (strcmp(options[i], "-h") || strcmp(options[i], "--help")) {
			print_help();
			return 0;
		}
	}

	if (sizeof(arguments) > 0) 
		hours_minutes_to_float(total_args, arguments);

	printf("\n");

	return 0;
}
