/***************************************************************************
 *============= Copyright by Darmstadt University of Applied Sciences =======
 ****************************************************************************
 * Filename        : CFILEIO.H
 * Author          : George Sebastian
 * Description     : CFileIO provides some basic text file input and output operations
 *                   for the ease of the end user
 *                   Provides APIs for opening,closing,reading and writing a line to file
 ****************************************************************************/
#ifndef CFILEIO_H
#define CFILEIO_H

#include <string>
#include <fstream>

class CFileIO {

public:

	// enumeration for identifying the direction of file operation
	enum t_direction{IN, OUT, UNDEF};

	/**
	 * Constructor of CFileIO class
	 * Initializes attributes with some initial value
	 * @returnvalue no value
	 */
	CFileIO();

	/**
	 * Constructor of CFileIO class with parameters
	 * @param const string& fileName       : IN name of the file
	 * @param const t_direction& direction : IN direction of File operation
	 * @returnvalue no value
	 */
	CFileIO(const std::string& fileName,const t_direction& direction);

	/**
	 * Destructor of CFileIO class
	 * @returnvalue no value
	 */
	~CFileIO();

	/**
	 * Function for closing the file
	 * @returnvalue true if closing operation is done successfully
	 */
    bool closeFile();

    /**
     * Function for opening the file and sets the direction of
     * file operations as mentioned by user
     * @param const string& fileName       : IN name of the file to be opend
     * @param const t_direction& direction : IN direction for file operations
     * @returnvalue true if file is opened successfully
     */
    bool openFile(const std::string& fileName, const t_direction& dir);

    /**
     * Function writes a single line to the file
     * @param const string& line : IN line to be written to the file
     * @returnvalue true if write operation is successful
     */
    bool writeLineToFile(const std::string& line);

    /**
     * Function reads a single line from a file
     * @param string& line : OUT line read from the file
     * @returnvalue true if line is read successfully
     */
    bool readLineFromFile(std::string& line);



private:

	t_direction m_direction;
	std::fstream m_file;

};

/********************
**  CLASS END
*********************/
#endif /* CFILEIO_H */
