/***************************************************************************
 *============= Copyright by Darmstadt University of Applied Sciences =======
 ****************************************************************************
 * Filename        : CROUTE.H
 * Author          : George Sebastian
 * Description     : Route class consists of Waypoints and POIs in our Route.
 *                   Dynamic memory allocation is used for storing the Waypoints and POIs present in our Route
 *                   Waypoints are added to Route class via addWaypoint() function
 *                   & POIs are added to Route class from POIDatabase via addPoi() function
 ****************************************************************************/
#ifndef CROUTE_H
#define CROUTE_H

#include <list>
#include <vector>

#include "CPoiDatabase.h"
#include "CWpDatabase.h"
#include "CWaypoint.h"

class CWaypoint;

class CRoute
{
private:

    list <CWaypoint*> m_pWaypointList;

    unsigned short int m_noPoi;
    unsigned short int m_noWp;

	CPoiDatabase* m_pPoiDatabase;
	CWpDatabase* m_pWpDatabase;

public:

	/**
	 * Constructor for the CRoute class
	 * Initialize the attributes of the class with some initial values
	 * @return no value
	 */
	CRoute();

	/**
	 * Copy constructor of class CRoute
	 * @param const CRoute& origin : IN the object of the class to be copied is passed via reference
	 * @return no value
	 */
	CRoute(CRoute const& origin);

	/**
	 * Assignment operator of class CRoute
	 * @param const CRoute& origin : IN the object of the class to be assigned is passed via reference
	 * @return CRoute& : returned via reference
	 */
	CRoute& operator=(const CRoute& origin);

	/**
	 * Destructor for CRoute class
	 * Destroys the List in our Route
	 * @param no parameters
	 * @return no value
	 */
	~CRoute();

	/**
	 * operator+= is overloaded for addition of POI and WP to our Routes.
	 * Checks for the string name passed by user in both WP and POI Databases
	 * If found in Both Databases, WP is added before POI.
	 * Otherwise, simply added to the end of our Route
	 * @param const std::string& name : IN name of POI or WP to be added to Route
	 * @return CRoute& : returned via reference
	 */
	CRoute& operator+=(const std::string& name);

	/**
	 * operator+ is overloaded for the concatenation of two CRoute objects
	 * The two routes are concatenated if both Routes are connected to same WP and POI Databases.
	 * If not, then return empty Route
	 * @param CRoute const& rop : IN object of type CRoute, right operator
	 * @return CRoute : object of CRoute type, contains result of the concatenation
	 */
	CRoute operator+(CRoute const& rop);

	/**
	 * Function connects CRoute class to the POI Database
	 * @param CPoiDatabase* pPoiDB : IN pointer to the intended POI Database,which should be connected to the CRoute class
	 * @returnvalue void
	 */
	void connectToPoiDatabase(CPoiDatabase* pPoiDB);


	/**
	 * Function connects CRoute class to the WP Database
	 * @param CWpDatabase* pWpDB : IN pointer to the intended WP Database,which should be connected to the CRoute class
	 * @returnvalue void
	 */
	void connectToWpDatabase(CWpDatabase* pWpDB);

	/**
	 * Function checks for the WP name passed as parameter in the WP Database and add WP to our route from WP Database.
	 * In case if WP is not found in the WP Database, ERROR message is printed.
	 * Function also checks if Route is connected a valid WP Database
	 * @param const string& name : IN name of WP which should be added from WP Database
	 * @returnvalue void
	 */
	void addWaypoint(const std::string& name);

	/**
	 * Function checks for the POI name passed as parameter in the POI Database and add POI to our route from POI Database
	 * after the last 'afterWp' in our Route. In case if 'afterWP' is not found in our Route, POI is NOT added.
	 * In case if user does not provide 'afterWp' parameter(default value), POI is added to the end of our Route.
	 * In case if POI is not found in the POI Database, ERROR message is printed.
	 * Function also checks if Route is connected a valid POI Database.
	 * @param const string& namePoi : IN name of the POI to be added to our route from the POI Database
	 * @param const string& afterWp : IN POI is added to our Route after the last 'afterWP' in our Route
	 * @returnvalue void
	 */
	void addPoi(const std::string& namePoi,const std::string& afterWp ="default WP");

	/**
	 * Function calculates and returns the distance to the nearest POI in our route from the Waypoint passed as parameter
	 * @param CWaypoint const& wp  : IN const object of type CWaypoint passed via reference, function calculates the distance to the nearest POI in our route from this Waypoint
	 * @param CPOI& poi            : OUT copies the details of nearest POI in our Route to this object
	 * @return double : the distance to the nearest POI in our route
	 */
	double getDistanceNextPoi(CWaypoint const& wp, CPOI& poi);

	/**
	 * Function print the Waypoints and POIs in our route
	 * @param no parameters
	 * @returnvalue void
	 */
	void print();

	/**
	 * Function returns a vector containing the Route
	 * @param no parameter
	 * @return const std::vector<const CWaypoint*> : vector containing Route
	 */
	const std::vector<const CWaypoint*> getRoute();
};
/********************
 **  CLASS END
 *********************/
#endif /* CROUTE_H */
