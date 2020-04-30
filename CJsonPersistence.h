/***************************************************************************
 *============= Copyright by Darmstadt University of Applied Sciences =======
 ****************************************************************************
 * Filename        : CJSONPERSISTENCE.CPP
 * Author          : George Sebastian
 * Description     : This Class is derived from the Abstract CPersistentStroage class.
 *                   This class provides access to persistent storage.
 *                   Single file is maintained for POI Database and WP Database
 *                   in JSON format. This class provides functions for reading from and
 *                   writing to these files.
 ****************************************************************************/
#ifndef CJSONPERSISTENCE_H
#define CJSONPERSISTENCE_H

#include <string>

#include "CPersistentStorage.h"

class CJsonPersistence: public CPersistentStorage
{

private:

	std::string m_mediaName;

	/**
	 * Template Function for translating Database (WP Database or POI Database) to JSON Format.
	 * Translated version can be found in the string "line"
	 * @param const map<string, dBType>& dbContainer : IN  Map containing the elements of Database (WP or POI Database)
	 * @param string& line                           : OUT string line contains the translated version
	 * @returnvalue void
	 */
	template<class dBType>
	void translateDbToJson(const map<string, dBType>& dbContainer,
			string& line);

public:

	/**
	 * Enumeration for different states in State Machine for handling read operation
	 * from JSON File
	 */
	enum state
	{
		IS_WAITING_FILE_BEGIN,
		IS_WAITING_DB_NAME,
		IS_WAITING_DB_NAME_SEPARATOR,
		IS_WAITING_DB_ARRAY_BEGIN,
		IS_WAITING_OBJECT_BEGIN,
		IS_WAITING_ATTRIBUTE_NAME,
		IS_WAITING_ATTRIBUTE_NAME_SEPARATOR,
		IS_WAITING_ATTRIBUTE_VALUE,
		IS_WAITING_ATTRIBUTE_VALUE_SEPARATOR,
		IS_WAITING_OBJECT_SEPARATOR,
		IS_WAITING_DB_ARRAY_SEPARATOR
	};

	/**
	 * Enumeration for different index for 'count[]' array
	 */
	enum attributes
	{
		NAME, LONGITUDE, LATITUDE, TYPE, DESCRIPTION
	};

	/**
	 * Function sets the name of Media
	 * @paramconst string& name : IN name of the Media
	 * @returnvalue void
	 */
	void setMediaName(const std::string& name);

	/**
	 * Function for writing Data to File in JSON Format
	 * In this context, Data means element of WP Database and POI Database
	 * @param const CWpDatabase& waypointDb : IN  Object of CWpDatabase, which needs to be written to File
	 * @param const CPoiDatabase& poiDb     : IN  Object of CPoiDatabase, which needs to be written to File
	 * @returnvalue true,if writing operation is successful
	 */
	bool writeData(const CWpDatabase& waypointDb, const CPoiDatabase& poiDb);

	/**
	 * Function for reading Data from File in JSON Format
	 * In this context, Data means element of WP Database and POI Database
	 * Function reads from persistent storage. If Merge mode is MERGE, content of Persistent
	 * storage will be merged with already existing data in WP Database and POI Database.
	 * If merge mode is REPLACE, the content of persistent storage will completely replace
	 * the content of WP Database and POI Database.
	 * @param CWpDatabase& waypointDb           : OUT database with Waypoints
	 * @param CPoiDatabase&  poiDb              : OUT database with POIs
	 * @param CJsonPersistence::MergeMode mode  : IN  merge mode (MERGE/REPLACE)
	 * @returnvalue true,if reading operation is successful
	 */
	bool readData(CWpDatabase& waypointDb, CPoiDatabase& poiDb,
			CJsonPersistence::MergeMode mode);

};

#endif
