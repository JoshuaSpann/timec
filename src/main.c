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

/**
 * Return [hh,mm] from "hh:mm"
 **/
int* numbers_from_argument(char *arg)
{
	static int numbers[2] = {0,0};

	// Hold marker where seconds start //
	int secondsi = 0;

	// Temp segregated time values for later conversion //
	char minuteschar[10];
	char secondschar[10];

	// Converted numbers go here //
	int minutes = 0;
	int seconds = 0;

	// Iterate through all characters in argument //
	for (int j=0;j<sizeof(arg)+2;j++) {
		char argch = arg[j];

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

	numbers[0] = minutes;
	numbers[1] = seconds;
	return numbers;
}

// TODO convert number_before_colon to 12-hour time and return output string
int clock24_to_clock12(int military_hour)
{
	static int standard_hour = 0;
	standard_hour = military_hour;
	if (military_hour > 12) standard_hour = military_hour - 12;

	return standard_hour;
}

/**
 * Convert xx:yy to float value of x (converts y to x's)
 **/
//void hours_minutes_to_float(int argcount, char **args)
void hours_minutes_to_float(int argcount, char **args, int hour_format)
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

		minutes = numbers_from_argument(args[i])[0];
		seconds = numbers_from_argument(args[i])[1];

		if (hour_format == 12) minutes = clock24_to_clock12(minutes);

		totaltime = minutes+(seconds/60);

		// Output converted, total value to user //
		printf("%s = ", args[i]);
		printf("%f minutes\n", totaltime);
	}
}

/**
 * Display help messages
 **/
void print_help()
{
	char *help = 	"timec: The Simple Time Converter v"
					VERSION
					"\nUsage:\n"
					"  timec [<time>...]\n"
					"  timec -o | --option [<time>...]\n"
					"\nOptions:\n"
					"  -h | --help\tDisplay this help message\n"
					"\n";
	printf("%s", help);
}

/**
 *  Get total amount of arguments vs options from parameters
 **/
int* get_total_number_args_options(int argcount, char **args)
{
	static int argsOpts[2] = {0,0};
	int total_args = 0;
	int total_opts = 0;

	// Filter number of args from number of opts //
	for (int i=1;i<argcount;i++) {
		if (args[i][0] == '-') {
			total_opts++;
			continue;
		}
		total_args++;
	}

	argsOpts[0] = total_args;
	argsOpts[1] = total_opts;

	return argsOpts;
}

int main(int argcount, char **args)
{
	//printf("Time converter\n");
	printf("\n");

	// Hold total number of arguments vs options passed in //
	int total_args = 0;
	int total_opts = 0;
	int out_format = 24;

	// Get total amount of arguments vs options from parameters //
	total_args = get_total_number_args_options(argcount, args)[0];
	total_opts = get_total_number_args_options(argcount, args)[1];

	// Hold separate arrays of arguments and options //
	char *arguments[total_args];
	char *options[total_opts];

	// Reuse counters to save memory //
	int argsi = 0;
	int optsi = 0;

	// Split arguments and options to their own arrays //
	for (int i=1;i<argcount;i++) {
		// Assign options //
		if (args[i][0] == '-') {
			options[optsi] = args[i];
			optsi++;
			continue;
		}
		// Assign arguments //
		arguments[argsi] = args[i];
		argsi++;
	}

	// Modify Program Behavior with Option Flags //
	for (int i=0;i<optsi;i++) {
		// Display Help //
		if (strcmp(options[i], "-h") == 0 || strcmp(options[i], "--help") == 0) {
			print_help();
			return 0;
		}
		// Set 12-hour Output Formatting //
		if (strcmp(options[i], "-f12") == 0) {
			out_format = 12;
		}
	}

	// Convert Time if Args Supplied //
	if (argsi > 0) {
		if (out_format == 12) {
			printf("Formatting as 12 hour time...\n");
			//printf char* output = clock24_to_clock12(arguments);
		}
		hours_minutes_to_float(total_args, arguments, out_format);
	}

	// Default to Print Help if No Args/Opts //
	if (argsi == 0 && optsi == 0)
			print_help();

	printf("\n");

	return 0;
}
