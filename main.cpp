/***************************************************************************
 *============= Copyright by Darmstadt University of Applied Sciences =======
 ****************************************************************************
 * Filename        : MAIN.CPP
 * Author          : George Sebastian
 * Description     : Main() function invokes our Navigation System.
 *                   Additionally MAIN.CPP contains different Testcases for
 *                   testing the functionality of CWaypoint class
 ****************************************************************************/
#include <iostream>
using namespace std;

#include "CCSV.h"
#include "CWaypoint.h"
#include "CNavigationSystem.h"

#define TESTCASE 0   // Different Testcases for CWaypoint Class
#define DEGREE 1     // It is better to include DEGREE and MMSS in main.cpp file, not in CWaypoint.h file.
#define MMSS 2       // if it is included in .h file,these might get included unnecessarily in many other .cpp files

int main(void)
{

	CNavigationSystem navigationSystem;
	navigationSystem.run();

/**
* ================================
*  TESTCASE FOR CWaypoint CLASS
* ================================
*/
#if TESTCASE == 1
	// Testcase to print latitude and longitude of berlin in both decimal and deg mm ss formats

	CWaypoint amsterdam("amsterdam",52.3680,4.9036),darmstadt("darmstadt",49.8728,8.6512),berlin("berlin",52.5200,13.4050),tokio("tokio",35.6895,139.6917);

	berlin.print(DEGREE);
	berlin.print(MMSS);

#endif

#if TESTCASE == 2
	// Testcase for function getName()

	CWaypoint amsterdam("amsterdam",52.3680,4.9036),darmstadt("darmstadt",49.8728,8.6512),berlin("berlin",52.5200,13.4050),tokio("tokio",35.6895,139.6917);
	cout<<"Name as return value : "<<berlin.getName()<<endl;

#endif

#if TESTCASE == 3
	// Testcase for function getLatitude()

	CWaypoint amsterdam("amsterdam",52.3680,4.9036),darmstadt("darmstadt",49.8728,8.6512),berlin("berlin",52.5200,13.4050),tokio("tokio",35.6895,139.6917);
	cout<<"Latitude as return value : "<<berlin.getLatitude()<<endl;

#endif

#if TESTCASE == 4
	// Testcase for function getLongitude()

	CWaypoint amsterdam("amsterdam",52.3680,4.9036),darmstadt("darmstadt",49.8728,8.6512),berlin("berlin",52.5200,13.4050),tokio("tokio",35.6895,139.6917);
	cout<<"Longitude as return value : "<<berlin.getLongitude()<<endl;

#endif

#if TESTCASE == 5
	// Testcase for function getAllDataByReference()

	CWaypoint amsterdam("amsterdam",52.3680,4.9036),darmstadt("darmstadt",49.8728,8.6512),berlin("berlin",52.5200,13.4050),tokio("tokio",35.6895,139.6917);
	string name="default city";
	double latitude=0.0,longitude=0.0;

	cout<<"Address and values of local variables before invoking getAllDataByReference() : name "<<name<<" "<<&name<<", latitude "<<latitude<<" "<<&latitude<<", longitude "<<longitude<<" "<<&longitude<<endl;

/*  Call by Reference : Both Address and values of actual parameters and formal parameters are same.
	Therefore changes done in these formal parameters inside the function will get reflected back on
	the actual parameters.
	Reason : same address
*/
	tokio.getAllDataByReference(name,latitude,longitude);

	cout<<"Name, Latitude and Longitude as return value after invoking getAllDataByReference() : "<<name<<" "<<latitude<<" "<<longitude<<endl;

#endif

#if TESTCASE == 6
	// Testcase for calculateDistance()

	CWaypoint amsterdam("amsterdam",52.3680,4.9036),darmstadt("darmstadt",49.8728,8.6512),berlin("berlin",52.5200,13.4050),tokio("tokio",35.6895,139.6917);

	cout<<"Distance between Amsterdam and Berlin in km : "<<amsterdam.calculateDistance(berlin)<<endl;
	cout<<"Distance between Berlin and Tokio in km : "<<berlin.calculateDistance(tokio)<<endl;

#endif

	return 0;
}
