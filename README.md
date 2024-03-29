# timec: The Time Converter

`timec` is a simple time conversion utility that converts time formatting.

---

## Use
### Arguments
Any number of arguments passed in will be interpreted as mm:ss times.
timec will then convert the times to the total time in minutes:
```shell
$ timec 12:45 8:08 256:32

12:45 = 12.750000 minutes
8:08: = 82.000000 minutes
256:32 = 256.533325 minutes
```

### Options
There are a few options that you can use to change `timec`'s behavior.
Using options is started by a `-` or `--` character then the option.

## Development Roadmap
There is a list of planned options to be added:

- [x] `-h` : Displays help information
- [ ] `-v` : Displays version info
- [ ] `-f` : Sets the formatting as stopwatch, 24-hour, or 12-hour time formats
  - [x] `-f12` sets formatting as 12-hour
  - [ ] `-fd` : formats time as decimal
- [x] `-d` : Calculates difference between two times
  - [x] `-d` with `-fd` set : Provides difference in decimal format (mm.%ss)
