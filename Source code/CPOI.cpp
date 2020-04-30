/***************************************************************************
 *============= Copyright by Darmstadt University of Applied Sciences =======
 ****************************************************************************
 * Filename        : CPOI.CPP
 * Author          : George Sebastian
 * Description     : CPOI class stores a POI and provides some simple operations
 *                   for POI
 ****************************************************************************/
#include <iostream>
#include <math.h>
using namespace std;

#include "CPOI.h"

#define DEGREE 1 // printing longitude and latitude in Degree format
#define MMSS 2   // printing longitude and latitude in Minutes,seconds format

/**
 * Function takes poi type from user and converts it to string for the ease of printing
 * @param CPOI::t_poi type : IN type of the POI
 * @returnvalue string type of poi
 */
string getPoiType(CPOI::t_poi type);

/**
 * Function returns type of Poi in string form
 * @returnvalue string type of poi
 */
string CPOI::getType()const
{
	return getPoiType(m_type);
}

/**
 * Function returns description of Poi in string form
 * @returnvalue string description of poi
 */
string CPOI::getPoiDescription()const
{
	return m_description;
}

/**
 * Function to get the values of all the attributes by reference
 * @param string& name       : OUT name of the POI
 * @param double & latitude  : OUT latitude of the POI
 * @param double & longitude : OUT longitude of the POI
 * @param t_poi& type        : OUT type of the POI
 * @string& description      : OUT description about the POI
 * @returnvalue void
 */
void CPOI::getAllDataByReference(string& name, double & latitude,
		double & longitude, t_poi& type, string& description)const
{
	name = getName();
	latitude = getLatitude();
	longitude = getLongitude();
	type = m_type;
	description = m_description;
}

/**
 * Function prints the latitude and longitude of POI in deg mm ss format or decimal format along with the type and description of POI
 * @param no parameters
 * @returnvalue void
 */
void CPOI::print(int format)
{
	if (format == DEGREE)
	{

	string type=getPoiType(m_type);

	cout << endl << "Point of Interest " << endl << "=================" << endl
			<< "of type " << type << " : " <<m_description << endl;

	CWaypoint::print(DEGREE);

	}
	else
		cout<<*this;
}

/**
 * Friend function to CPOI class
 * operator<< is overloaded for printing all attributes of CPOI object
 * @param ostream& out        : OUT passed via reference
 * @param const CWaypoint& Wp : IN CPOI object to be printed
 * @returnvalue ostream&
 */
ostream& operator<<(ostream& out, const CPOI& poi)  // friend function & transformLongitude2degmmss() changed to public from private
{
	CPOI lpoi=poi;

	string type=getPoiType(lpoi.m_type);

	out << "Point of Interest " << endl << "=================" << endl
			<< "of type " << type << " : " <<lpoi.m_description << endl;

	int degLongitude = 0, mmLongitude = 0, degLatitude = 0, mmLatitude = 0;
	double ssLongitude = 0.0,ssLatitude = 0.0;

	// function called to transform longitude from decimal to deg mm ss format
	lpoi.transformLongitude2degmmss(degLongitude, mmLongitude, ssLongitude);


	// function called to transform latitude from decimal to deg mm ss format
	lpoi.transformLatitude2degmmss(degLatitude, mmLatitude, ssLatitude);

	out << lpoi.getName() << " on latitude = " << degLatitude << "deg "
			<< mmLatitude << "mm " << ssLatitude << "ss "
			<< " and longitude = " << degLongitude << "deg " << mmLongitude
			<< "mm " << ssLongitude << "ss " << endl;

return out;
}

/**
 * Function checks the validity of parameter values and writes the correct values to the attributes.
 * In case of invalid parameter values, a '0' shall be written to the attributes.
 * Overrides the inherited function
 * @param t_poi type         : IN type of the POI
 * @param string name        : IN name of the POI
 * @param string description : IN description about the POI
 * @param double latitude    : IN latitude of the POI
 * @param double longitude   : IN longitude of the POI
 * @returnvalue void
 */
void CPOI::set(t_poi type, string name, string description, double latitude,
		double longitude)
{
	// checking the validity of parameter values and writing the correct values to the attributes.
	if (latitude >= -90 && latitude <= 90 && longitude >= -180
			&& longitude <= 180)
	{
		m_name = name;
		m_latitude = latitude;
		m_longitude = longitude;
		m_type = type;
		m_description = description;
	}
	//In case of invalid parameter values, writing '0' to the attributes
	else
	{
		m_latitude = 0;
		m_longitude = 0;
		m_name = "default city";
		m_type = RESTAURANT;
		m_description = "default description";
	}
}

/**
 * Default constructor of Class CPOI
 * @param t_poi type         : IN type of the POI
 * @param string name        : IN name of the POI
 * @param string description : IN description about the POI
 * @param double latitude    : IN latitude of the POI
 * @param double longitude   : IN longitude of the POI
 * @return no value
 */
CPOI::CPOI(t_poi type, string name, string description, double latitude,
		double longitude) :
		CWaypoint(name, latitude, longitude), m_type(type), m_description(
				description)
{

}

/**
 * Function takes poi type from user and converts it to string for the ease of printing
 * @param CPOI::t_poi type : IN type of the POI
 * @returnvalue string type of poi
 */
string getPoiType(CPOI::t_poi type)
{
	switch(type)
	{
	case CPOI::RESTAURANT : return "RESTAURANT";
	break;
	case CPOI::TOURISTIC : return "TOURISTIC";
	break;
	case CPOI::GASSTATION : return"GASSTATION";
	break;
	case CPOI::UNIVERSITY : return"UNIVERSITY";
	break;
	default : return "default type";
	}

}
