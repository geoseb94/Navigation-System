/***************************************************************************
 *============= Copyright by Darmstadt University of Applied Sciences =======
 ****************************************************************************
 * Filename        : CFILEIO.CPP
 * Author          : George Sebastian
 * Description     : CFileIO provides some basic text file input and output operations
 *                   for the ease of the end user
 *                   Provides APIs for opening,closing,reading and writing a line to file
 ****************************************************************************/
#include <iostream>
#include <fstream>
using namespace std;

#include "CFileIO.h"

/**
 * Constructor of CFileIO class
 * Initializes attributes with some initial value
 * @returnvalue no value
 */
CFileIO::CFileIO()
{
	m_direction = UNDEF;
	m_file.clear();
}

/**
 * Constructor of CFileIO class with parameters
 * @param const string& fileName       : IN name of the file
 * @param const t_direction& direction : IN direction of File operation
 * @returnvalue no value
 */
CFileIO::CFileIO(const string& fileName, const t_direction& direction)
{
	if (!openFile(fileName, direction))
	// checking error condition
	{
		cout << "Error opening file" << endl;
	}
}

/**
 * Destructor of CFileIO class
 * @returnvalue no value
 */
CFileIO::~CFileIO()
{
	closeFile();
}

/**
 * Function for opening the file and sets the direction of
 * file operations as mentioned by user
 * @param const string& fileName       : IN name of the file to be opend
 * @param const t_direction& direction : IN direction for file operations
 * @returnvalue true if file is opened successfully
 */
bool CFileIO::openFile(const string& fileName, const t_direction& direction)
{
	switch (direction)
	{
	case IN:
		m_file.open(fileName.c_str(), fstream::in); // Open the file and sets file operation direction
		m_file.seekg(0);                          // Go to the first position
		break;
	case OUT:
		m_file.open(fileName.c_str(), fstream::out); // Open the file and sets file operation direction
		m_file.seekp(0);
		break;
	default:
		;
	}

	m_direction = direction;
	return true;
}

/**
 * Function reads a single line from a file
 * @param string& line : OUT line read from the file
 * @returnvalue true if line is read successfully
 */
bool CFileIO::readLineFromFile(string& line)
{
	if ((m_file.is_open()) && (!m_file.eof()))
	// file is open and not at end of the file
	{
		getline(m_file, line);  // reading a single line from file
		return true;
	}

	return false;
}

/**
 * Function writes a single line to the file
 * @param const string& line : IN line to be written to the file
 * @returnvalue true if write operation is successful
 */
bool CFileIO::writeLineToFile(const string& line)
{
	if (m_file.is_open())
	// file is open
	{
		m_file << line << endl; // Write a single line, 'endl' is important here
		return true;
	}
	else
	// file is not open
	{
		return false;
	}
}

/**
 * Function for closing the file
 * @returnvalue true if closing operation is done successfully
 */
bool CFileIO::closeFile()
{
	if (m_file.is_open())
		// file is open
		m_file.close();

	return true;
}


