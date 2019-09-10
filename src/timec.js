let options = {
	asHtml: false,
	as12Hour: false,
	asDecimal: true,
	subtract: false
}
function timec(arg) {
	if (Array.isArray(arg) === true) return formatResults(timecFromArray(arg))
	return formatResults(timecFromArray([arg]))
}

function timecFromValue(arg) {
	if (arg.toString().length === 4 && Number.isInteger(parseInt(arg)))
		return (timecFromInt(arg))
	if (arg.length === 5 && arg[2] === ':')
		return (timecFromString(arg))
}
function timecFromArray(args) {
/*
	let timecResults = {}
	for (let args_i in args) {
		let arg = args[args_i]
		timecResults[arg] = timecFromValue(arg)
	}
	return timecResults
*/
	let timecResultsArr = []
	for (let args_i=0; args_i<args.length; args_i++) {
		let key = Object.keys(args)[args_i]
		timecResultsArr.push({key: args[args_i], value: timecFromValue(args[args_i])})
	}
	return timecResultsArr
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
		let key = arr_i
		let value = arr[arr_i]

		key = Object.keys(arr)[arr_i]
		key = arr[arr_i].key
		value = arr[arr_i].value

		let delimiter = '\n'
		if (options.asHtml === true) delimiter = '<br>'
		outputString += `${key} = ${value}${delimiter}`
	}
	outputString += '\n'
	return outputString
}
		

//console.log(timec([1230, '13:15', '1246']))
//console.log(timec(1367))
