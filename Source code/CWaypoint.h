/***************************************************************************
 *============= Copyright by Darmstadt University of Applied Sciences =======
 ****************************************************************************
 * Filename        : CWAYPOINT.H
 * Author          : George Sebastian
 * Description     : CWaypoint Class stores a geodetic position and provides some simple operations
 *                   for geodetic data
 ****************************************************************************/
#ifndef CWAYPOINT_H
#define CWAYPOINT_H

#include <string>

class CWaypoint
{
protected:
	std::string m_name;
	double m_latitude;
	double m_longitude;
public:

	/**
	 * Friend function to CWaypoint Class
	 * operator<< is overloaded for printing all attributes of CWaypoint object
	 * @param ostream& out        : OUT passed via reference
	 * @param const CWaypoint& Wp : IN CWaypoint object to be printed
	 * @returnvalue ostream&
	 */
	friend std::ostream& operator<<(std::ostream& out,const CWaypoint& Wp);

	/**
	 * Default constructor of Class CWaypoint
	 * @param string name      : IN name of the Waypoint
	 * @param double latitude  : IN latitude of the Waypoint
	 * @param double longitude : IN longitude of the Waypoint
	 * @returnvalue no value
	 */
	CWaypoint(std::string name = "default city", double latitude = 0.0,
			double longitude = 0.0);

	/**
	 * Function checks the validity of parameter values and writes the correct values to the attributes.
	 * In case of invalid parameter values, a '0' shall be written to the attributes.
	 *
	 * @param string name      : IN name of the Waypoint
	 * @param double latitude  : IN latitude of the Waypoint
	 * @param double longitude : IN longitude of the Waypoint
	 * @returnvalue void
	 */
	void set(std::string name, double latitude, double longitude);

	/**
	 * Function returns the name of the Waypoint
	 * @param no parameters
	 * @returnvalue string : the name of the Waypoint
	 */
	std::string getName() const;

	/**
	 * Function returns the latitude of the Waypoint
	 * @param no parameters
	 * @returnvalue double : the latitude of the Waypoint
	 */
	double getLatitude()const;

	/**
	 * Function returns the longitude of the Waypoint
	 * @param no parameters
	 * @returnvalue double : the longitude of the Waypoint
	 */
	double getLongitude()const;

	/**
	 * Function to get the values of all the attributes by reference
	 * @param string& name       : OUT name of the Waypoint
	 * @param double & latitude  : OUT latitude of the Waypoint
	 * @param double & longitude : OUT longitude of the Waypoint
	 * @returnvalue void
	 */
	void getAllDataByReference(std::string& name, double & latitude,
			double & longitude)const;

	/**
	 * Function calculates the distance between two Waypoints using the formula and returns the distance
	 * @param const CWaypoint& wp : IN one of the Waypoint object passed via reference (Other Waypoint object is the object invoking this member function)
	 * @returnvalue double : the calculated distance between the two Waypoints
	 */
	double calculateDistance(const CWaypoint& wp);

	/**
	 * Virtual Function prints the latitude and longitude of the Waypoint in decimal or deg mm ss format depending on the value of format being passed
	 * @param int format : IN format in which latitude and longitude needs to be printed. '0' - decimal format, '1' - deg mm ss format
	 * @returnvalue void
	 */
	virtual void print(int format);

	/**
	 * Virtual Destructor for CWaypoint class
	 * @param no parameters
	 * @returnvalue void
	 */
	virtual ~CWaypoint();


	/**
	 * Function to transform and get the latitude from decimal values to deg mm ss format
	 * @param int & deg   : OUT degree part of the latitude
	 * @param int & mm    : OUT minute part of the latitude
	 * @param double & ss : OUT second part of the latitude
	 * @returnvalue void
	 */
	void transformLatitude2degmmss(int & deg, int & mm, double & ss);

	/**
	 * Function to transform and get the longitude from decimal values to deg mm ss format
	 * @param int & deg   : OUT degree part of the longitude
	 * @param int & mm    : OUT minute part of the longitude
	 * @param double & ss : OUT second part of the longitude
	 * @returnvalue void
	 */
	void transformLongitude2degmmss(int & deg, int & mm, double & ss);
};

/********************
 **  CLASS END
 *********************/

#endif /* CWAYPOINT_H */
