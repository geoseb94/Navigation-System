/***************************************************************************
 *============= Copyright by Darmstadt University of Applied Sciences =======
 ****************************************************************************
 * Filename        : CCSV.CPP
 * Author          : George Sebastian
 * Description     : CCSV is derived from the Abstract CPersistentStroage class.
 *                   CCSV class provides access to persistent storage.
 *                   Two files are maintained for POI Database and WP Database
 *                   in CSV format. This class provides functions for reading from and
 *                   writing to these files. Important thing : separator should not
 *                   be a part of the attributes of POI and WP
 ****************************************************************************/
#include <iostream>
#include <vector>

using namespace std;

#include "CFileIO.h"
#include "CCSV.h"
#include "CString.h"

#define NUMBER_OF_WP_ATTRIBUTES 3  // number of WP attributes for a single WP object
#define NUMBER_OF_POI_ATTRIBUTES 5 // number of POI attributes for a single POI object

#define ERROR_MISSING_ATTRIBUTES 0    // error handling : some attributes of WP/POI are missing
#define ERROR_UNKNOWN_ATTRIBUTES 1    // error handling : some attributes of WP/POI are unknown
#define ERROR_INVALID_NAME 2          // error handling : invalid name of POI/Wp
#define ERROR_CORRUPTED_DATA 3        // error handling : corrupted longitude or latitude data in string form
#define ERROR_INVALID_RANGE 4         // error handling : longitude or latitude not in valid range
#define ERROR_INVALID_POI_TYPE 5      // error handling : invalid type of POI

namespace waypoint
{
typedef enum waypointAttributes
{
	NAME, LATITUDE, LONGITUDE
} t_waypointAttributes;
}

namespace POI
{
typedef enum poiAttributes
{
	TYPE, NAME, DESCRIPTION, LATITUDE, LONGITUDE
} t_poiAttributes;
}

/**
 * Function returns the type of POI based on the input string
 * @param const string& type : IN type of POI in string
 * @returnvalue t_poi type of POI
 */
CPOI::t_poi stringToPoiType(const string& type);

/**
 * Function sets the name of Media
 * @paramconst string& name : name of the Media
 * @returnvalue void
 */
void CCSV::setMediaName(const string& name = "csv")
{
	m_mediaName = name;
}

/**
 * Function writes POI Database and WP Database data to our persistent storage in CSV format
 * @param const CWpDatabase& waypointDb     : IN database with Waypoints
 * @param const CPoiDatabase& poiDb         : IN database with POIs
 * @returnvalue true is data could be saved successfully
 */
bool CCSV::writeData(const CWpDatabase& waypointDb, const CPoiDatabase& poiDb)
{
	CFileIO myFile;
	string line;
	CString myString;

	/*======================================================================
	 * 	Writing Waypoints to Persistent storage in CSV format
	 *=======================================================================
	 */
	map<string, CWaypoint> wpMap;
	waypointDb.getWpMap(wpMap); // get pointer to the Map of Waypoints in WpDatabase

	myFile.openFile("csv_wp.txt", CFileIO::OUT); // opening csv-wp.txt file in OUT mode

	for (map<string, CWaypoint>::const_iterator itr = wpMap.begin(); // iterating through the Map of Waypoints in WpDatabase
	itr != wpMap.end(); ++itr)
	{
		line = (itr->second).getName()
				+ ";" // All Waypoint attributes of a WP to be written to persistent storage are stored in a single line in CSV format
				+ myString.doubleToString((itr->second).getLatitude()) + ";"
				+ myString.doubleToString((itr->second).getLongitude());

		myFile.writeLineToFile(line); // Writing line by line to the persistent storage in CSV format
	}

	myFile.closeFile();                           // closing the csv-wp.txt file

	/*======================================================================
	 * 	Writing POIs to Persistent storage in CSV format
	 *=======================================================================
	 */
	map<string, CPOI> poiMap; // get pointer to the Map of POIs in PoiDatabase
	poiDb.getPoiMap(poiMap);

	myFile.openFile("csv_poi.txt", CFileIO::OUT); // opening csv-poi.txt file in OUT mode

	for (map<string, CPOI>::const_iterator itr = poiMap.begin(); // iterating through the Map of POIs in PoiDatabase
	itr != poiMap.end(); ++itr)
	{
		line = (itr->second).getType() + ";"
				+ (itr->second).getName() // All POI attributes of a POI to be written to persistent storage are stored in a single line in CSV format
				+ ";" + (itr->second).getPoiDescription() + ";"
				+ myString.doubleToString((itr->second).getLatitude()) + ";"
				+ myString.doubleToString((itr->second).getLongitude());

		myFile.writeLineToFile(line); // Writing line by line to the persistent storage in CSV format
	}

	myFile.closeFile();                          // closing the csv-poi.txt file
	return true;
}


/**
 * Function reads from persistent storage. If Merge mode is MERGE, content of Persistent
 * storage will be merged with already existing data in WP Database and POI Database.
 * If merge mode is REPLACE, the content of persistent storage will completely replace
 * the content of WP Database and POI Database.
 * @param CWpDatabase& waypointDb    : OUT database with Waypoints
 * @param CPoiDatabase&  poiDb       : OUT database with POIs
 * @param CCSV::MergeMode            : IN  merge mode (MERGE/REPLACE)
 * @returnvalue true is data could be read successfully
 */
bool CCSV::readData(CWpDatabase& waypointDb, CPoiDatabase& poiDb,
		CCSV::MergeMode mode)
{
	string name, description, type;
	double longitude, latitude;
	vector<string> lineVector;

	if (mode == REPLACE)
	// if Mode is replace, then the content of persistent storage will completely replace the content of WP Database and POI Database
	{
		waypointDb.clearWpDb();
		poiDb.clearPoiDb();
	}
	/*==========================================================
	 * 	Reading and processing Waypoints from Persistent storage
	 *==========================================================
	 */
	getEntireLinesFromFile("csv_wp.txt", lineVector); // read entire lines from wp.txt and store the contents to lineVector

	for (unsigned short int lineNumber = 0; lineNumber < lineVector.size(); // process each line
			lineNumber++)
	{
		try
		{
			validateEachWpLine(name, latitude, longitude, // validate each lines read from the Wp file, in case of error, exception is thrown
					lineVector[lineNumber]);
			CWaypoint wp(name, latitude, longitude); // if it is a valid line, add wp to the WpDatabase
			waypointDb.addWaypoint(wp);
		} catch (int error)                           // catch exceptions thrown
		{
			errorHandler(error, lineNumber, lineVector[lineNumber]); // handle the exception thrown
		}
	}

	/*======================================================
	 * 	Reading and processing POIs from Persistent storage
	 *======================================================
	 */
	getEntireLinesFromFile("csv_poi.txt", lineVector); // read entire lines from poi.txt and store the contents to lineVector

	for (unsigned short int lineNumber = 0; lineNumber < lineVector.size(); // process each line
			lineNumber++)
	{
		try
		{
			validateEachPoiLine(name, latitude, longitude, type, description, // validate each lines read from the poi file, in case of error, exception is thrown
					lineVector[lineNumber]);
			CPOI poi(stringToPoiType(type), name, description, latitude,
					longitude);
			poiDb.addPoi(poi); // if it is a valid line, add poi to the poiDatabase
		} catch (int error)                       // catch exceptions thrown
		{
			errorHandler(error, lineNumber, lineVector[lineNumber]); // handle the exception thrown
		}
	}
	return true;
}

/**
 * Function handles different possible error conditions while validating each POI or WP line
 * read from persistent storage
     **
	 * ============================================================
	 *  POSSIBLE ERRONEOUS LINES IN POI FILE : READ OPERATIONS
	 * ===========================================================
TOURISTIC;Brandenburg123 Gate;Touristic attraction in Berlin;52.51;13.77     // Invalid Poi name
TOURISTIC;Cologne Cathedral;Catholic Cathedral;50.9;6.98;                    // Unknown attributes
UNIVERSITY;HDA;best among Hochschules in Germany;49.86                       // Missing attributes
RESTAURANT;Heising;French Restaurant excellent service;53;11sd               // Corrupted longitude data
RESTAURANT;Mensa HDA;The best Mensa in the world;1--0;20                     // Corrupted latitude data
RESTAURANT;Morsh;French Restaurant;50.55;10.4.                               // Corrupted latitude data
RESTAURANT;Sitte;More expensive but also good;1000;22                        // Invalid latitude range
UNIVERSITY;TUM;best University for Robotics;;11.56                           // Latitude field is blank
GASSTATION;;open untill 21 Uhr in Dresden;51.05;13.67                        // POI name field is blank
college;Brandenburg123 Gate;Touristic attraction in Berlin;52.51;13.77       // Poi type unknown
TOURISTIC;berlin;Capital City;52.52;13.405                                   // Proper line

 * @param const int& error      : IN error number for identifying the error
 * @param const unsigned short int& lineNumber : IN line Number at which error occurred
 * @param const string& line                   : IN line content at which error occurred
 * @returnvalue void
 */
void CCSV::errorHandler(const int& error, const unsigned short int& lineNumber,
		const string& line)
{
	switch (error)
	// handle the exception thrown
	{
	case ERROR_MISSING_ATTRIBUTES:
		cout << "ERROR : " << "MISSING ATTRIBUTES " << "in line" // some attributes of WP/POI are missing
				<< lineNumber + 1 << " : " << line << endl;
		break;
	case ERROR_UNKNOWN_ATTRIBUTES:
		cout << "ERROR : " << "UNKNOWN ATTRIBUTES " << "in line" // some attributes of WP/POI are unknown
				<< lineNumber + 1 << " : " << line << endl;
		break;
	case ERROR_INVALID_NAME:
		cout << "ERROR : " << "INVALID NAME " << "in line" << lineNumber + 1 // invalid name of POI/Wp
		<< " : " << line << endl;
		break;
	case ERROR_CORRUPTED_DATA:
		cout << "ERROR : " << "CORRUPTED DATA (LONGITUDE/LATITUDE) " // corrupted longitude or latitude data in string form
				<< "in line" << lineNumber + 1 << " : " << line << endl;
		break;
	case ERROR_INVALID_RANGE:
		cout << "ERROR : "
				<< "INVALID DATA (LONGITUDE/LATITUDE : NOT IN VALID RANGE) " // longitude or latitude not in valid range
				<< "in line" << lineNumber + 1 << " : " << line << endl;
		break;
	case ERROR_INVALID_POI_TYPE:
		cout << "ERROR : " << "INVALID POI TYPE " << "in line" << lineNumber + 1 // invalid type of POI
		<< " : " << line << endl;
		break;
	default:
		cout << "ERROR : UNKNOWN ERROR in line " << lineNumber + 1 << " : "
				<< line << endl;
		break;
	}
}

/**
 * Function validates each line read from WP persistent storage and
 * checks for user errors such as missing attributes, unknown attributes,
 * corrupted data, data in invalid range etc
 * @param string& name        : OUT name of WP
 * @param double& latitude    : OUT latitude of WP
 * @param double& longitude   : OUT longitude of WP
 * @param const string& line  : IN WP line to be validated
 * @returnvalue true in case all WP data are valid, otherwise throws an exception
 */
bool CCSV::validateEachWpLine(string& name, double& latitude, double& longitude,
		 string& line)
{
	using namespace waypoint;
	// opening namespace for WP

	CString myString;

	vector<string> attributeVector;
	getAttributesFromLine(line, attributeVector); // get individual attributes from a line (CSV format)

	if (attributeVector.size() < NUMBER_OF_WP_ATTRIBUTES)
		// validating the number of attributes extracted from a line
		throw ERROR_MISSING_ATTRIBUTES;
	else if (attributeVector.size() > NUMBER_OF_WP_ATTRIBUTES)
		// validating the number of attributes extracted from a line
		throw ERROR_UNKNOWN_ATTRIBUTES;
	else
	{
		if (validateName(attributeVector[NAME]))
			// validating name of WP
			name = attributeVector[NAME];
		else
			throw ERROR_INVALID_NAME;

		if (validateLongitudeLatitude(attributeVector[LATITUDE],
				attributeVector[LONGITUDE]))
		// validating latitude and longitude in string form
		{
			longitude = myString.stringToDouble(attributeVector[LONGITUDE]); // converting longitude from string to double form
			latitude = myString.stringToDouble(attributeVector[LATITUDE]); // converting latitude from string to double form

			if (validateRange(latitude, longitude))
				// validating the range of latitude and longitude
				return true;                       // all validations SUCCESSFUL
			else
				throw ERROR_INVALID_RANGE;
		}
		else
			throw ERROR_CORRUPTED_DATA;
	}
}

/**
 * Function validates name of POI/WP in string form as read from persistent storage
 * Allows only alphabets or white spaces in Name
 * Name also shouldn't be blank
 * @param const string& name : IN name of POI/WP to be validated
 * @returnvalue true in case Name is valid, otherwise false
 */
bool CCSV::validateName(const string& name)
{
	CString myString;

	return (myString.isOnlyAplhabets(name));
	/*
	 * string should contain only alphabets and whitespaces
	 * name should not be blank
	 * Reusing the function from my own CString Library
	 */
}

/**
 * Function validates longitude and latitude of POI/WP in string form as read from persistent storage.
 * Longitude and latitude of POI/WP in string form should contain ONLY numbers, at the max single '.' and
 * single '-' indicating decimal and negative numbers. Also longitude and latitude fields should not be blank.
 * No other alphabets or special characters allowed in the string form of longitude and latitude.
 * @param const string& latitude  : IN latitude of POI/WP in string form to be validated
 * @param const string& longitude : IN longitude of POI/WP in string form to be validated
 * @returnvalue true in case BOTH latitude and longitude in string form is valid, otherwise false
 */
bool CCSV::validateLongitudeLatitude(const string& latitude,
		const string& longitude)
{
	CString myString;

	return (myString.isStringValidNumber(latitude)
			&& myString.isStringValidNumber(longitude));

	/* The string form of longitude and latitude of POI/ WP should contain only numbers.These fields should not be
	 *blank. Longitude and latitude can contain at the max single '.' and single '-' character indicating decimal
	 * and negative numbers respectively. No other special characters are allowed.
	 *Reusing the function from my own CString Library
	 */
}

/**
 * Function validates the range of longitude and latitude of POI/WP in double format,
 * after the conversion from string to double format.
 * @param const double& latitude  : IN latitude of POI/WP
 * @param const double& longitude : IN longitude of POI/WP
 * @returnvalue true in case BOTH latitude and longitude are in valid range, otherwise false
 */
bool CCSV::validateRange(const double& latitude, const double& longitude)
{
	return (latitude >= -90 && latitude <= 90 && longitude >= -180 // checks if latitude and longitude of POI/WP are in valid range
	&& longitude <= 180);
}

/**
 * Function validates the type of POI in string form as read from persistent storage.
 * POI type should be one among the known types such as Restaurant, Touristic,
 * Gasstation and University
 * @param const string& poiType : IN type of POI in string form
 * @returnvalue true in case type of POI is valid, otherwise false
 */
bool CCSV::validatePoiType(const string& poiType)
{
	return (!poiType.compare("RESTAURANT") || !poiType.compare("TOURISTIC") // checks if type of POI is one among the known types of POI
			|| !poiType.compare("GASSTATION") || !poiType.compare("UNIVERSITY"));
}

/**
 * Function validates each line read from POI persistent storage and
 * checks for user errors such as missing attributes, unknown attributes,
 * corrupted data, data in invalid range, invalid POI type etc
 * @param string& name        : OUT name of POI
 * @param double& latitude    : OUT latitude of POI
 * @param double& longitude   : OUT longitude of POI
 * @param string& type        : OUT type of POI
 * @param string& description : OUT description of POI
 * @param const string& line  : IN POI line to be validated
 * @returnvalue true in case all POI data are valid, otherwise throws an exception
 */
bool CCSV::validateEachPoiLine(string& name, double& latitude,
		double& longitude, string& type, string& description,
		 string& line)
{
	using namespace POI;
	// opening namespace for POI
	CString myString;

	vector<string> attributeVector;
	getAttributesFromLine(line, attributeVector); // get individual attributes from a line (CSV format)

	if (attributeVector.size() < NUMBER_OF_POI_ATTRIBUTES)
		// validating the number of attributes extracted from a line
		throw ERROR_MISSING_ATTRIBUTES;
	else if (attributeVector.size() > NUMBER_OF_POI_ATTRIBUTES)
		// validating the number of attributes extracted from a line
		throw ERROR_UNKNOWN_ATTRIBUTES;
	else
	{
		if (validateName(attributeVector[NAME]))
			// validating name of POI
			name = attributeVector[NAME];
		else
			throw ERROR_INVALID_NAME;

		description = attributeVector[DESCRIPTION];

		if (validatePoiType(attributeVector[TYPE]))
			// validating type of POI
			type = attributeVector[TYPE];
		else
			throw ERROR_INVALID_POI_TYPE;

		if (validateLongitudeLatitude(attributeVector[LATITUDE],
				attributeVector[LONGITUDE]))
		// validating latitude and longitude in string form
		{
			longitude = myString.stringToDouble(attributeVector[LONGITUDE]); // converting longitude from string to double form
			latitude = myString.stringToDouble(attributeVector[LATITUDE]); // converting latitude from string to double form

			if (validateRange(latitude, longitude))
				// validating the range of latitude and longitude
				return true;                       // all validations SUCCESSFUL
			else
				throw ERROR_INVALID_RANGE;
		}
		else
			throw ERROR_CORRUPTED_DATA;
	}
}

/**
 * Function reads and stores the entire lines from a CSV file
 * @param const string& filename      : IN name of the file to be read
 * @param vector<string>& lineVector  : OUT gets the entire lines from the file
 * @returnvalue void
 */
void CCSV::getEntireLinesFromFile(const string& filename,
		vector<string>& lineVector)
{
	CFileIO myFile;
	string line;

	lineVector.clear();              // clear any previous content of lineVector

	myFile.openFile(filename, CFileIO::IN);  // opening filename in IN mode

	while (myFile.readLineFromFile(line))    // reading line by line
	{
		if ((line != ""))
			// blank lines are not added to lineVector. Blank lines are considered as invalid lines
			lineVector.push_back(line);
	}

	myFile.closeFile();             // closing the file
}

/**
 * Function to get individual attributes from a line based on CSV format
 * Each attributes are separated by "," or ";" in CSV files
 * @param const string& line              : IN line from which attributes are to be extracted
 * @param vector<string>& attributeVector : OUT individual attributes are stored in attributeVector
 * @returnvalue void
 */
void CCSV::getAttributesFromLine( const string& line,
		vector<string>& attributeVector)
{
	CString myString;
	string source=line,attribute;


	while (myString.cutLeft(source, attribute)) // Each attributes are separated by "," or ";" in CSV files
	{
		attributeVector.push_back(attribute); // individual attributes are stored in attributeVector
	}
	attributeVector.push_back(attribute);
}

/**
 * Function returns the type of POI based on the input string
 * @param const string& type : IN type of POI in string
 * @returnvalue t_poi type of POI
 */
CPOI::t_poi stringToPoiType(const string& type)
{

	if (type.compare("RESTAURANT")== 0)
		return CPOI::RESTAURANT;
	else if (type.compare("TOURISTIC") == 0)
		return CPOI::TOURISTIC;
	else if (type.compare("GASSTATION") == 0)
		return CPOI::GASSTATION;
	else
		return CPOI::UNIVERSITY;
}

