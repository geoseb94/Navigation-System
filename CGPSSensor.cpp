/***************************************************************************
 *============= Copyright by Darmstadt University of Applied Sciences =======
 ****************************************************************************
 * Filename        : CGPSSENSOR.H
 * Author          : George Sebastian
 * Description     : CGPSSensor class is used to get the current position of user
 ****************************************************************************/
#include <iostream>
using namespace std;

#include "CGPSSensor.h"
#include "CWaypoint.h"

/**
 *Function to get the current position of the user
 *@param no parameters
 *@returnvalue CWaypoint : object of type CWaypoint
 */
CWaypoint CGPSSensor::getCurrentPosition()
{
	double latitude, longitude;

	cout << endl << "GPS Sensor" << endl;
	cout << "   Enter latitude: ";
	cin >> latitude;
	cout << "   Enter longitude: ";
	cin >> longitude;

	CWaypoint currentPosition("Current position", latitude, longitude);
	return currentPosition;
}

/**
 * Constructor of class CGPSSensor
 * @param no parameters
 * @return no value
 */
CGPSSensor::CGPSSensor()
{
}
