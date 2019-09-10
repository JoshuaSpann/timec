let options = {
	asHtml: false,
	as12Hour: false,
	asDecimal: true,
	subtract: false
}
function timec(arg) {
	if (arg === undefined) {
		try{
			arg = process.argv.splice(2)
			arg = setOptions(arg)
		}catch (err){}
	}
	if (Array.isArray(arg) === true) return formatResults(timecFromArray(arg))
	return formatResults(timecFromArray([arg]))
}
function setOptions(args) {
	let arg = []
	for (let args_i = 0; args_i < args.length; args_i++) {
		let currArg = args[args_i]
		if (['d','decimal','-d','-decimal'].includes(currArg)) {
			options.asDecimal = true
			continue
		}
		if (['s','sub','subtract','-s','-sub','-subtract'].includes(currArg)) {
			options.subtract = true
			continue
		}
		arg.push(currArg)
	}
	return arg
}

function timecFromValue(arg) {
	if (arg.toString().length === 4 && Number.isInteger(parseInt(arg)))
		return (timecFromInt(arg))
	if (arg.length === 5 && arg[2] === ':')
		return (timecFromString(arg))
}
function timecFromArray(args) {
	let timecResultsArr = []
	for (let args_i=0; args_i<args.length; args_i++) {
		let keyValue = args_i
		let timevalue = -1
		if (options.asDecimal === true && options.subtract === false) {
			let decimalTimeValue = timecFromValue(args[args_i])
			keyValue = args[args_i]
			timevalue = decimalTimeValue
		}
		if (options.subtract === true && (args_i > 0) && ((args_i+1) % 2 === 0) ) {
			let lastArg = args[args_i-1]
			let currentArg = args[args_i]

			keyValue = `${lastArg} - ${currentArg}`

			timevalue = subtractValues(lastArg, currentArg)
			if (options.asDecimal === true) timevalue = timecFromValue(timevalue)
		}
		if (timevalue === -1) continue
		timecResultsArr.push({key: keyValue, value: timevalue})
	}
	return timecResultsArr
}
function subtractValues(lastArg, currentArg) {
			let lastValue = {}
			lastValue.main = parseInt(lastArg.substr(0,1))
			lastValue.sub = parseInt(lastArg.substr(2,3))

			let currentValue = {}
			currentValue.main = parseInt(currentArg.substr(0,1))
			currentValue.sub = parseInt(currentArg.substr(2,3))

			let newMain = lastValue.main-currentValue.main
			let newSub = lastValue.sub-currentValue.sub

			if (newMain <= 0) newMain = '00:'
			else if (newMain < 10) newMain = `0${newMain}`

			if (newSub <= 0) newSub = '00'
			else if (newSub < 10) newSub = `0${newSub}`

			return `${newMain}${newSub}`
}
function timecFromInt(arg) {
	let strValue = arg.toString()
	let mainUnit = strValue.substr(0,2)
	let subUnit = strValue.substr(2,3)
	return timecMain(mainUnit, subUnit)
}
function timecFromString(arg) {
	let strValue = arg.toString()
	let mainUnit = strValue.substr(0,2)
	let subUnit = strValue.substr(3,4)
	return timecMain(mainUnit, subUnit)
}
function timecMain(mainUnit, subUnit) {
	mainUnit = parseInt(mainUnit)
	subUnit = parseInt(subUnit)

	if (options.asDecimal === true)
		return mainUnit + (subUnit / 60)

	return 'ERROR PARSING OPTIONS'
}
function formatResults(arr) {
	let outputString = '\n'
	for (let arr_i in arr) {
		let key = arr[arr_i].key
		let value = arr[arr_i].value


		let delimiter = '\n'
		if (options.asHtml === true) delimiter = '<br>'
		outputString += `${key} = ${value}${delimiter}`
	}
	outputString += '\n'
	return outputString
}

try {console.log(timec())}catch(err){}
