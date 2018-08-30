class Config {
	public static var version = '2.0.0';
	public static var command_info = '
timec: The Simple Time Converter v$version - Joshua Spann
  Usage:
    timec [options...] [<time>...]
  Options:
    -d <times...> | -diff <times...>: Get the difference between all supplied times
    -f <format> <a> | -format <a> : Display the output in a specific format ("d"/decimal by default, "12" for 12-hour)
	-h | --help : Display this help message
	-v | --version : Display the current version number
';
}

class Utils {
	/**
	 * Returns given number with padding 0s if a single-digit number
	 * @param val 
	 * @returns modified leading-zero val
	 */
	public static function padNumberLeadingZeroes(val :Int) {
		if (val < 10 && val > 0) return '0$val';
		if (val > -10 && val < 0) return '0${-val}';
		if (val == 0) return '00';
		return '$val';
	}
}

class Parameters {
	/**
	 * Parses and returns Array<String> of all arguments that are not options, flags, or midifiers to the opts/flags
	 * @param args
	 * @returns arguments
	 */
	public static function getArguments(args :Array<String>) {
		var options = getOptionNamesAsStringArray(args);
		var optionvals = getOptionValuesAsStringArray(args);
		var arguments: Array<Dynamic> = [];

		for (arg in args) {
			var cleanarg = arg;
			if (StringTools.startsWith(arg, '--')) cleanarg = arg.substr(2);
			if (StringTools.startsWith(arg, '-')) cleanarg = arg.substr(1);
			if (options.indexOf(cleanarg) < 0 && optionvals.indexOf(cleanarg) < 0) arguments.push(cleanarg);
		}

		return arguments;
	}

	/**
	 * Pulls tags '-' and options '--' from all passed command line args.
	 * Assigns next given value to the option if it is not a flag or option.
	 * 
	 * @param args 
	 * @returns options{option: <option name>, value: <option value>|null}
	 */
	public static function getOptions(args :Array<String>) {
		var options: Array<Dynamic> = [];

		for (arg in args) {

			var option = {};
			var clean_option_name = arg;
			var isFlag = StringTools.startsWith(arg, '-');
			var isOption = StringTools.startsWith(arg, '--');

			if (isOption) {
				clean_option_name = arg.substr(2);
			} else if (isFlag) {
				clean_option_name = arg.substr(1);
			}

			if (clean_option_name == arg) continue;

			var next_arg = args[ args.indexOf(arg) +1 ];
			var option = {option: clean_option_name, value: null};
			var optionUsesValue = true;

			if (clean_option_name == 'd' || clean_option_name == 'diff') {
				optionUsesValue = false;
			}

			if (optionUsesValue && !StringTools.startsWith(next_arg, '-')) {
				option.value = next_arg;
			}

			options.push(option);
		}

		return options;
	}

	/**
	 * Returns string array of options/flags without values
	 * @param args 
	 * @returns option names
	 */
	static function getOptionNamesAsStringArray(args: Array<String>) {
		var option_params = getOptions(args);
		var options: Array<String> = [];

		for (opt in option_params) {
			options.push(opt.option);
		}

		return options;
	}

	/**
	 * Returns string array of option/flag values, not the opts/flags themselves
	 * @param args 
	 * @returns option values
	 */
	static function getOptionValuesAsStringArray(args: Array<String>) {
		var option_params = getOptions(args);
		var options: Array<String> = [];

		for (opt in option_params) {
			options.push(opt.value);
		}

		return options;
	}

}

class Timec {
	/**
	 * Returns output string of given time in 12-hour, am/pm format
	 * @param arg 
	 * @returns am/pm output string
	 */
	static function formatAs12HourTime(arg :String) {
		var mainTimeUnit = Std.parseInt(arg.split(':')[0]);
		var subTimeUnit = Std.parseInt(arg.split(':')[1]);
		var apm = 'am';

		if (mainTimeUnit > 23 || mainTimeUnit < 0) throw 'Main time unit of "$mainTimeUnit:##" must be in a range of 0-23';
		if (subTimeUnit > 59 || subTimeUnit < 0) throw 'Sub time unit of "##:$subTimeUnit" must be in a range of 0-59';

		if (mainTimeUnit >= 12) apm ='pm';
		if (mainTimeUnit > 12) mainTimeUnit = mainTimeUnit - 12;
		if (mainTimeUnit == 0) mainTimeUnit = 12;
		
		return '${Utils.padNumberLeadingZeroes(mainTimeUnit)}:${Utils.padNumberLeadingZeroes(subTimeUnit)} $apm';
	}

	/**
	 * Returns array of output strings of each supplied time in am/pm, 12-hour format
	 * @param args 
	 * @returns am/pm output string array
	 */
	static function getAs12HourTime(args :Array<Dynamic>) {
		Sys.println('Formatting as 12-hour time...');
		var output :Array<String> = [];

		for (arg in args) {
			output.push('\t${arg} = ${formatAs12HourTime(arg)}');
		}

		return output;
	}

	/**
	 * Returns an array of output strings of each supplied time in decimal format
	 * @param args 
	 * @returns decimal-time string output array
	 */
	static function getAsDecimalTime(args :Array<Dynamic>) {
		Sys.println('Formatting as decimal time...');
		var output :Array<String> = [];

		for (arg in args) {
			var mainTime = Std.parseInt(arg.split(':')[0]);
			var subTime = Std.parseInt(arg.split(':')[1]);
			var decimalTime = mainTime + subTime/60;
			//output.push('\t$arg = ${Utils.padNumberLeadingZeroes(decimalTime)}');
			output.push('\t$arg = ${decimalTime}');
		}

		return output;
	}

	/**
	 * Returns calculated distance between all given times in regular, unformatted time
	 * @param args 
	 * @returns time-difference string output array
	 */
	static function getTimeDiffs(args :Array<Dynamic>) {
		if (args.length <= 1) throw 'Not enough arguments to perfrom subtraction';
		Sys.println('Subtracting times...');

		var output :Array<String> = [];
		var mainTime = Std.parseInt(args[0].split(':')[0]);
		var subTime = Std.parseInt(args[0].split(':')[1]);
		//var time = mainTime + subTime;
		var equationLine = '\t${args[0]}';

		for (arg in args) {
			if (args.indexOf(arg) == 0) continue;

			equationLine += ' - $arg';

			var mainTimeUnit = Std.parseInt(arg.split(':')[0]);
			var subTimeUnit = Std.parseInt(arg.split(':')[1]);

			mainTime -= mainTimeUnit;
			subTime -= subTimeUnit;

			if (subTime < 0) {
					mainTime--;
					subTime = 60 + subTime;
			}
		}

		equationLine += ' = ${Utils.padNumberLeadingZeroes(mainTime)}:${Utils.padNumberLeadingZeroes(subTime)}';
		output.push(equationLine);

		return output;
	}

	/**
	 * Checks that all arguments are ##:## format, throw an error if otherwise
	 * @param arguments 
	 * @returns void
	 */
	static function verifyArguments(arguments :Array<Dynamic>) {
		for (arg in arguments) {
			var argHasLetters = true;
			if (arg.toUpperCase() == arg && arg.toLowerCase() == arg) argHasLetters = false;
			if (arg.length != 5 || arg.charAt(2) != ':' || argHasLetters) throw '"$arg" needs to be in ##:## format';
		}
	}

	/**
	 * Checks that all options are within defined boundaries, throw an error if otherwise
	 * @param options 
	 * @returns void
	 */
	static function verifyOptions(options :Array<Dynamic>) {
		for (opt in options) {
			var option = opt.option;
			var value = opt.value;

			//TODO - Have multiple single flags scrambled to allow multitasking
			if (option == 'd' || option == 'diff') {
				//Sys.println('Calculating time difference');
				continue;
			}
			else if (option == 'f' || option == 'format') {
				if (value != '12' && value != 'd' && value != 'decimal') throw 'Invalid formatting "${value}"';
			}
			else if (option == 'h' || option == 'help') {
				Sys.println(Config.command_info);
			}
			else if (option == 'v' || option == 'version') {
				Sys.println('timec v${Config.version}');
			}
			else throw 'Invalid option "${option}"';
		}
	}


	/***************************************************/
	static function main() {
		var options = Parameters.getOptions(Sys.args());
		verifyOptions(options);
		var arguments = Parameters.getArguments(Sys.args());
		verifyArguments(arguments);

		Sys.println('');

		var output :Array<Dynamic> = [];
		var isOption = (options.length > 0);

		if (isOption) {
			//TODO - Have ability to mix option flags
			for (opt in options) {
				if ((opt.option == 'format' || opt.option == 'f') && opt.value == '12') {
					output = getAs12HourTime(arguments);
					break;
				}
				//TODO - else if ((opt.option == 'format' || opt.option == 'f') && opt.value == '12d') output = getAs12HourTime(arguments);
				else if (opt.option == 'd' || opt.option == 'diff') {
					output = getTimeDiffs(arguments);
					break;
				}
				else {
					output = getAsDecimalTime(arguments);
					break;
				}
			}
		}
		else {
			output = getAsDecimalTime(arguments);
		}

		for (line in output) Sys.println(line);

		Sys.println('');
	}
}
