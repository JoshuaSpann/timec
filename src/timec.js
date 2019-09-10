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
	let timecResultsArr = []
	for (let args_i=0; args_i<args.length; args_i++) {
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
		let key = arr[arr_i].key
		let value = arr[arr_i].value


		let delimiter = '\n'
		if (options.asHtml === true) delimiter = '<br>'
		outputString += `${key} = ${value}${delimiter}`
	}
	outputString += '\n'
	return outputString
}
