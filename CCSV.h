/***************************************************************************
 *============= Copyright by Darmstadt University of Applied Sciences =======
 ****************************************************************************
 * Filename        : CCSV.H
 * Author          : George Sebastian
 * Description     : CCSV is derived from the Abstract CPersistentStroage class.
 *                   CCSV class provides access to persistent storage.
 *                   Two files are maintained for POI Database and WP Database
 *                   in CSV format. This class provides functions for reading from and
 *                   writing to these files. Important thing : separator should not
 *                   be a part of the attributes of POI and WP
 ****************************************************************************/
#ifndef CCSV_H
#define CCSV_H

#include <vector>
#include "CPersistentStorage.h"

class CCSV: public CPersistentStorage
{

private:

	std::string m_mediaName;

	/**
	 * Function validates the type of POI in string form as read from persistent storage.
	 * POI type should be one among the known types such as Restaurant, Touristic,
	 * Gasstation and University
	 * @param const string& poiType : IN type of POI in string form
	 * @returnvalue true in case type of POI is valid, otherwise false
	 */
	bool validatePoiType(const std::string& poiType);

	/**
	 * Function validates name of POI/WP in string form as read from persistent storage
	 * Allows only alphabets or white spaces in Name
	 * Name also shouldn't be blank
	 * @param const string& name : IN name of POI/WP to be validated
	 * @returnvalue true in case Name is valid, otherwise false
	 */
	bool validateName(const std::string& name);

	/**
	 * Function validates longitude and latitude of POI/WP in string form as read from persistent storage.
	 * Longitude and latitude of POI/WP in string form should contain ONLY numbers, at the max single '.' and
	 * single '-' indicating decimal and negative numbers. Also longitude and latitude fields should not be blank.
	 * No other alphabets or special characters allowed in the string form of longitude and latitude.
	 * @param const string& latitude  : IN latitude of POI/WP in string form to be validated
	 * @param const string& longitude : IN longitude of POI/WP in string form to be validated
	 * @returnvalue true in case BOTH latitude and longitude in string form is valid, otherwise false
	 */
	bool validateLongitudeLatitude(const std::string& latitude,
			const std::string& longitude);

	/**
	 * Function validates the range of longitude and latitude of POI/WP in double format,
	 * after the conversion from string to double format.
	 * @param const double& latitude  : IN latitude of POI/WP
	 * @param const double& longitude : IN longitude of POI/WP
	 * @returnvalue true in case BOTH latitude and longitude are in valid range, otherwise false
	 */
	bool validateRange(const double& latitude, const double& longitude);

	/**
	 * Function handles different possible error conditions while validating each POI or WP line
	 * read from persistent storage
	 * @param const int& error      : IN error number for identifying the error
	 * @param const unsigned short int& lineNumber : IN line Number at which error occurred
	 * @param const string& line                   : IN line content at which error occurred
	 * @returnvalue void
	 */
	void errorHandler(const int& error, const unsigned short int& lineNumber,
			const string& line);

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
	bool validateEachWpLine(std::string& name, double& latitude,
			double& longitude,  std::string& line);

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
	bool validateEachPoiLine(std::string& name, double& latitude,
			double& longitude, std::string& type, std::string& description,
			 std::string& line);

	/**
	 * Function to get individual attributes from a line based on CSV format
	 * Each attributes are separated by "," or ";" in CSV files
	 * @param const string& line              : IN line from which attributes are to be extracted
	 * @param vector<string>& attributeVector : OUT individual attributes are stored in attributeVector
	 * @returnvalue void
	 */
	void getAttributesFromLine( const std::string& line,
			std::vector<string>& attributeVector);

	/**
	 * Function reads and stores the entire lines from a CSV file
	 * @param const string& filename      : IN name of the file to be read
	 * @param vector<string>& lineVector  : OUT gets the entire lines from the file
	 * @returnvalue void
	 */
	void getEntireLinesFromFile(const std::string& filename,
			std::vector<std::string>& lineVector);

public:

	/**
	 * Function sets the name of Media
	 * @paramconst string& name : name of the Media
	 * @returnvalue void
	 */
	void setMediaName(const std::string& name);

	/**
	 * Function writes POI Database and WP Database data to our persistent storage in CSV format
	 * @param const CWpDatabase& waypointDb     : IN database with Waypoints
	 * @param const CPoiDatabase& poiDb         : IN database with POIs
	 * @returnvalue true is data could be saved successfully
	 */
	bool writeData(const CWpDatabase& waypointDb, const CPoiDatabase& poiDb);

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
	bool readData(CWpDatabase& waypointDb, CPoiDatabase& poiDb,
			CCSV::MergeMode mode);

};



/********************
 **  CLASS END
 *********************/
#endif /* CCSV_H */
