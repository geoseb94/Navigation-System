/***************************************************************************
 *============= Copyright by Darmstadt University of Applied Sciences =======
 ****************************************************************************
 * Filename        : CWPDATABASE.H
 * Author          : George Sebastian
 * Description     : CWpDatabase class maintain a Database for WPs and WPs to our Route
 *                   are added from this Database
 ****************************************************************************/
#ifndef CWPDATABASE_H
#define CWPDATABASE_H

#include <map>

#include "CWaypoint.h"
#include "CDatabase.h"

class CWpDatabase: public CDatabase<std::string, CWaypoint>
{

	std::map<std::string, CWaypoint> m_WpMap;

public:

	/**
	 * Constructor for the CWpDatabase class
	 * @param no parameters
	 * @return no values
	 */
	CWpDatabase();

	/**
	 * Function adds WP object provided by user to WP Database, if WP is valid
	 * If WP is already existing in Database, it will be overwritten and Warning is
	 * displayed.
	 * @param CWaypoint const& wp : IN WP object to be added to WP Database
	 * @returnvalue void
	 */
	void addWaypoint(CWaypoint const& wp);

	/**
	 * Function checks for the WP having the same name as passed by the user in the WP Database
	 * and returns the pointer to the WP in the WP Database
	 * @param const string& name : IN name of the WP to be checked in WP Database
	 * @returnvalue CWaypoint*   : pointer to the WP in WP Database if found,otherwise returns a NULL pointer
	 */
	CWaypoint* getPointerToWaypoint(const std::string& name);

	/**
	 * Function prints the contents of WP Database
	 * @param no parameter
	 * @returnvalue void
	 */
	void print();

	/**
	 * Function used to extract WpMap containing all WPs in WP Database
	 * @param std::map<std::string, CWaypoint>& wpMap : OUT wpMap containing all WPs in WP Database
	 * @returnvalue void
	 */
	const void getWpMap(std::map<std::string, CWaypoint>& wpMap) const;

	/**
	 * Function clears the contents of WP Database
	 * @param no parameters
	 * @returnvalue void
	 */
	void clearWpDb();
};

/********************
 **  CLASS END
 *********************/
#endif /* CWPDATABASE_H */
