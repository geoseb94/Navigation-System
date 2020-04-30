/***************************************************************************
 *============= Copyright by Darmstadt University of Applied Sciences =======
 ****************************************************************************
 * Filename        : CDATABASE.H
 * Author          : George Sebastian
 * Description     : CDatabase.h provides a Template Class for handling
 *                   Waypoint Database and POI Database
 *                   T1 -> Keytype of Map
 *                   T2 -> Type of the object stored in Map
 ****************************************************************************/
#ifndef CDATABASE_H_
#define CDATABASE_H_

#include <iostream>
#include <map>

template<class keyType, class objectType> // T1 -> Type of the Key in Map , T2 -> Type of the object stored in Map
class CDatabase
{

private:

	std::map<keyType, objectType> m_dbContainer;

public:

	/**
	 * Function adds object of type 'ObjectType' provided by user to Database, if Object is valid
	 * If Object is already existing in Database, it will be overwritten and Warning is
	 * displayed.
	 * @param objectType const& object : IN object to be added to Database
	 * @returnvalue void
	 */
	void addObjectToDb(objectType const& object);

	/**
	 * Function prints the contents of Database
	 * @param no parameter
	 * @returnvalue void
	 */
	void printDb();

	/**
	 * Function clears the contents of Database
	 * @param no parameters
	 * @returnvalue void
	 */
	void clearDb();

	/**
	 * Function checks for the Object having the same name as passed by the user in the Database
	 * and returns the pointer to the Object in the Database
	 * @param const std::string& name : IN name of the object to be checked in Database
	 * @returnvalue objectType*       : pointer to the object in Database if found,otherwise returns a NULL pointer
	 */
	objectType* getPointerToObject(const std::string& name);

	/**
	 * Constructor for the CDatabase class
	 * @param no parameters
	 * @return no values
	 */
	CDatabase()
	{
	}

	/**
	 * Function used to extract Map containing all elements in Database
	 * @param std::map<keyType, objectType>& dbContainer : OUT Map containing all elements in Database
	 * @returnvalue void
	 */
	const void getDbContainer(std::map<keyType, objectType>& dbContainer) const;
};

/**
 * Function adds object of type 'ObjectType' provided by user to Database, if Object is valid
 * If Object is already existing in Database, it will be overwritten and Warning is
 * displayed.
 * @param objectType const& object : IN object to be added to Database
 * @returnvalue void
 */
template<class keyType, class objectType>
void CDatabase<keyType, objectType>::addObjectToDb(objectType const& object)
{
	if (object.getName() != "default city")
	//  Object contains valid data, then added to the Database
	{
		typename std::map<keyType, objectType>::iterator itr;
		itr = m_dbContainer.find(object.getName()); // checks if Object is already existing in Database

		if (itr != m_dbContainer.end())
			//	Object already exists in Database
			std::cout << "WARNING : " << object.getName()
					<< " already exists in  Database and will be overwritten ! "
					<< std::endl;

		m_dbContainer.insert(
				pair<keyType, objectType>(object.getName(), object));

	}
	else
		// Object contains invalid data, then not added to the Database
		std::cout
				<< "Object details are invalid, Therefore not added to Database"
				<< std::endl;

}

/**
 * Function checks for the Object having the same name as passed by the user in the Database
 * and returns the pointer to the Object in the Database
 * @param const std::string& name : IN name of the object to be checked in Database
 * @returnvalue objectType*       : pointer to the object in Database if found,otherwise returns a NULL pointer
 */
template<class keyType, class objectType>
objectType* CDatabase<keyType, objectType>::getPointerToObject(
		const std::string& name)
{
	objectType* pObject;

	typename std::map<keyType, objectType>::iterator itr;
	itr = m_dbContainer.find(name); // searches in STL MAP for a KEY equal to 'name'

	if (itr != m_dbContainer.end())
		// found in the Database
		pObject = &(itr->second);
	else
		// not found in the Database
		pObject = NULL;

	return pObject;

}

/**
 * Function prints the contents of Database
 * @param no parameter
 * @returnvalue void
 */
template<class keyType, class objectType>
void CDatabase<keyType, objectType>::printDb()
{

	std::cout << "Database contains " << m_dbContainer.size() << " elements"
			<< std::endl << std::endl;

	for (typename std::map<keyType, objectType>::const_iterator itr =
			m_dbContainer.begin(); itr != m_dbContainer.end(); ++itr)
	{
		std::cout << itr->second << std::endl; // operator overloaded for printing the objects
	}
}

/**
 * Function clears the contents of Database
 * @param no parameters
 * @returnvalue void
 */
template<class keyType, class objectType>
void CDatabase<keyType, objectType>::clearDb()
{
	m_dbContainer.clear();
}

/**
 * Function used to extract Map containing all elements in Database
 * @param std::map<keyType, objectType>& dbContainer : OUT Map containing all elements in Database
 * @returnvalue void
 */
template<class keyType, class objectType>
const void CDatabase<keyType, objectType>::getDbContainer(
		std::map<keyType, objectType>& dbContainer) const
{
	dbContainer = m_dbContainer;
}

#endif /* CDATABASE_H_ */
