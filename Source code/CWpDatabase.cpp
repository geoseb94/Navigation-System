/***************************************************************************
 *============= Copyright by Darmstadt University of Applied Sciences =======
 ****************************************************************************
 * Filename        : CWPDATABASE.CPP
 * Author          : George Sebastian
 * Description     : CWpDatabase class maintain a Database for WPs and WPs to our Route
 *                   are added from this Database
 ****************************************************************************/
#include <iostream>
using namespace std;

#include "CWpDatabase.h"

/**
 * Constructor for the CWpDatabase class
 * @param no parameters
 * @return no values
 */
CWpDatabase::CWpDatabase()
{
}

/**
 * Function adds WP object provided by user to WP Database, if WP is valid
 * If WP is already existing in Database, it will be overwritten and Warning is
 * displayed.
 * @param CWaypoint const& wp : IN WP object to be added to WP Database
 * @returnvalue void
 */
void CWpDatabase::addWaypoint(CWaypoint const& wp)
{
	CDatabase::addObjectToDb(wp);

}

/**
 * Function checks for the WP having the same name as passed by the user in the WP Database
 * and returns the pointer to the WP in the WP Database
 * @param const string& name : IN name of the WP to be checked in WP Database
 * @returnvalue CWaypoint*   : pointer to the WP in WP Database if found,otherwise returns a NULL pointer
 */
CWaypoint* CWpDatabase::getPointerToWaypoint(const string& name)
{
	return CDatabase::getPointerToObject(name);
}

/**
 * Function prints the contents of WP Database
 * @param no parameter
 * @returnvalue void
 */
void CWpDatabase::print()
{
	cout << endl << "***********************************" << endl
			<< "Waypoint Database" << endl
			<< "***********************************" << endl;

	CDatabase::printDb();
}

/**
 * Function used to extract WpMap containing all WPs in WP Database
 * @param std::map<std::string, CWaypoint>& wpMap : OUT wpMap containing all WPs in WP Database
 * @returnvalue void
 */
const void CWpDatabase::getWpMap(std::map<std::string, CWaypoint>& wpMap) const
{
	CDatabase::getDbContainer(wpMap);
}

/**
 * Function clears the contents of WP Database
 * @param no parameters
 * @returnvalue void
 */
void CWpDatabase::clearWpDb()
{
	CDatabase::clearDb();
}
