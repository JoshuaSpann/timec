import sys

class Config:
    version = '2.0.0'
    command_info = f'''
timec: The Simple Time Converter v{version} - Joshua Spann
  Usage:
    timec [options...] [<time>...]
  Options:
    -d <times...> | -diff <times...>: Get the difference between all supplied times
    -f <format> <a> | -format <a> : Display the output in a specific format ("d"/decimal by default, "12" for 12-hour)
	-h | --help : Display this help message
	-v | --version : Display the current version number
    '''

class Utils:
    def padNumberLeadingZeroes(self, val):
        if val < 10 and val > 0:
            return f'0{val}'
        if val > -10 and val < 0:
            return f'0{(-val)}'
        if val is 0:
            return '00'
        return f'{val}'

class Parameters:
    def getArguments(self, args):
        options = self.getOptionNamesAsStringArray(args)
        optionvals = self.getOptionValuesAsStringArray(args)
        arguments = []

        for arg in args:
            if args.index(arg) is 0:
                continue
            
            cleanarg = arg
            if arg.startswith('--'):
                cleanarg = arg[2:]
            if arg.startswith('-'):
                cleanarg = arg[1:]
            if cleanarg not in options and cleanarg not in optionvals:
                arguments.append(cleanarg)

        return arguments
    
    def getOptions(self,args):
        options = []

        for arg in args:

            if args.index(arg) is 0:
                continue

            option = {}
            clean_option_name = arg
            isFlag = arg.startswith('-')
            isOption = arg.startswith('--')

            if isOption:
                clean_option_name = arg[2:]
            elif isFlag:
                clean_option_name = arg[1:]

                if clean_option_name == arg:
                    continue

                next_arg = args[ args.index(arg) +1 ]
                option = {'option': clean_option_name, 'value': None}
                optionUsesValue = True

                if clean_option_name == 'd' or clean_option_name == 'diff':
                    optionUsesValue = False

                if optionUsesValue and not next_arg.startswith('-'):
                    option['value'] = next_arg

                options.append(option)

        return options

    def getOptionNamesAsStringArray(self, args):
        option_params = self.getOptions(args)
        options = []

        for opt in option_params:
            options.append(opt['option'])

        return options
    
    def getOptionValuesAsStringArray(self, args):
        option_params = self.getOptions(args)
        options = []

        for opt in option_params:
            options.append(opt['value'])

        return options

class Timec:
    def formatAs12HourTime(self, arg):
        mainTimeUnit = int(arg.split(':')[0])
        subTimeUnit = int(arg.split(':')[1])
        apm = 'am'

        if mainTimeUnit > 23 or mainTimeUnit < 0:
            raise ValueError(f'Main time unit of "{mainTimeUnit}:##" must be in a range of 0-23')
        if subTimeUnit > 59 or subTimeUnit < 0:
            raise ValueError(f'Sub time unit of "##:{subTimeUnit}" must be in a range of 0-59')
        
        if mainTimeUnit >= 12:
            apm = 'pm'
        if mainTimeUnit > 12:
            mainTimeUnit = mainTimeUnit - 12
        if mainTimeUnit == 0:
            mainTimeUnit = 12
        
        return f'{Utils().padNumberLeadingZeroes(mainTimeUnit)}:{Utils().padNumberLeadingZeroes(subTimeUnit)} {apm}'
    
    def getAs12HourTime(self, args):
        print('Formatting as 12-hour time...')
        output = []

        for arg in args:
            output.append(f'{arg} = {self.formatAs12HourTime(arg)}')
        
        return output

    def getAsDecimalTime(self, args):
        output = []

        for arg in args:
            mainTime = int(arg.split(':')[0])
            subTime = int(arg.split(':')[1])
            decimalTime = mainTime + subTime/60
            output.append(f'{arg} = {decimalTime}')
        
        return output
    
    def getTimeDiffs(self, args):
        if len(args) <= 1:
            raise Exception('Not enough arguments to perfrom subtraction')
        print('Subtracting times...')

        output = []
        mainTime = int(args[0].split(':')[0])
        subTime = int(args[0].split(':')[1])
        equationLine = args[0]

        for arg in args:
            if args.index(arg) is 0:
                continue
            
            equationLine += f' - {arg}'

            mainTimeUnit = int(arg.split(':')[0])
            subTimeUnit = int(arg.split(':')[1])

            mainTime -= mainTimeUnit
            subTime -= subTimeUnit

            if subTime < 0:
                mainTime -= 1
                subTime = 60 + subTime
        
        equationLine += f' = {Utils().padNumberLeadingZeroes(mainTime)}:{Utils().padNumberLeadingZeroes(subTime)}'
        output.append(equationLine)

        return output
    
    def verifyArguments(self, arguments):
        for arg in arguments:
            
            argHasLetters = True
            if arg.upper() == arg and arg.lower() == arg:
                argHasLetters = False

            if len(arg) != 5 or arg[2] != ':' or argHasLetters is True:
                raise ValueError(f'"{arg}" needs to be in ##:## format')
    
    def verifyOptions(self, options):
        for opt in options:
            option = opt['option']
            value = opt['value']

            if option is 'd' or option is 'diff':
                continue
            elif option is 'f' or option is 'format':
                if value != '12' and value != 'd' and value != 'decimal':
                    raise ValueError(f'Invalid formatting "{value}"')
            elif option is 'h' or option is 'help':
                print(Config().command_info)
            elif option is 'v' or option is 'version':
                print('timec v{}', Config().version)
            else:
                raise ValueError(f'Invalid option "{option}"')
    
    def main(self):
        options = Parameters().getOptions(sys.argv)
        self.verifyOptions(options)
        arguments = Parameters().getArguments(sys.argv)
        self.verifyArguments(arguments)

        print('')

        output = []
        isOption = (len(options) > 0)

        if isOption:
            for opt in options:
                if (opt['option'] is 'format' or opt['option'] is 'f') and opt['value'] == '12':
                    output = self.getAs12HourTime(arguments)
                    break
                elif opt['option'] is 'd' or opt['option'] is 'diff':
                    output = self.getTimeDiffs(arguments)
                    break
                else:
                    output = self.getAsDecimalTime(arguments)
                    break
        else:
            output = self.getAsDecimalTime(arguments)

        for line in output:
            print(line)

        print('')

Timec().main()