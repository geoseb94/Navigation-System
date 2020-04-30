/***************************************************************************
 *============= Copyright by Darmstadt University of Applied Sciences =======
 ****************************************************************************
 * Filename        : CPERSISTENTSTORAGE.H
 * Author          : George Sebastian
 * Description     : CPersistentStorage class is an Abstract Class.
 *                   No objects of this class can be instantiated
 ****************************************************************************/
#ifndef CPERSISTENTSTORAGE_H
#define CPERSISTENTSTORAGE_H

#include <string>

#include "CWpDatabase.h"
#include "CPoiDatabase.h"

class CPersistentStorage{

public :

//	CPersistentStorage();
//	virtual ~CPersistentStorage();

	/**
	 * Function sets the name of Media
	 * @paramconst string& name : name of the Media
	 * @returnvalue void
	 */
	virtual void setMediaName(const std::string& name)=0;

	/**
	 * Function writes POI Database and WP Database data to our persistent storage in CSV format
	 * @param const CWpDatabase& waypointDb     : IN database with Waypoints
	 * @param const CPoiDatabase& poiDb         : IN database with POIs
	 * @returnvalue true is data could be saved successfully
	 */
	virtual  bool writeData(const CWpDatabase& waypointDb, const CPoiDatabase&  poiDb )=0;

	/*
	 *  Mode to be used while reading from the file
	 */
	enum MergeMode{MERGE,REPLACE};

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
	virtual bool readData(CWpDatabase& waypointDb, CPoiDatabase&  poiDb,MergeMode mode )=0;

};

/********************
 **  CLASS END
 *********************/
#endif /* CPERSISTENTSTORAGE_H  */
