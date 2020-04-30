/***************************************************************************
 *============= Copyright by Darmstadt University of Applied Sciences =======
 ****************************************************************************
 * Filename        : CSTRING.H
 * Author          : George Sebastian
 * Description     : CSTRING provides some additional string operations in addition to C++ string operations
 *                   for the ease of the end user
 *                   Provides APIs for converting string to double, double to string, and for
 *                   checking if a string contains only alphabets, if string represents a
 *                   valid number etc
 ****************************************************************************/
#ifndef CSTRING_H
#define CSTRING_H

#include <string>
#include <sstream>

class CString
{

public:

	/**
	 * Function converts a number in string format into a valid double number
	 * @param const string& str : IN number in string format to be converted
	 * @returnvalue double, valid number corresponding to the string
	 */
	double stringToDouble(const std::string& str);

	/**
	 * Function converts a number of type double into a string
	 * @param const double& number : IN number to be converted
	 * @returnvalue string, corresponding to the number
	 */
	std::string doubleToString(const double& number);

	/**
	 * Function checks if a string contains only Alphabets and whitespaces. No other characters allowed
	 * @param const std::string& str    : IN string which should be checked if it has only alphabets and white spaces
	 * @returnvalue true, in case string has only alphabets or white spaces
	 */
	bool isOnlyAplhabets(const std::string& str);

	/**
	 * Function checks if a string represents a valid number
	 * String represents a valid number if the string contains only digits,at the max single "." and at the
	 * max single "-" indicating decimal and negative numbers. If negative sign present, it should be present
	 * at the start of the string. The string should not be blank. The string should not contain any other special characters.
	 * @param const std::string& str    : IN string which should be checked if it's a valid number or not
	 * @returnvalue true, in case string is a valid number
	 */
	bool isStringValidNumber(const std::string& str);

	/**
	 * Function extracts substring from a source, until the first encountered ";" or "," (CSV format)
	 * @param string& source    : OUT string following the first encountered ";" or ","
	 * @param string& subString : OUT substring from a source, until the first encountered ";" or ","
	 * @returnvalue true, in case a ";" or "," is encountered in the source
	 */
	bool cutLeft(std::string& source, std::string& subString);

};

/********************
**  CLASS END
*********************/
#endif /* CSTRING_H */
