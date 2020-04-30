/***************************************************************************
 *============= Copyright by Darmstadt University of Applied Sciences =======
 ****************************************************************************
 * Filename        : CNAVIGATIONSYSTEM.H
 * Author          : George Sebastian
 * Description     : Navigation System consists of 4 major components - GPS Sensor, Route, WP Databse &
 *                   POI Database.Databases are populated with POIs and WPs. Then Waypoints and POIs
 *                   are added to our Route from respective Databases. Finally,current position of user
 *                   is taken and the distance to the nearest POI in our Route is estimated
 ****************************************************************************/
#ifndef CNAVIGATIONSYSTEM_H
#define CNAVIGATIONSYSTEM_H

#include "CGPSSensor.h"
#include "CRoute.h"
#include "CPoiDatabase.h"
#include "CWpDatabase.h"

class CNavigationSystem
{
public:

	/**
	 * Constructor for CNavigationSystem class
	 * @param no parameters
	 * @reutrn no value
	 */
	CNavigationSystem();

	/**
     * Function calls all the other private member functions of the Navigation System
	 * @param no parameters
	 * @returnvalue void
	 */
	void run();

private:
	/**
	 * Function for adding Waypoints and POIs to our route
	 * @param no parameters
	 * @returnvalue void
	 */
	void enterRoute();

	/**
	 * Function for adding POIs to the POI Database
	 * @param no parameters
	 * @returnvalue void
	 */
	void addPoiToPoiDatabase();

	/**
	 * Function for adding WPs to the WP Database
	 * @param no parameters
	 * @returnvalue void
	 */
	void addWpToWpDatabase();

	/**
	 * Testcase for populating WP and POI Databases
	 * @param no parameters
	 * @reutrn no value
	 */
	void TC_createDatabases();

	/**
	 * Testcase for writing the content of Databases to File
	 * @param no parameters
	 * @reutrn no value
	 */
	void TC_writeToFile();

	/**
	 * Testcase for reading the content of File and updating databases based on the selected mode
	 * @param no parameters
	 * @reutrn no value
	 */
	void TC_readFromFile();

private:
	/**
	 * Function for printing our route
	 * @param no parameters
	 * @returnvalue void
	 */
	void printRoute();

	/*
	 * Function for printing the distance from current position to the nearest POI in our route
	 * @param no parameters
	 * @returnvalue void
	 */
	void printDistanceCurPosNextPoi();

	CGPSSensor m_GPSSensor;
	CRoute m_route;
	CPoiDatabase m_PoiDatabase;
	CWpDatabase m_WpDatabase;

};
/********************
 **  CLASS END
 *********************/
#endif /* CNAVIGATIONSYSTEM_H */
