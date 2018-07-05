/**
 * timec: THE TIME CONVERTER
 *  author:   Joshua Spann
 *  date:     20180608:1035
 *  liscense: GPLv2
 **/
#include <stdio.h>
#include <inttypes.h>
#include <string.h>
#include <math.h>

#define FALSE 0
#define MAX_ARG_SIZE 6
# define TRUE 1
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

/**
 * Convert number_before_colon to 12-hour time and return output string
 **/
int clock24_to_clock12(int military_hour)
{
	static int standard_hour = 0;
	standard_hour = military_hour;
	if (military_hour > 12) standard_hour = military_hour - 12;

	return standard_hour;
}

/**
 * Converts 12+:yy time to 0-12:yy time
 **/
void print_time_as_12_hour(char* time, int minutes, int seconds)
{
	printf("%s = ", time);
	minutes = clock24_to_clock12(minutes);
	printf("%d:%d\n", minutes, seconds);
}

void print_time_as_float(char* time, float minutes, float seconds)
{
	float totaltime = 0;
	totaltime = minutes+(seconds/60);

	// Output converted, total value to user //
	printf("%s = ", time);
	printf("%f minutes\n", totaltime);
}

/**
 * Convert xx:yy to float value of x (converts y to x's)
 **/
void print_times(int argcount, char **args, int hour_format)
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

		minutes = numbers_from_argument(args[i])[0];
		seconds = numbers_from_argument(args[i])[1];

		if (hour_format == 12) {
			print_time_as_12_hour(args[i], minutes, seconds);
			continue;
		}

		print_time_as_float(args[i], minutes, seconds);
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
					"  -h | --help    Display this help message\n"
					"  -f12           Format as 12-hour time\n"
					"  -fd            Format time in decimal format\n"
					"  -d <t1> <t2>   Calculate difference between t1 and t2 (t1-t2)\n"
					"\n";
	printf("%s", help);
}

/**
 * Converts mm:ss to mm.%ss and subtracts the second time given from the first
 **/
void print_time_difference(int args_count, char **args, int format_decimal)
{
	// TODO?: Merge this functionality with print_times to new get_minutes_and_seconds() that returns minutes as int and seconds as int
	char minutesch[10],	secondsch[10];
	float minutes = 0, firstminutes = 0, secondminutes = 0;
	float seconds = 0, firstseconds = 0, secondseconds = 0;
	float time = 0;

	printf("Calculating time difference...\n");

	// Get first time values //
	firstminutes = numbers_from_argument(args[0])[0];
	firstseconds = numbers_from_argument(args[0])[1];

	// Get second time values //
	secondminutes = numbers_from_argument(args[1])[0];
	secondseconds = numbers_from_argument(args[1])[1];

	// Get time difference in decimat (mm.%ss) format //
	if (format_decimal == TRUE) {
		float firsttime = 0, secondtime = 0;
		firsttime = firstminutes + (firstseconds/60);
		secondtime = secondminutes + (secondseconds/60);

		time = firsttime - secondtime;
		printf("%s-%s = %f minutes\n", args[0], args[1], fabs(time));
		return;
	}

	// Default to printing difference in time (mm:ss) format //
	minutes = firstminutes - secondminutes;
	seconds = firstseconds - secondseconds;

	printf("%s-%s = %0.0f:%0.0f\n", args[0], args[1], fabs(minutes), fabs(seconds));
}

/**
 * Acts like sizeof() but for the size of a pointer array with a safe limit
 **/
int get_pointer_array_element_size(char *arg)
{
	int maximum_scan_size = 10;

	for (int arg_i=0; arg_i < maximum_scan_size; arg_i++) {
		// Break when end of string //
		if (arg[arg_i] == '\0') {
			// Include the end of the string like sizeof() //
			return arg_i+1;
		}
	}
	return maximum_scan_size;
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

/**
 * Main program code
 **/
void run_program(int args_count, int opts_count, char **arguments, char **options)
{
	int out_format = 24;
	int format_decimal = FALSE;
	int get_time_diff = FALSE;
	// Modify Program Behavior with Option Flags //
	for (int i=0;i<opts_count;i++) {
		// Display Help //
		if (strcmp(options[i], "-h") == 0 || strcmp(options[i], "--help") == 0) {
			print_help();
			return;
		}
		// Format output in decimal (mm.%ss) format //
		if (strcmp(options[i], "-fd") == 0) format_decimal = TRUE;
		// Set 12-hour Output Formatting //
		if (strcmp(options[i], "-f12") == 0) out_format = 12;
		// Perform Difference between 2 times //
		if (strcmp(options[i], "-d") == 0) get_time_diff = TRUE;
	}

	// Convert Time if Args Supplied //
	if (args_count > 0) {
		if (out_format == 12) {
			printf("Formatting as 12 hour time...\n");
		}
		if (get_time_diff == TRUE) {
			print_time_difference(args_count, arguments, format_decimal);
			return;
		}
		print_times(args_count, arguments, out_format);
	}

	// Default to Print Help if No Args/Opts //
	if (args_count == 0 && opts_count == 0) print_help();
}

int main(int argcount, char **args)
{
	//printf("Time converter\n");
	printf("\n");

	// Catches Generic Errors //
	int error = 0;

	// Hold total number of arguments vs options passed in //
	int total_args = 0;
	int total_opts = 0;

	// Get total amount of arguments vs options from parameters //
	total_args = get_total_number_args_options(argcount, args)[0];
	total_opts = get_total_number_args_options(argcount, args)[1];

	// Hold separate arrays of arguments and options //
	char *arguments[total_args];
	char *options[total_opts];

	// Counters to properly store disordered arg/opt strings in array //
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

		// Firewall to prevent anything but 0-9 and : as an argument input //
		for (int char_i=0; char_i < MAX_ARG_SIZE; char_i++) {
			char c = 'a';
			c = args[i][char_i];

			// Prevent from going past argument end in memory //
			if (c == '\0') break;

			// Will throw an error later if anything below '0' or above ':' //
			if (c < 30 || c > 58) {
				printf("Incorrect argument format: An argument must include numbers 0-9 and a ':'\nExiting...\n");
				error = TRUE;
				break;
			}
		}

		if (error) break;

		// Firewall to ensure args are of the correct length //
		if (get_pointer_array_element_size(args[i]) > MAX_ARG_SIZE) {
			printf("Incorrect size of argument: An arg must be %d characters long.\nExiting...\n", MAX_ARG_SIZE-1);
			error = TRUE;
		}

		// Assign arguments //
		arguments[argsi] = args[i];
		argsi++;
	}

	// Only Proceed with Program if no Errors //
	if (!error) {
		run_program(argsi, optsi, arguments, options);
	}

	printf("\n");

	return 0;
}
