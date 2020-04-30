/***************************************************************************
 *============= Copyright by Darmstadt University of Applied Sciences =======
 ****************************************************************************
 * Filename        : CROUTE.CPP
 * Author          : George Sebastian
 * Description     : Route class consists of Waypoints and POIs in our Route.
 *                   Dynamic memory allocation is used for storing the Waypoints and POIs present in our Route
 *                   Waypoints are added to Route class via addWaypoint() function
 *                   & POIs are added to Route class from POIDatabase via addPoi() function
 ****************************************************************************/
#include <iostream>
using namespace std;

#include "CRoute.h"
#include "CWaypoint.h"
#include "CPoiDatabase.h"
#include "CWpDatabase.h"
#include "CPOI.h"

#define DEGREE 1                // printing longitude and latitude in Degree format
#define MMSS 2                  // printing longitude and latitude in Minutes,seconds format

#define TESTCASE_ROUTE_PRINT 0   // Testcase for printing routes in different ways
#define ERROR_NO_POI_IN_ROUTE 1  // Error handling : when no POI was added to the Route

/**
 * Constructor for the CRoute class
 * Initialize the attributes of the class with some initial values
 * @return no value
 */
CRoute::CRoute()
{
	m_noWp = 0;
	m_noPoi = 0;

	m_pPoiDatabase = NULL;
	m_pWpDatabase = NULL;
}

/**
 * Copy constructor of class CRoute
 * @param const CRoute& origin : IN the object of the class to be copied is passed via reference
 * @return no value
 */
CRoute::CRoute(CRoute const& origin)
{
	m_pWaypointList = origin.m_pWaypointList;

	m_noWp = origin.m_noWp;
	m_noPoi = origin.m_noPoi;

	m_pPoiDatabase = origin.m_pPoiDatabase;
	m_pWpDatabase = origin.m_pWpDatabase;

}

/**
 * Assignment operator of class CRoute
 * @param const CRoute& origin : IN the object of the class to be assigned is passed via reference
 * @return CRoute& : returned via reference
 */
CRoute& CRoute::operator=(const CRoute& origin)
{

	if (this != &origin)                         // self copy must be protected
	{
		m_pWaypointList.clear();                 // delete old content
		m_pWaypointList = origin.m_pWaypointList;

		m_noWp = origin.m_noWp;
		m_noPoi = origin.m_noPoi;

		m_pPoiDatabase = origin.m_pPoiDatabase;
		m_pWpDatabase = origin.m_pWpDatabase;
	}
	return *this;

}

/**
 * Destructor for CRoute class
 * Destroys the List in our Route
 * @param no parameters
 * @return no value
 */
CRoute::~CRoute()
{
	m_pWaypointList.clear();
}

/**
 * Function connects CRoute class to the POI Database
 * @param CPoiDatabase* pPoiDB : IN pointer to the intended POI Database,which should be connected to the CRoute class
 * @returnvalue void
 */
void CRoute::connectToPoiDatabase(CPoiDatabase* pPoiDB)
{
	//connecting my Route class to the POI Database via pointer
	m_pPoiDatabase = pPoiDB;
}

/**
 * Function connects CRoute class to the WP Database
 * @param CWpDatabase* pWpDB : IN pointer to the intended WP Database,which should be connected to the CRoute class
 * @returnvalue void
 */
void CRoute::connectToWpDatabase(CWpDatabase* pWpDB)
{
	//connecting my Route class to the Waypoint Database via pointer
	m_pWpDatabase = pWpDB;
}

/**
 * Function checks for the WP name passed as parameter in the WP Database and add WP to our route from WP Database.
 * In case if WP is not found in the WP Database, ERROR message is printed.
 * Function also checks if Route is connected a valid WP Database
 * @param const string& name : IN name of WP which should be added from WP Database
 * @returnvalue void
 */
void CRoute::addWaypoint(const string& name)
{
	if (m_pWpDatabase != NULL)
	// Route is connected to valid Waypoint Database
	{
		CWaypoint* pWp;
		pWp = m_pWpDatabase->getPointerToWaypoint(name);

		if (pWp != NULL)
		// Waypoint 'name' is found in Waypoint Database
		{
			m_pWaypointList.push_back(pWp);
			m_noWp++;
		}

		else
			// Waypoint 'name' is not found in Waypoint Database
			cout << name << " is not found in WP Database " << endl;
	}

	else
		// Route is not connected to any valid Waypoint Database
		cout << "Route is not connected to any valid Waypoint Database" << endl;
}

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
void CRoute::addPoi(const string& namePoi, const string& afterWp)
{
	if (m_pPoiDatabase != NULL)
	// Route is connected to valid POI Database
	{
		CPOI* pPoi;
		pPoi = m_pPoiDatabase->getPointerToPoi(namePoi);

		if (pPoi != NULL)
		// POI 'namePoi' is found in POI Database
		{
			if (afterWp != "default WP")
			// afterWp parameter is provided by user, add POI after 'afterWp' if found, otherwise, don't add POI to the Route
			{
				list<CWaypoint*>::iterator pos = m_pWaypointList.end(); // pos is initialized with the iterator pointing to the theoretical last element which follows the last element

				for (list<CWaypoint*>::iterator itr = m_pWaypointList.begin();
						itr != m_pWaypointList.end(); ++itr)
				{
					if ((*itr)->getName() == afterWp)
					// 'afterWp' is found in the Route
					{
						CPOI* pPoi = dynamic_cast<CPOI*>(*itr);

						if (pPoi == NULL)
							// Validating afterWp. POI and WP can have same name. 'namePoi' should be added after the last WAYPOINT 'afterWp' in the Route
							pos = itr; // position of "LAST Wp" having the name 'afterWp' is found by iterating through the list
					}
				}

				if (pos != m_pWaypointList.end())
				//  'afterWp' is found in the Route
				{
					m_pWaypointList.insert(++pos, pPoi); // pPoi is inserted at the position ++pos, ie after 'afterWp'
					m_noPoi++;
				}
				else
					//  'afterWp' is not found in the Route
					cout << afterWp
							<< " is not found in the Route. Therefore POI is not added to the Route"
							<< endl;
			}
			else
			// afterWp parameter is not provided by user, add POI to the end of the list
			{
				m_pWaypointList.push_back(pPoi);
				m_noPoi++;
			}
		}
		else
			// POI 'namePoi' is not found in POI Database
			cout << namePoi << " is not found in POI Database" << endl;
	}
	else
		// Route is not connected to any valid POI Database
		cout << "Route is not connected to any valid POI Database" << endl;
}

/**
 * Function calculates and returns the distance to the nearest POI in our route from the Waypoint passed as parameter
 * @param CWaypoint const& wp  : IN const object of type CWaypoint passed via reference, function calculates the distance to the nearest POI in our route from this Waypoint
 * @param CPOI& poi            : OUT copies the details of nearest POI in our Route to this object
 * @return double : the distance to the nearest POI in our route
 */
double CRoute::getDistanceNextPoi(CWaypoint const& wp, CPOI& poi)
{
	double distance = 0.0;
	map<double, CPOI*> mapPoi;

	if (m_noPoi != 0)
	// making sure that atleast one POI is present in our route
	{
		for (list<CWaypoint*>::const_iterator itr = m_pWaypointList.begin();
				itr != m_pWaypointList.end(); ++itr)
		{
			CPOI* pPoi = dynamic_cast<CPOI*>(*itr);

			if (pPoi != NULL)
			// Validating whether *itr is a pointer to POI
			{
				mapPoi.insert(
						pair<double, CPOI*>((*itr)->calculateDistance(wp),
								pPoi));
			}
		}

		map<double, CPOI*>::iterator itr = mapPoi.begin(); // Elements are stored in Map after sorting internally (ascending order of KEY = distance)
		poi = *(itr->second);
		distance = itr->first;
	}
	else
		throw ERROR_NO_POI_IN_ROUTE;

	return distance;

}

/**
 * Function print the Waypoints and POIs in our route
 * @param no parameters
 * @returnvalue void
 */
void CRoute::print()
{

	cout << endl << "********* Our Route has " << m_noWp << " Waypoints and "
			<< m_noPoi << " Point of Interests ********* " << endl;

	/**
	 * =====================================================
	 *  TESTCASE FOR PRINTING ROUTE IN DIFFERENT WAYS
	 * =====================================================
	 */
#if TESTCASE_ROUTE_PRINT == 0
	// normal case . Polymorphism (Late Binding)

	for (list<CWaypoint*>::const_iterator itr = m_pWaypointList.begin();
			itr != m_pWaypointList.end(); ++itr)
	{
		(*itr)->print(MMSS);   // Polymorphism (Late Binding)

	}

#endif

#if TESTCASE_ROUTE_PRINT == 1
	// Invoking operator overloaded function using objects (by derefering the iterator of list containing pointers to CWaypoint)

	cout<<endl;

	for (list<CWaypoint*>::const_iterator itr = m_list_pWaypoint.begin();
			itr != m_list_pWaypoint.end(); ++itr)
	{
		cout<<**itr<<endl;
	}

	/*
	 * Since function is invoked using an object of the class, Early binding occurs., NOT Late binding
	 * List is of type : pointer to CWaypoint
	 * In the case of Early binding, (**itr) everytime operator overloaded function of CWaypoint is called
	 * Data Slicing occurs !
	 */

#endif

#if TESTCASE_ROUTE_PRINT == 2
	// Invoking operator overloaded function using "dynamically casted" objects

	cout<<endl;

	for (list<CWaypoint*>::const_iterator itr = m_list_pWaypoint.begin();
			itr != m_list_pWaypoint.end(); ++itr)
	{
		CPOI* pPoi = dynamic_cast <CPOI*> (*itr);

		if(pPoi != NULL)
		// *itr is a pointer to CPOI
		cout<<*pPoi<<endl;
		else
		// *itr is a pointer to CWaypoint
		cout<<**itr<<endl;
	}

#endif

}

/**
 * operator+ is overloaded for the concatenation of two CRoute objects
 * The two routes are concatenated if both Routes are connected to same WP and POI Databases.
 * If not, then return empty Route
 * @param CRoute const& rop : IN object of type CRoute, right operator
 * @return CRoute : object of CRoute type, contains result of the concatenation
 */
CRoute CRoute::operator+(CRoute const& rop)
{
	CRoute result;

	if ((m_pPoiDatabase == rop.m_pPoiDatabase)
			&& (m_pWpDatabase == rop.m_pWpDatabase))
	// Both routes are connected to same WP Database and same POI Database
	{
		result.m_pWaypointList = m_pWaypointList;        // lop copied to result

		for (list<CWaypoint*>::const_iterator itr =
				rop.m_pWaypointList.begin(); itr != rop.m_pWaypointList.end();
				++itr)
		{
			result.m_pWaypointList.push_back(*itr); // rop is added to the back of result one by one
		}

		result.m_noWp = m_noWp + rop.m_noWp; // updating the number of WP and POI in result
		result.m_noPoi = m_noPoi + rop.m_noPoi;

	}
	else
		// Both routes are not connected to same WP Database and same POI Database
		cout
				<< "ERROR : Both routes must be connected to same WP Database and same POI Database "
				<< endl;

	return result;
}

/**
 * operator+= is overloaded for addition of POI and WP to our Routes.
 * Checks for the string name passed by user in both WP and POI Databases
 * If found in Both Databases, WP is added before POI.
 * Otherwise, simply added to the end of our Route
 * @param const std::string& name : IN name of POI or WP to be added to Route
 * @return CRoute& : returned via reference
 */
CRoute& CRoute::operator+=(const string& name)
{
	unsigned int sizeList = m_pWaypointList.size();

	addWaypoint(name);  // name will be added to our Route if found in WP Database

	if (sizeList != m_pWaypointList.size())
		//  'name' is found in WP Database and added to the List in previous step
		addPoi(name, name); // add POI 'name' if found in POI Database after Wp 'name'
	else
		//  'name' is not found in WP Database, therefore not added to the list
		addPoi(name); // add POI 'name' if found in POI Database to the end of the list

	return *this;
}

/**
 * Function returns a vector containing the Route
 * @param no parameter
 * @return const std::vector<const CWaypoint*> : vector containing Route
 */
const std::vector<const CWaypoint*> CRoute::getRoute()
{

	 vector <const CWaypoint*> m_pWaypointVector;

	for (list<CWaypoint*>::const_iterator itr = m_pWaypointList.begin();
			itr != m_pWaypointList.end(); ++itr)
	{
		m_pWaypointVector.push_back(*itr);

	}

	return m_pWaypointVector;
}


