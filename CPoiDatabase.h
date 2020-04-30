/***************************************************************************
 *============= Copyright by Darmstadt University of Applied Sciences =======
 ****************************************************************************
 * Filename        : CPOIDATABASE.H
 * Author          : George Sebastian
 * Description     : CPOIDatabase class maintain a Database for POIs and POIs to our Route
 *                   are added from this Database
 ****************************************************************************/
#ifndef CPOIDATABASE_H
#define CPOIDATABASE_H

#include <map>

#include "CPOI.h"
#include "CDatabase.h"

class CPoiDatabase: public CDatabase<std::string, CPOI>
{
private:

	std::map<std::string, CPOI> m_poiMap;

public:

	/**
	 * Constructor for the CPoiDatabase class
	 * @param no parameters
	 * @return no values
	 */
	CPoiDatabase();

	/**
	 * Function checks for the POI having the same name as passed by the user in the POI Database
	 * and returns the pointer to the POI in the POI Database
	 * @param const string& name : IN name of the POI to be checked in POI Database
	 * @returnvalue CPOI*        : pointer to the POI in POI Database if found,otherwise returns a NULL pointer
	 */
	CPOI* getPointerToPoi(const std::string& name);

	/**
	 * Function adds POI object provided by user to POI Database, if POI is valid
	 * If POI is already existing in Database, it will be overwritten and Warning is
	 * displayed.
	 * @param CPOI const& poi : IN POI object to be added to POI Database
	 * @returnvalue void
	 */
	void addPoi(CPOI const& poi);

	/**
	 * Function prints the contents of POI Database
	 * @param no parameter
	 * @returnvalue void
	 */
	void print();

	/**
	 * Function used to extract poiMap containing all POIs in POI Database
	 * @param std::map<std::string, CPOI>& poiMap : OUT poiMap containing all POIs in POI Database
	 * @returnvalue void
	 */
	const void getPoiMap(std::map<std::string, CPOI>& poiMap) const;

	/**
	 * Function clears the contents of POI Database
	 * @param no parameters
	 * @returnvalue void
	 */
	void clearPoiDb();
};

/********************
 **  CLASS END
 *********************/
#endif /* CPOIDATABASE_H */
