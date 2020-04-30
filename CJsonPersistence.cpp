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
#include <typeinfo>
#include <iostream>
#include <string>
using namespace std;

#include "CJsonPersistence.h"
#include "CFileIO.h"
#include "CString.h"
#include "CPOI.h"
#include "CJsonScanner.h"
using namespace APT;

/**
 * Function returns the type of POI based on the input string
 * @param const string& type : IN type of POI in string
 * @returnvalue t_poi type of POI
 */
CPOI::t_poi stringToPoi(const string& type);

/**
 * Template Function for translating Database (WP Database or POI Database) to JSON Format.
 * Translated version can be found in the string "line"
 * @param const map<string, dBType>& dbContainer : IN  Map containing the elements of Database (WP or POI Database)
 * @param string& line                           : OUT string line contains the translated version
 * @returnvalue void
 */
template<class dBType>
void CJsonPersistence::translateDbToJson(const map<string, dBType>& dbContainer,
		string& line)
{
	CString myString;

	for (typename map<string, dBType>::const_iterator itr = dbContainer.begin(); // iterating through the Map
	itr != dbContainer.end(); ++itr)
	{

		line = line + ("\t{\n\t \"name\": \"")
				+ (itr->second).getName() // translating the contents of WaypointMap to JSON Format
				+ "\",\n\t \"latitude\": "
				+ myString.doubleToString((itr->second).getLatitude())
				+ ",\n\t \"longitude\": "
				+ myString.doubleToString((itr->second).getLongitude());

		if (typeid(dBType) == typeid(CPOI)) // handling the translation of POI Database to JSON
		{
			const CPOI* pPoi;
			pPoi = (CPOI*) (&(itr->second));
			line = line + ",\n\t \"type\": \"" + (pPoi)->getType()
					+ "\"" // translating the contents of poitMap to JSON Format
					+ ",\n\t \"description\": \"" + (pPoi)->getPoiDescription()
					+ "\"";
		}
		line = line + "\n\t},\n";
	}
	line = line.substr(0, line.find_last_of(',')) + "\n  ]"; // removing the last "," from the string (JSON Format)
}

/**
 * Function for writing Data to File in JSON Format
 * In this context, Data means element of WP Database and POI Database
 * @param const CWpDatabase& waypointDb : IN  Object of CWpDatabase, which needs to be written to File
 * @param const CPoiDatabase& poiDb     : IN  Object of CPoiDatabase, which needs to be written to File
 * @returnvalue true,if writing operation is successful
 */
bool CJsonPersistence::writeData(const CWpDatabase& waypointDb,
		const CPoiDatabase& poiDb)
{
	CFileIO myFile;
	string line;
	map<string, CWaypoint> wpMap;
	map<string, CPOI> poiMap;

	waypointDb.getWpMap(wpMap); // get Wp Map containing elements of WP Database
	poiDb.getPoiMap(poiMap);  // get POI Map containing elements of POI Database

	line = "{\n  \"waypoints\":  [\n";
	translateDbToJson(wpMap, line); // WP Database is translated to JSON Format and 'line' is updated
	line = line + ",\n  \"pois\":  [\n";
	translateDbToJson(poiMap, line); // POI Database is translated to JSON Format and 'line' is updated

	myFile.openFile("json.txt", CFileIO::OUT);  // opening the File
	myFile.writeLineToFile(line + "\n}");       // writing to File
	myFile.closeFile();                         // closing the File

	return true;
}

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
bool CJsonPersistence::readData(CWpDatabase& waypointDb, CPoiDatabase& poiDb,
		CJsonPersistence::MergeMode mode)
{
	fstream jsonFile;
	CJsonToken* token;
	state nextState;
	string databaseName, attributeName; // declaring variables required for State Machine processing
	string name, type, description;
	double longitude, latitude;
	bool objectOK = true;
	unsigned int attributeCount = 0;
	int count[5] =
	{ 0, 0, 0, 0, 0 };

	jsonFile.open("json.txt", fstream::in);  // opening file
	CJsonScanner scan(jsonFile);

	nextState = IS_WAITING_FILE_BEGIN;      // initial state

	if (mode == REPLACE) // If merge mode is REPLACE, the content of persistent storage will completely replace
	{                            // the content of WP Database and POI Database.
		waypointDb.clearDb();
		poiDb.clearDb();                    // clearing both Database
	}

	try
	{
		token = scan.nextToken();
	} catch (string & illegalChar)     // if illegal character is read from file
	{
		cout << "ERROR: Invalid character at line :" << scan.scannedLine()
				<< ".Retrying.." << endl;
		token = scan.nextToken();         // retrying by scanning the next token
	}

	while (token != NULL)  // until the file is completely read
	{
		switch (nextState)
		{
		case IS_WAITING_FILE_BEGIN:
		{

			if (token->getType() == CJsonToken::BEGIN_OBJECT)
				nextState = IS_WAITING_DB_NAME;
			else
				cout << "ERROR: Unexpected character at line :"
						// Unexpected character read
						<< scan.scannedLine()
						<< ". Expecting BEGIN_OBJECT ({) at this place. Retrying.."
						<< endl;
			break;
		}

		case IS_WAITING_DB_NAME:
		{

			if (token->getType() == CJsonToken::STRING)
			{

				CJsonStringToken* stringToken = (CJsonStringToken*) token; // type casting token to special token
				databaseName = stringToken->getValue();

				if (databaseName == "pois" || databaseName == "waypoints") // known Database - Waypoint DB and POI DB
					nextState = IS_WAITING_DB_NAME_SEPARATOR;
				else
					cout << "ERROR: Unknown DB name at line :"
							<< scan.scannedLine()
							<< " Therefore skipping the entire unknown DB"
							<< endl;
			}
			else
				cout << "ERROR: Unexpected character at line :"
						// Unexpected character read
						<< scan.scannedLine()
						<< ". Expecting DATABASE NAME at this place. Retrying.."
						<< endl;
			break;
		}

		case IS_WAITING_DB_NAME_SEPARATOR:
		{

			if (token->getType() == CJsonToken::NAME_SEPARATOR)
				nextState = IS_WAITING_DB_ARRAY_BEGIN;
			else
				cout << "ERROR: Unexpected character at line :"
						// Unexpected character read
						<< scan.scannedLine()
						<< ". Expecting NAME_SEPARATOR (:) at this place. Retrying.."
						<< endl;
			break;
		}

		case IS_WAITING_DB_ARRAY_BEGIN:
		{

			if (token->getType() == CJsonToken::BEGIN_ARRAY)
				nextState = IS_WAITING_OBJECT_BEGIN;
			else
				cout << "ERROR: Unexpected character at line :"
						// Unexpected character read
						<< scan.scannedLine()
						<< ". Expecting BEGIN_ARRAY ([) at this place. Retrying.."
						<< endl;
			break;
		}

		case IS_WAITING_OBJECT_BEGIN:
		{

			if (token->getType() == CJsonToken::BEGIN_OBJECT)
				nextState = IS_WAITING_ATTRIBUTE_NAME;
			else
				cout << "ERROR: Unexpected character at line :"
						// Unexpected character read
						<< scan.scannedLine()
						<< ". Expecting BEGIN_OBJECT ({) at this place. Retrying.."
						<< endl;
			break;
		}

		case IS_WAITING_ATTRIBUTE_NAME:
		{

			if (token->getType() == CJsonToken::STRING)
			{
				CJsonStringToken* stringToken = (CJsonStringToken*) token; // type casting token to special token
				attributeName = stringToken->getValue();

				if (attributeName == "name" || attributeName == "longitude" // known attribute names
				|| attributeName == "latitude" || attributeName == "type"
						|| attributeName == "description")
					nextState = IS_WAITING_ATTRIBUTE_NAME_SEPARATOR;
				else
				{
					cout << "ERROR: Corrupted Object at line :"
							<< scan.scannedLine()
							<< ". Therefore skipping current object.." << endl;
					objectOK = false;                       // Object is corrupt
					nextState = IS_WAITING_OBJECT_SEPARATOR;
				}
			}
			else
				cout << "ERROR: Unexpected character at line :"
						// Unexpected character read
						<< scan.scannedLine()
						<< ". Expecting ATTRIBUTE_NAME at this place. Retrying.."
						<< endl;
			break;
		}

		case IS_WAITING_ATTRIBUTE_NAME_SEPARATOR:
		{

			if (token->getType() == CJsonToken::NAME_SEPARATOR)
				nextState = IS_WAITING_ATTRIBUTE_VALUE;
			else
				cout << "ERROR: Unexpected character at line :"
						// Unexpected character read
						<< scan.scannedLine()
						<< ". Expecting NAME_SEPARATOR (:) at this place. Retrying.."
						<< endl;
			break;
		}

		case IS_WAITING_ATTRIBUTE_VALUE:
		{
			if ((token->getType() == CJsonToken::STRING)
					|| (token->getType() == CJsonToken::NUMBER))
			{
				CJsonStringToken* stringToken = (CJsonStringToken*) token;

				if (token->getType() == CJsonToken::STRING)
				{
					if (attributeName == "name")
					{
						name = stringToken->getValue();
						count[NAME]++;
						if (count[NAME] == 2) // checking if the attribute is present twice in an Object
							objectOK = false;               // Object is corrupt
					}
					else if (attributeName == "type")
					{
						type = stringToken->getValue();
						count[TYPE]++;
						if (count[TYPE] == 2) // checking if the attribute is present twice in an Object
							objectOK = false;               // Object is corrupt
					}
					else
					{
						description = stringToken->getValue();
						count[DESCRIPTION]++;
						if (count[DESCRIPTION] == 2) // checking if the attribute is present twice in an Object
							objectOK = false;               // Object is corrupt
					}
					attributeCount++;

				}
				else
				{
					CJsonNumberToken* numberToken = (CJsonNumberToken*) token;

					if (attributeName == "latitude")
					{
						latitude = numberToken->getValue();
						count[LATITUDE]++;
						if (count[LATITUDE] == 2) // checking if the attribute is present twice in an Object
							objectOK = false;               // Object is corrupt
					}
					else
					{
						longitude = numberToken->getValue();
						count[LONGITUDE]++;

						if (count[LONGITUDE] == 2) // checking if the attribute is present twice in an Object
							objectOK = false;               // Object is corrupt
					}

					if (!(latitude >= -90 && latitude <= 90 && longitude >= -180 // Checking the validity of the values of Latitude and Longitude
					&& longitude <= 180))
						objectOK = false;                   // Object is corrupt

					attributeCount++;
				}

				if ((databaseName == "waypoints" // Object is corrupt if objectOK is 'False' or unknown Database Names
				&& (attributeCount > 3 || objectOK == false)) // number of attributes of WP should be 3 and number of attributes of POI should be 5
						|| (databaseName == "pois"
								&& (attributeCount > 5 || objectOK == false)))
				{
					nextState = IS_WAITING_OBJECT_SEPARATOR;
					cout << "ERROR: Corrupted Object at line :"
							<< scan.scannedLine()
							<< ". Therefore skipping current object.." << endl;
				}

				nextState = IS_WAITING_ATTRIBUTE_VALUE_SEPARATOR;
			}
			else
				cout << "ERROR: Unexpected character at line :"
						// Unexpected character read
						<< scan.scannedLine()
						<< ". Expecting ATTRIBUTE VALUE at this place. Retrying.."
						<< endl;
			break;
		}

		case IS_WAITING_ATTRIBUTE_VALUE_SEPARATOR:
		{

			if ((token->getType() == CJsonToken::VALUE_SEPARATOR)
					|| (token->getType() == CJsonToken::END_OBJECT))
			{
				if (token->getType() == CJsonToken::VALUE_SEPARATOR)
					nextState = IS_WAITING_ATTRIBUTE_NAME;

				else
				{
					nextState = IS_WAITING_OBJECT_SEPARATOR;

					if ((databaseName == "waypoints") && (objectOK == true))
					{
						if (count[NAME] == 0 || count[LATITUDE] == 0 // required attributes are present at least once
						|| count[LONGITUDE] == 0)
						{
							cout << "ERROR: Attributes missing at line :"
									// Object is corrupt
									<< scan.scannedLine()
									<< "  Therefore skipping current object.."
									<< endl;
						}
						else
						{
							CWaypoint wp(name, latitude, longitude);
							waypointDb.addWaypoint(wp); // WP is added to WP Database
						}
					}
					if ((databaseName == "pois") && (objectOK == true))
					{
						if (count[NAME] == 0 || count[LATITUDE] == 0 // required attributes are present at least once
						|| count[LONGITUDE] == 0 || count[TYPE] == 0
								|| count[DESCRIPTION] == 0)
						{
							cout << "ERROR: Attributes missing at line :"
									// Object is corrupt
									<< scan.scannedLine()
									<< "  Therefore skipping current object.."
									<< endl;
						}
						else

						{
							CPOI poi(stringToPoi(type), name, description,
									latitude, longitude);
							poiDb.addPoi(poi);   // POI is added to POI Database
						}
					}

				}
			}
			else
				cout << "ERROR: Unexpected character at line :"
						// Unexpected character read
						<< scan.scannedLine()
						<< ". Expecting either VALUE_SEPARATOR (,) or END_OBJECT (}) at this place. Retrying.."
						<< endl;
			break;
		}

		case IS_WAITING_OBJECT_SEPARATOR:
		{
			attributeCount = 0;
			objectOK = true;
			count[NAME] = 0, count[LATITUDE] = 0, count[LONGITUDE] = 0, count[TYPE] = // resetting the variables
					0, count[DESCRIPTION] = 0;

			if ((token->getType() == CJsonToken::VALUE_SEPARATOR)
					|| (token->getType() == CJsonToken::END_ARRAY))
			{
				if (token->getType() == CJsonToken::VALUE_SEPARATOR)
					nextState = IS_WAITING_OBJECT_BEGIN;

				else
					nextState = IS_WAITING_DB_ARRAY_SEPARATOR;

			}
			else
				cout << "ERROR: Unexpected character at line :"
						// Unexpected character read
						<< scan.scannedLine()
						<< ". Expecting either VALUE_SEPARATOR (,) or END_ARRAY (]) at this place. Retrying.."
						<< endl;
			break;
		}

		case IS_WAITING_DB_ARRAY_SEPARATOR:
		{

			if ((token->getType() == CJsonToken::VALUE_SEPARATOR)
					|| (token->getType() == CJsonToken::END_OBJECT))
			{
				if (token->getType() == CJsonToken::VALUE_SEPARATOR)
					nextState = IS_WAITING_DB_NAME;

				else
					nextState = IS_WAITING_FILE_BEGIN;

			}
			else
				cout << "ERROR: Unexpected character at line :"
						// Unexpected character read
						<< scan.scannedLine()
						<< ". Expecting either VALUE_SEPARATOR (,) or END_OBJECT (}) at this place. Retrying.."
						<< endl;
			break;
		}
		}

		try
		{
			token = scan.nextToken();
		} catch (string & illegalChar) // if illegal character is read from file
		{
			cout << "ERROR: Invalid character at line :" << scan.scannedLine()
					<< ".Retrying.." << endl;
			token = scan.nextToken();         // retrying by scanning next token
		}

	}

	waypointDb.print();           // printing updated WP Database
	poiDb.print();                // printing updated POI Database

	jsonFile.close();             // closing the file

	return true;
}

/**
 * Function sets the name of Media
 * @paramconst string& name : IN name of the Media
 * @returnvalue void
 */
void CJsonPersistence::setMediaName(const std::string& name)
{
	m_mediaName = name;
}

/**
 * Function returns the type of POI based on the input string
 * @param const string& type : IN type of POI in string
 * @returnvalue t_poi type of POI
 */
CPOI::t_poi stringToPoi(const string& type)
{

	if (type.compare("RESTAURANT") == 0)
		return CPOI::RESTAURANT;
	else if (type.compare("TOURISTIC") == 0)
		return CPOI::TOURISTIC;
	else if (type.compare("GASSTATION") == 0)
		return CPOI::GASSTATION;
	else
		return CPOI::UNIVERSITY;
}
