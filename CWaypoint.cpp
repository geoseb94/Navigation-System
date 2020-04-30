/***************************************************************************
 *============= Copyright by Darmstadt University of Applied Sciences =======
 ****************************************************************************
 * Filename        : CWAYPOINT.CPP
 * Author          : George Sebastian
 * Description     : CWaypoint Class stores a geodetic position and provides some simple operations
 *                   for geodetic data
 ****************************************************************************/
#include <iostream>
#include <math.h>

using namespace std;

#include "CWaypoint.h"

#define RADIUS 6378.17       // Value of Radius of Earth
#define PI 3.14159           // Value of Pi
#define DEGREE 1
#define MMSS 2
//#define SHOWADDRESS   // define SHOWADDRESS only if address needs to be printed inside default constructor

/**
 * Function checks the validity of parameter values and writes the correct values to the attributes.
 * In case of invalid parameter values, a '0' shall be written to the attributes.
 * @param string name      : IN name of the Waypoint
 * @param double latitude  : IN latitude of the Waypoint
 * @param double longitude : IN longitude of the Waypoint
 * @returnvalue void
 */
void CWaypoint::set(string name, double latitude, double longitude)
{
	// checking the validity of parameter values and writing the correct values to the attributes.
	if (latitude >= -90 && latitude <= 90 && longitude >= -180
			&& longitude <= 180)
	{
		m_name = name;
		m_latitude = latitude;
		m_longitude = longitude;
	}
	//In case of invalid parameter values, writing '0' to the attributes
	else
	{
		m_latitude = 0;
		m_longitude = 0;
		m_name = "default city";
	}
}

/**
 * Function returns the name of the Waypoint
 * @param no parameters
 * @returnvalue string : the name of the Waypoint
 */
string CWaypoint::getName()const
{
	return m_name;
}

/**
 * Function returns the latitude of the Waypoint
 * @param no parameters
 * @returnvalue double : the latitude of the Waypoint
 */
double CWaypoint::getLatitude()const
{
	return m_latitude;
}

/**
 * Function returns the longitude of the Waypoint
 * @param no parameters
 * @returnvalue double : the longitude of the Waypoint
 */
double CWaypoint::getLongitude()const
{
	return m_longitude;
}

/**
 * Function to get the values of all the attributes by reference
 * @param string& name       : OUT name of the Waypoint
 * @param double & latitude  : OUT latitude of the Waypoint
 * @param double & longitude : OUT longitude of the Waypoint
 * @returnvalue void
 */
void CWaypoint::getAllDataByReference(string& name, double & latitude,
		double & longitude)const
{

#ifdef SHOWADDRESS
	cout<<"Address and values of parameters of member function : name "<<name<<" "<<&name<<", latitude "<<latitude<<" "<<&latitude<<", longitude "<<longitude<<" "<<&longitude<<endl;
#endif

	name = getName();
	latitude = getLatitude();
	longitude = getLongitude();
}

/**
 * Function to transform and get the longitude from decimal values to deg mm ss format
 * @param int & deg   : OUT degree part of the longitude
 * @param int & mm    : OUT minute part of the longitude
 * @param double & ss : OUT second part of the longitude
 * @returnvalue void
 */
void CWaypoint::transformLongitude2degmmss(int & deg, int & mm, double & ss)
{
	double integerPart, fractionPart;
	// modf() to extract integer and fraction part
	// 60 minutes or 3600 seconds = 1 degree
	fractionPart = modf(m_longitude, &integerPart);
	deg = (int) integerPart;
	mm = (int) (fractionPart * 60);

	fractionPart = modf(fractionPart * 60, &integerPart);
	ss = fractionPart * 60;
}

/**
 * Function to transform and get the latitude from decimal values to deg mm ss format
 * @param int & deg   : OUT degree part of the latitude
 * @param int & mm    : OUT minute part of the latitude
 * @param double & ss : OUT second part of the latitude
 * @returnvalue void
 */
void CWaypoint::transformLatitude2degmmss(int & deg, int & mm, double & ss)
{
	double integerPart, fractionPart;
	// modf() to extract integer and fraction part
	// 60 minutes or 3600 seconds = 1 degree
	fractionPart = modf(m_latitude, &integerPart);
	deg = (int) integerPart;
	mm = (int) (fractionPart * 60);

	fractionPart = modf(fractionPart * 60, &integerPart);
	ss = fractionPart * 60;
}

/**
 * Function calculates the distance between two Waypoints using the formula and returns the distance
 * @param const CWaypoint& wp : IN one of the Waypoint object passed via reference (Other Waypoint object is the object invoking this member function)
 * @return double : the calculated distance between the two Waypoints
 */
double CWaypoint::calculateDistance(const CWaypoint& wp)
{
	return (RADIUS
			* acos(
					sin(this->m_latitude * PI / 180)
							* sin(wp.m_latitude * PI / 180)
							+ cos(this->m_latitude * PI / 180)
									* cos(wp.m_latitude * PI / 180)
									* cos(
											(wp.m_longitude - this->m_longitude)
													* PI / 180)));
}

/**
 * Function prints the latitude and longitude of the Waypoint in decimal or deg mm ss format depending on the value of format being passed
 * @param int format : IN format in which latitude and longitude needs to be printed. '0' - decimal format, '1' - deg mm ss format
 * @returnvalue void
 */
void CWaypoint::print(int format)
{   // latitude and longitude printed in decimal format
	if (format == DEGREE)
		cout << m_name << " on latitude = " << m_latitude << " and longitude = "
				<< m_longitude << endl;
	// latitude and longitude printed in deg mm ss format
	else
	{
		cout<<*this;

	}
}

/**
 * Friend function to CWaypoint Class
 * operator<< is overloaded for printing all attributes of CWaypoint object
 * @param ostream& out        : OUT passed via reference
 * @param const CWaypoint& Wp : IN CWaypoint object to be printed
 * @returnvalue ostream&
 */
ostream& operator<<(ostream& out, const CWaypoint& Wp)
{
	CWaypoint lWp=Wp;

	int degLongitude = 0, mmLongitude = 0, degLatitude = 0, mmLatitude = 0;
	double ssLongitude = 0.0,ssLatitude = 0.0;

	// function called to transform longitude from decimal to deg mm ss format
	lWp.transformLongitude2degmmss(degLongitude, mmLongitude, ssLongitude);

	// function called to transform latitude from decimal to deg mm ss format
	lWp.transformLatitude2degmmss(degLatitude, mmLatitude, ssLatitude);

	out << lWp.getName() << " on latitude = " << degLatitude << "deg "
			<< mmLatitude << "mm " << ssLatitude << "ss "
			<< " and longitude = " << degLongitude << "deg " << mmLongitude
			<< "mm " << ssLongitude << "ss " << endl;

	return out;
}

/**
 * Default constructor of Class CWaypoint
 * @param string name      : IN name of the Waypoint
 * @param double latitude  : IN latitude of the Waypoint
 * @param double longitude : IN longitude of the Waypoint
 * @returnvalue no value
 */
CWaypoint::CWaypoint(string name, double latitude, double longitude)
{
	set(name, latitude, longitude);

#ifdef SHOWADDRESS

	cout<<"Address of object "<<this<<endl;
	cout<<"Attribute values are : "<<m_name<<" latitude "<<m_latitude<<" and longitude "<<m_longitude<<endl;
	cout<<"Address of attributes are : "<<&m_name<<" "<<&m_latitude<<" "<<&m_longitude<<endl;
#endif

	/*
	 *  Address is allocated to attributes in the same order in which they are defined inside the Class
	 *  Address of object = Address of 1st attribute
	 *  Size of double = 8 Bytes
	 *  Size of string = 32 Bytes  // sizeof() operator
	 */
}

/**
 * Virtual Destructor for CWaypoint class
 * @param no parameters
 * @returnvalue void
 */
CWaypoint:: ~CWaypoint()
{
}


