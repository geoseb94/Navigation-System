/***************************************************************************
 *============= Copyright by Darmstadt University of Applied Sciences =======
 ****************************************************************************
 * Filename        : CGPSSENSOR.H
 * Author          : George Sebastian
 * Description     : CGPSSensor class is used to get the current position of user
 ****************************************************************************/
#ifndef CGPSSENSOR_H
#define CGPSSENSOR_H

#include "CWaypoint.h"

class CGPSSensor
{
private:
public:

	/**
	 * Constructor of class CGPSSensor
	 * @param no parameters
	 * @return no value
	 */
	CGPSSensor();

	/**
	 *Function to get the current position of the user
	 *@param no parameters
	 *@returnvalue CWaypoint : object of type CWaypoint
	 */
	CWaypoint getCurrentPosition();
};
/********************
 **  CLASS END
 *********************/
#endif /* CGPSSENSOR_H */
