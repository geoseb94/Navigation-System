/***************************************************************************
 *============= Copyright by Darmstadt University of Applied Sciences =======
 ****************************************************************************
 * Filename        : CNAVIGATIONSYSTEM.CPP
 * Author          : George Sebastian
 * Description     : Navigation System consists of 4 major components - GPS Sensor, Route, WP Databse &
 *                   POI Database.Databases are populated with POIs and WPs. Then Waypoints and POIs
 *                   are added to our Route from respective Databases. Finally,current position of user
 *                   is taken and the distance to the nearest POI in our Route is estimated
 ****************************************************************************/
#include <iostream>
using namespace std;

#include "CNavigationSystem.h"
#include "CCSV.h"
#include "CJsonPersistence.h"

#define DEGREE 1                           // printing longitude and latitude in Degree format
#define MMSS 2                             // printing longitude and latitude in Minutes,seconds format

#define TESTCASE_POI_DATABASE 0            // Testcase for inserting POIs to POI Database
#define TESTCASE_WP_DATABASE 0             // Testcase for inserting WPs to WP Database
#define TESTCASE_CONNECTING_TO_DATABASE 0  // Testcase for connection between Route and Databases
#define TESTCASE_ADDING_TO_ROUTE 0         // Testcase for adding POIs and WPs to our Route
#define TESTCASE_ADDING_TO_ROUTE_OPERATOROVERLOAD 0 // Testcase for adding POIs and WPs to our Route using Operator Overloaded fundtions
#define TESTCASE_COPYCONSTRUCTOR 0         // Testcase for copy constructor of CRoute
#define TESTCASE_ASSIGNMENT_OPERATOR 0     // Testcase for assignment operator of CRoute
#define TESTCASE_WAYPOINT_PRINT 0          // Testcase for different ways of printing Waypoints
#define TESTCASE_POI_PRINT 0               // Testcase for different ways of printing POIs
#define TESTCASE_ROUTE_CONCATENATION 0     // Testcase for concantenation of two routes
#define TESTCASE_DISTANCE_NEXTPOI 0        // Testcase for testing the distance to Next closest POI in Route
#define TESTCASE_FILE_HANDLING 0           // Testcase for testing File operations
#define TESTCASE_FILE_WRITE 0              // Testcase for testing File write operations
#define TESTCASE_FILE_READ_MERGE 0         // Testcase for testing File Read operations in MERGE mode
#define TESTCASE_FILE_READ_REPLACE 0       // Testcase for testing File Read operations in REPLACE mode

/**
 * Testcase for populating WP and POI Databases
 * @param no parameters
 * @reutrn no value
 */
void CNavigationSystem::TC_createDatabases()
{
	addPoiToPoiDatabase();
	addWpToWpDatabase();
}

/**
 * Testcase for writing the content of Databases to File
 * @param no parameters
 * @reutrn no value
 */
void CNavigationSystem::TC_writeToFile()
{
CCSV csv;
csv.setMediaName("csv");
csv.writeData(m_WpDatabase,m_PoiDatabase);

}

/**
 * Testcase for reading the content of File and updating databases based on the selected mode
 * @param no parameters
 * @reutrn no value
 */
void CNavigationSystem::TC_readFromFile()
{
	CCSV csv;
	csv.setMediaName("csv");

	cout<<"=============================================="<<endl<<"Database before reading from file "<<endl<<"=============================================="<<endl;
	m_WpDatabase.print();
	m_PoiDatabase.print();

#if TESTCASE_FILE_READ_REPLACE == 1

	csv.readData(m_WpDatabase, m_PoiDatabase, CCSV::REPLACE);

#endif

#if TESTCASE_FILE_READ_MERGE == 1

	csv.readData(m_WpDatabase, m_PoiDatabase, CCSV::MERGE);

#endif

	cout<<"=============================================="<<endl<<"Database after reading from file "<<endl<<"=============================================="<<endl;
	m_WpDatabase.print();
	m_PoiDatabase.print();
}

/**
 * Constructor for CNavigationSystem class
 * @param no parameters
 * @reutrn no value
 */
CNavigationSystem::CNavigationSystem()
{

}

/**
 * Function calls all the other private member functions of the Navigation System
 * @param no parameters
 * @returnvalue void
 */
void CNavigationSystem::run()
{
	addPoiToPoiDatabase();
	addWpToWpDatabase();

	enterRoute();
	printRoute();
	//printDistanceCurPosNextPoi();

	CJsonPersistence ob;
   // ob.writeData(m_WpDatabase,m_PoiDatabase);
	ob.readData(m_WpDatabase,m_PoiDatabase,CJsonPersistence::REPLACE);

	/**
	 * ============================================================
	 *  TESTCASE FOR CSV FILE HANDLING : READ AND WRITE OPERATIONS
	 * ===========================================================
	 */

#if TESTCASE_FILE_HANDLING == 1
   // Testing File Handling operations

	TC_createDatabases();            // creates WP and POI Databases

#if TESTCASE_FILE_WRITE == 1
	// Testing writing to File operations

	TC_writeToFile();                // writes to File

#endif

	TC_readFromFile();               // reads from File, mode could be Merge or Replace

#endif

	/**
	 * ==================================================
	 *  TESTCASE FOR COPY CONSTRUCTOR OF CROUTE
	 * ==================================================
	 */
#if TESTCASE_COPYCONSTRUCTOR == 1
	// Testing Copy Constructor

	CRoute route=m_route;

	cout<<endl<<"*********** Testing Copy Constructor *********** "<<endl;
	route.print();

	m_route.addWaypoint("berlin");// adding more WPs to m_route
	m_route.addWaypoint("tokio");

	m_route.print();// m_route got updated
	route.print();// route unchanged

#endif

	/**
	 * ==================================================
	 *  TESTCASE FOR ASSIGNMENT OPERATOR OF CROUTE
	 * ==================================================
	 */
#if TESTCASE_ASSIGNMENT_OPERATOR == 1
	// Testing Assignment Operator

	CRoute route;
	route=m_route;

	cout<<endl<<"*********** Testing Assignment Operator *********** "<<endl;
	route.print();

	m_route.addWaypoint("berlin");// adding more WPs to m_route
	m_route.addWaypoint("tokio");

	m_route.print();// m_route got updated
	route.print();// route unchanged

#endif

	/**
	 * =====================================================
	 *  TESTCASE FOR DIFFERENT PRINTING OPTIONS OF CWaypoint
	 * =====================================================
	 */
#if TESTCASE_WAYPOINT_PRINT == 1
	// Testing different print options for Waypoints

	CWaypoint stuttgart("stuttgart",55.03,6.23);

	stuttgart.print(DEGREE);
	stuttgart.print(MMSS);
	cout<<stuttgart;

#endif

	/**
	 * =====================================================
	 *  TESTCASE FOR DIFFERENT PRINTING OPTIONS OF CPOI
	 * =====================================================
	 */
#if TESTCASE_POI_PRINT == 1
	// Testing different print options for POIs

	CPOI cologneCathedral(TOURISTIC, "Cologne Cathedral",
			"Catholic Cathedral", 50.9, 6.98);

	cologneCathedral.print(DEGREE);
	cologneCathedral.print(MMSS);
	cout<<cologneCathedral;

#endif

	/**
	 * =====================================================
	 *  TESTCASE FOR CONCATENATION OF TWO CRoute CLASSES
	 * =====================================================
	 */
#if TESTCASE_ROUTE_CONCATENATION == 1
	// noraml testcase for concatenation of two routes

	CRoute route1=m_route;// m_route is copied to route1
	CRoute route2;

	route2.connectToPoiDatabase(&m_PoiDatabase);// Both routes are connected to the same WP Database and POI Database
	route2.connectToWpDatabase(&m_WpDatabase);

	route2.addWaypoint("amsterdam");// adding elements to route2
	route2.addWaypoint("darmstadt");

	route2.addPoi("TUM","darmstadt");
	route2.addPoi("Morsh","amsterdam");

	cout<<"Routes before concatenation "<<endl;
	route1.print();
	route2.print();

	CRoute route3;
	route3=route1+route2;// operator overloading

	cout<<"Routes after concatenation "<<endl;
	route1.print();
	route2.print();
	route3.print();

#endif

#if TESTCASE_ROUTE_CONCATENATION == 2
	// Both routes are NOT connected to the same WP Database and POI Database

	CRoute route1=m_route;
	CRoute route2;

	route2.connectToPoiDatabase(&m_PoiDatabase);// Both routes are NOT connected to the same WP Database and POI Database
												// WP Database is NOT connected
	route2.addWaypoint("amsterdam");
	route2.addWaypoint("darmstadt");

	route2.addPoi("TUM","darmstadt");
	route2.addPoi("Morsh","amsterdam");

	cout<<"Routes before concatenation "<<endl;
	route1.print();
	route2.print();

	CRoute route3;
	route3=route1+route2;// operator overloading

	cout<<"Routes after concatenation "<<endl;
	route1.print();
	route2.print();
	route3.print();

#endif
}

/**
 * Function for adding POIs to the POI Database
 * @param no parameters
 * @returnvalue void
 */
void CNavigationSystem::addPoiToPoiDatabase()
{

	CPOI mensaHda(CPOI::RESTAURANT, "Mensa HDA", "The best Mensa in the world", 10,
			20), sitte(CPOI::RESTAURANT, "Sitte", "More expensive but also good", 11,
			22), heising(CPOI::RESTAURANT, "Heising",
			"French Restaurant excellent service", 53, 11), morsh(CPOI::RESTAURANT,
			"Morsh", "French Restaurant", 50.55, 10.4), tum(CPOI::UNIVERSITY, "TUM",
			"best University for Robotics", 48.14, 11.56), hda(CPOI::UNIVERSITY,
			"HDA", "best among Hochschules in Germany", 49.86, 8.6),
			brandenburgGate(CPOI::TOURISTIC, "Brandenburg Gate",
					"Touristic attraction in Berlin", 52.51, 13.77),
			cologneCathedral(CPOI::TOURISTIC, "Cologne Cathedral",
					"Catholic Cathedral", 50.9, 6.98), aral(CPOI::GASSTATION, "Aral",
					"Best  Gas station in Leipzig", 51.3, 12.37), tankstelle(
							CPOI::GASSTATION, "Total Tankstelle",
					"open untill 21 Uhr in Dresden", 51.05, 13.67), berlin(
							CPOI::TOURISTIC, "berlin", "Capital City", 52.5200, 13.4050);

	/**
	 * =====================================================
	 *  TESTCASE FOR INSERTING POIs TO POI DATABASE
	 * =====================================================
	 */
#if TESTCASE_POI_DATABASE == 0
	//normal case

	m_PoiDatabase.addPoi(mensaHda);
	m_PoiDatabase.addPoi(sitte);
	m_PoiDatabase.addPoi(heising);
	m_PoiDatabase.addPoi(morsh);
	m_PoiDatabase.addPoi(tum);
	m_PoiDatabase.addPoi(hda);
	m_PoiDatabase.addPoi(brandenburgGate);
	m_PoiDatabase.addPoi(cologneCathedral);
	m_PoiDatabase.addPoi(aral);
	m_PoiDatabase.addPoi(tankstelle);
	m_PoiDatabase.addPoi(berlin); // berlin is added to both WP Database and POI Database

#endif

#if TESTCASE_POI_DATABASE == 2
	// Trying to add duplicate POIs to POI Database

	m_PoiDatabase.addPoi(mensaHda);
	m_PoiDatabase.addPoi(mensaHda);// duplicate 'mensaHDA' - handled by STL Map
	m_PoiDatabase.addPoi(sitte);
	m_PoiDatabase.addPoi(sitte);// duplicate 'sitte' - handled by STL Map
	m_PoiDatabase.addPoi(heising);
	m_PoiDatabase.addPoi(morsh);
	m_PoiDatabase.addPoi(tum);
	m_PoiDatabase.addPoi(hda);
	m_PoiDatabase.addPoi(brandenburgGate);
	m_PoiDatabase.addPoi(cologneCathedral);
	m_PoiDatabase.addPoi(aral);
	m_PoiDatabase.addPoi(tankstelle);

	m_PoiDatabase.print();

#endif

#if TESTCASE_POI_DATABASE == 3
	// Trying to add corrupted data to POI Database

	CPOI kirche(TOURISTIC,"kirche","Catholic church",330,400);// corrupted longitude and latitude value - for testing purpose

	m_PoiDatabase.addPoi(mensaHda);
	m_PoiDatabase.addPoi(sitte);
	m_PoiDatabase.addPoi(heising);
	m_PoiDatabase.addPoi(morsh);
	m_PoiDatabase.addPoi(tum);
	m_PoiDatabase.addPoi(hda);
	m_PoiDatabase.addPoi(kirche);//corrupted longitude and latitude value - for testing purpose

	m_PoiDatabase.print();

#endif

#if TESTCASE_POI_DATABASE == 4
	// with empty POI Database

	m_PoiDatabase.print();

#endif

}

/**
 * Function for adding WPs to the WP Database
 * @param no parameters
 * @returnvalue void
 */
void CNavigationSystem::addWpToWpDatabase()
{
	CWaypoint amsterdam("amsterdam", 52.3680, 4.9036), darmstadt("darmstadt",
			49.8728, 8.6512), berlin("berlin", 52.5200, 13.4050), tokio("tokio",
			35.6895, 139.6917);

	/**
	 * =====================================================
	 *  TESTCASE FOR INSERTING WPs TO WP DATABASE
	 * =====================================================
	 */
#if TESTCASE_WP_DATABASE == 0
	// normal case

	m_WpDatabase.addWaypoint(amsterdam);
	m_WpDatabase.addWaypoint(berlin); // berlin is added to both WP Database and POI Database
	m_WpDatabase.addWaypoint(tokio);
	m_WpDatabase.addWaypoint(darmstadt);

#endif

#if TESTCASE_WP_DATABASE == 1
	// Trying to add duplicate WPs to WP Database

	m_WpDatabase.addWaypoint(amsterdam);
	m_WpDatabase.addWaypoint(berlin);
	m_WpDatabase.addWaypoint(tokio);
	m_WpDatabase.addWaypoint(tokio);// duplicate 'tokio' - handled by STL Map

	m_WpDatabase.print();

#endif

#if TESTCASE_WP_DATABASE == 2
	// Trying to add corrupted data to Wp Database

	CWaypoint frankfurt("frankfurt",330,500);// corrupted longitude and latitude value - for testing purpose

	m_WpDatabase.addWaypoint(amsterdam);
	m_WpDatabase.addWaypoint(berlin);
	m_WpDatabase.addWaypoint(tokio);
	m_WpDatabase.addWaypoint(darmstadt);
	m_WpDatabase.addWaypoint(frankfurt);// corrupted longitude and latitude value - for testing purpose

	m_WpDatabase.print();

#endif

#if TESTCASE_WP_DATABASE == 3
	// with empty Wp Database

	m_WpDatabase.print();

#endif

}

/**
 * Function for adding Waypoints and POIs to our route
 * @param no parameters
 * @returnvalue void
 */
void CNavigationSystem::enterRoute()
{

	/**
	 * =========================================================================
	 *  TESTCASE FOR ESTABLISHING CONNECTION BETWEEN CRoute CLASS AND DATABASES
	 * =========================================================================
	 */
#if TESTCASE_CONNECTING_TO_DATABASE == 0
	//normal case

	m_route.connectToPoiDatabase(&m_PoiDatabase);// connecting our Route to POI Database
	m_route.connectToWpDatabase(&m_WpDatabase);// connecting our Route to Waypoint Database

#endif

#if TESTCASE_CONNECTING_TO_DATABASE == 1
	// Not connecting the route to POI Database

	m_route.connectToWpDatabase(&m_WpDatabase);// connecting our Route to Waypoint Database

#endif

#if TESTCASE_CONNECTING_TO_DATABASE == 2
	// Not connecting the route to WP Database

	m_route.connectToPoiDatabase(&m_PoiDatabase); // connecting our Route to POI Database

#endif

#if TESTCASE_CONNECTING_TO_DATABASE == 3
	// Not connected route to both WP and POI Database

#endif

	/**
	 * =====================================================
	 *  TESTCASE FOR ADDING POIs AND WPs TO OUR ROUTE
	 * =====================================================
	 */
#if TESTCASE_ADDING_TO_ROUTE == 0
	// normal case

	m_route.addWaypoint("darmstadt");           // adding WPs
	m_route.addWaypoint("berlin");
	m_route.addWaypoint("amsterdam");
	m_route.addWaypoint("tokio");
	m_route.addWaypoint("berlin");
	m_route.addWaypoint("tokio");

	m_route.addPoi("Mensa HDA", "darmstadt");   // adding POIs
	m_route.addPoi("Sitte", "berlin");

#endif

#if TESTCASE_ADDING_TO_ROUTE == 1
	// Trying to add invalid WP "bonn", which is not present in WP Database

	m_route.addWaypoint("darmstadt");
	m_route.addWaypoint("berlin");
	m_route.addWaypoint("amsterdam");
	m_route.addWaypoint("tokio");
	m_route.addWaypoint("bonn");// bonn is not present in WP Database

	m_route.addPoi("Mensa HDA","darmstadt");
	m_route.addPoi("Sitte","berlin");

#endif

#if TESTCASE_ADDING_TO_ROUTE == 2
	// Trying to add invalid POI "Hotel Taj", which is not present in POI Database

	m_route.addWaypoint("darmstadt");
	m_route.addWaypoint("berlin");
	m_route.addWaypoint("amsterdam");
	m_route.addWaypoint("tokio");

	m_route.addPoi("Mensa HDA","darmstadt");
	m_route.addPoi("Sitte","berlin");
	m_route.addPoi("Hotel Taj","berlin");// Hotel Taj is not present in POI Database

#endif

#if TESTCASE_ADDING_TO_ROUTE == 3
	// Trying to add a valid POI "heising" afterWp "tokio", which is not a part of the Route

	m_route.addWaypoint("darmstadt");
	m_route.addWaypoint("berlin");
	m_route.addWaypoint("amsterdam");

	m_route.addPoi("Mensa HDA","darmstadt");
	m_route.addPoi("Sitte","berlin");
	m_route.addPoi("heising","tokio");// Wp "tokio" is not a part of the Route

#endif

#if TESTCASE_ADDING_TO_ROUTE == 4
	// Trying to add valid POI "Sitte" afterWp "berlin", "berlin" is present in our Route more than once

	m_route.addWaypoint("darmstadt");
	m_route.addWaypoint("berlin");// Wp "berlin" is added to the Route
	m_route.addWaypoint("amsterdam");
	m_route.addWaypoint("tokio");
	m_route.addWaypoint("berlin");// Wp "berlin" is again added to the Route

	m_route.addPoi("Mensa HDA","darmstadt");
	m_route.addPoi("Sitte","berlin");// Trying to add valid POI "Sitte" afterWp "berlin", "berlin" is present in our Route more than once

#endif

#if TESTCASE_ADDING_TO_ROUTE == 5
	// Trying to add more test cases for previous Testcase scenario

	m_route.addWaypoint("darmstadt");
	m_route.addWaypoint("berlin");
	m_route.addWaypoint("amsterdam");
	m_route.addWaypoint("tokio");// Wp "tokio" is added to the Route only once
	m_route.addWaypoint("berlin");// Wp "berlin" is again added to the Route
	m_route.addWaypoint("darmstadt");// Wp "darmstadt" is again added to the Route

	m_route.addPoi("Mensa HDA","darmstadt");// Trying to add valid POI "Mensa HDA" after the LAST Wp in the Route having the name "darmstadt"
	m_route.addPoi("Sitte","berlin");// Trying to add valid POI "Sitte" after the LAST Wp in the Route having the name "berlin"
	m_route.addPoi("heising","tokio");// Trying to add valid POI "heising" after the LAST Wp in the Route having the name "tokio"

#endif

#if TESTCASE_ADDING_TO_ROUTE == 6
	// POI and WP can have same name in the Route. 'namePoi' should be added after the last WAYPOINT 'afterWp' in the Route

	m_route.addWaypoint("darmstadt");
	m_route.addWaypoint("berlin");// "berlin" is a valid WP here
	m_route.addWaypoint("amsterdam");
	m_route.addWaypoint("berlin");// "berlin" is a valid WP here, added 2 times to the Route
	m_route.addWaypoint("tokio");

	m_route.addPoi("berlin","tokio");// "berlin" is a valid POI here
	m_route.addPoi("Sitte","berlin");// "Sitte" is a valid POI, which should be added after the Last WAYPOINT "berlin".. Validation for "afterWp" is required

#endif

#if TESTCASE_ADDING_TO_ROUTE == 7
	// empty Route

#endif

	/**
	 * ===================================================================================
	 *  TESTCASE FOR ADDING POIs AND WPs TO OUR ROUTE USING OPERATOR OVERLOADED FUNCTIONS
	 * ===================================================================================
	 */
#if TESTCASE_ADDING_TO_ROUTE_OPERATOROVERLOAD == 1
	// normal case

	m_route+="darmstadt";
	m_route+="berlin";
	m_route+="amsterdam";

	m_route+="Mensa HDA";
	m_route+="Sitte";

#endif

#if TESTCASE_ADDING_TO_ROUTE_OPERATOROVERLOAD == 2
	// Trying to add invalid WP "bonn", which is not present in WP Database

	m_route+="darmstadt";
	m_route+="amsterdam";
	m_route+="bonn";// bonn is not present in WP Database

	m_route+="Mensa HDA";
	m_route+="Sitte";

#endif

#if TESTCASE_ADDING_TO_ROUTE_OPERATOROVERLOAD == 3
	// Trying to add invalid POI "Hotel Taj", which is not present in POI Database

	m_route+="darmstadt";
	m_route+="amsterdam";

	m_route+="Mensa HDA";
	m_route+="Hotel Taj";// Hotel Taj is not present in POI Database

#endif

#if TESTCASE_ADDING_TO_ROUTE_OPERATOROVERLOAD == 4
	// "berlin" is present in both WP Database and POI Database

	m_route+="darmstadt";
	m_route+="amsterdam";
	m_route+="berlin";// "berlin" is present in both WP Database and POI Database

	m_route+="Mensa HDA";
	m_route+="Hotel Taj";

#endif

#if TESTCASE_ADDING_TO_ROUTE_OPERATOROVERLOAD == 5
	// empty Route

#endif

}

/**
 * Function detects current position of the user and prints the distance from
 * current position to the nearest POI in our route
 * @param no parameters
 * @returnvalue void
 */
void CNavigationSystem::printDistanceCurPosNextPoi()
{
	/**
	 * ===================================================================================
	 *  TESTCASE FOR CALCULATING DISTANCE TO NEXT CLOSEST POI IN OUR ROUTE
	 * ===================================================================================
	 */
#if TESTCASE_DISTANCE_NEXTPOI == 1

	// Give invalid latitude and longitude as current location
	// Give latitude = 0 and longitude =0 as current location

#endif

	CWaypoint currentPosition = m_GPSSensor.getCurrentPosition(); // getting current position of user

	CPOI poi;
	double distance;

	if (currentPosition.getName() != "default city")
	// In order to differentiate between valid latitude & longitude = 0 position case and invalid latitude/longitude input case
	{
		try
		{
			distance = m_route.getDistanceNextPoi(currentPosition, poi);
			cout << "Distance to next POI = "
					<< m_route.getDistanceNextPoi(currentPosition, poi) << endl;
			cout << poi;

		} catch (...)
		// if no POI was added to our Route. Exception handling
		{
			cout << "No POI is added to our Route" << endl;
		}
	}
	else
		// in case latitude/longitude entered is out of range
		cout << endl
				<< "GPS Sensor reporting invalid Latitude/Longitude,current position not found!";

}

/**
 * Function for printing our route
 * @param no parameters
 * @returnvalue void
 */
void CNavigationSystem::printRoute()
{
	m_route.print();

}
