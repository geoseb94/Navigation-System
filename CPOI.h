/***************************************************************************
 *============= Copyright by Darmstadt University of Applied Sciences =======
 ****************************************************************************
 * Filename        : CPOI.H
 * Author          : George Sebastian
 * Description     : CPOI class stores a POI and provides some simple operations
 *                   for POI
 ****************************************************************************/
#ifndef CPOI_H
#define CPOI_H

#include "CWaypoint.h"

class CPOI: public CWaypoint
{

public:

	/*
	 * enum for different types of POI
	 */
	typedef enum poi
	{
		RESTAURANT, TOURISTIC, GASSTATION, UNIVERSITY
	} t_poi;

	/**
	 * Friend function to CPOI class
	 * operator<< is overloaded for printing all attributes of CPOI object
	 * @param ostream& out        : OUT passed via reference
	 * @param const CWaypoint& Wp : IN CPOI object to be printed
	 * @returnvalue ostream&
	 */
	friend std::ostream& operator<<(std::ostream& out, const CPOI& poi); // friend function or trans function to be changed to public?????????????????

	/**
	 * Default constructor of Class CPOI
	 * @param t_poi type         : IN type of the POI
	 * @param string name        : IN name of the POI
	 * @param string description : IN description about the POI
	 * @param double latitude    : IN latitude of the POI
	 * @param double longitude   : IN longitude of the POI
	 * @return no value
	 */
	CPOI(t_poi type = RESTAURANT, std::string name = "default city",
			std::string description = "default description", double latitude = 0.0,
			double longitude = 0.0);

	/**
	 * Function prints the latitude and longitude of POI in deg mm ss format along with the type and description of POI
	 * @param no parameters
	 * @returnvalue void
	 */
	void print(int format);

	/**
	 * Function to get the values of all the attributes by reference
	 * @param string& name       : OUT name of the POI
	 * @param double & latitude  : OUT latitude of the POI
	 * @param double & longitude : OUT longitude of the POI
	 * @param t_poi& type        : OUT type of the POI
	 * @string& description      : OUT description about the POI
	 * @returnvalue void
	 */
	void getAllDataByReference(std::string& name, double & latitude,
			double & longitude, t_poi& type, std::string& description)const;

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
	void set(t_poi type, std::string name, std::string description, double latitude,
			double longitude);

	/**
	 * Function returns type of Poi in string form
	 * @returnvalue string type of poi
	 */
	std::string getType()const;

	/**
	 * Function returns description of Poi in string form
	 * @returnvalue string description of poi
	 */
	std::string getPoiDescription()const;

private:

	t_poi m_type;
	std::string m_description;
};
/********************
 **  CLASS END
 *********************/
#endif /* CPOI_H */
