/***************************************************************************
 *============= Copyright by Darmstadt University of Applied Sciences =======
 ****************************************************************************
 * Filename        : CSTRING.CPP
 * Author          : George Sebastian
 * Description     : CSTRING provides some additional string operations in addition to C++ string operations
 *                   for the ease of the end user
 *                   Provides APIs for converting string to double, double to string, and for
 *                   checking if a string contains only alphabets, if string represents a
 *                   valid number etc
 ****************************************************************************/
using namespace std;

#include "CString.h"

/**
 * Function converts a number in string format into a valid double number
 * @param const string& str : IN number in string format to be converted
 * @returnvalue double, valid number corresponding to the string
 */
double CString::stringToDouble(const string& str)
{
	double number;

	std::stringstream iostr;
	iostr << str;
	iostr >> number;

	return number;
}

/**
 * Function converts a number of type double into a string
 * @param const double& number : IN number to be converted
 * @returnvalue string, corresponding to the number
 */
string CString::doubleToString(const double& number)
{
	std::string str;

	std::stringstream iostr;
	iostr << number;
	iostr >> str;

	return str;
}

/**
 * Function checks if a string contains only Alphabets and whitespaces. No other characters allowed
 * @param const std::string& str    : IN string which should be checked if it has only alphabets and white spaces
 * @returnvalue true, in case string has only alphabets or white spaces
 */
bool CString::isOnlyAplhabets(const std::string& str)
{

	return (str.find_first_not_of(
			"abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ ") // string should contain only alphabets and whitespaces
	== string::npos && str != "");            // also string should not be blank

}

/**
 * Function checks if a string represents a valid number
 * String represents a valid number if the string contains only digits,at the max single "." and at the
 * max single "-" indicating decimal and negative numbers. If negative sign present, it should be present
 * at the start of the string. The string should not be blank. The string should not contain any other special characters.
 * @param const std::string& str    : IN string which should be checked if it's a valid number or not
 * @returnvalue true, in case string is a valid number
 */
bool CString::isStringValidNumber(const std::string& str)
{
	const string s = "-";

	if (str.find("-") != string::npos)
	// checks if string contains a negative sign
	{
		if (str.find_first_of("-") != s.find("-"))
			// checks if negative sign is at the start of the string, else error
			return false;
	}

	return ((str.find_first_not_of("0123456789.-") == string::npos)
			&& (str.find_first_of(".") == str.find_last_of("."))
			&& (str.find_first_of("-") == str.find_last_of("-")) && str != "");
}

/**
 * Function extracts substring from a source, until the first encountered ";" or "," (CSV format)
 * @param string& source    : OUT string following the first encountered ";" or ","
 * @param string& subString : OUT substring from a source, until the first encountered ";" or ","
 * @returnvalue true, in case a ";" or "," is encountered in the source
 */
bool CString::cutLeft(string& source, string& subString)
{
    int pos = source.find_first_of(";,");

	if (pos != std::string::npos)
	// 	";" or "," is encountered in the source
	{
		subString = source.substr(0, pos); // substring from a source, until the first encountered ";" or ","
		source.erase(0, pos + 1); // string following the first encountered ";" or ","

		return true;
	}
	else
	// ";" or "," is not encountered in the source
	{
		subString = source;

		return false;
	}
}
