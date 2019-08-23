// // Clovis Durand
// ENSEIRB-Matmeca - LASSENA (ETS)
// 12/06/2017

// Gaussian calculation on n points
/*
* This Class calculates n points of a gaussian bell 
* and stocks the info in a vector as it claculates them
*/

#ifndef GAUSSIAN_H
#define GAUSSIAN_H

// Includes ---------------------------------------------------------

#include <iostream>		// To print to stdout
#include <fstream>		// To print in a file
#include <vector>		// For main vector attribute
#include <string>		// For strings and filenames
#include <cmath>

// Defines ----------------------------------------------------------

typedef unsigned int uInt;

#define PI 3.14159265359f	// Definition of the approximate pi number

// Classes ----------------------------------------------------------
class Gaussian
{
public:
	// Con/Destructor
	Gaussian(const int &samplerate,
		const float &amplitude = 1, 
		const float &stddev    = 1.48631f*(float)pow(10, -6),
		const float &mean      = 0);
	~Gaussian();

	// Getters
	uInt getNbPoints(void) const;
	std::vector<float> getPointVector(void) const;
	float getPointAtIndex(const uInt &index) const;
	
	float getAmplitude(void) const;
	float getMean(void) const;
	float getStddev(void) const;
	float getSamplerate(void) const;

	// Setters : None because is fixed object. 

	void addSample(const uInt &index, const float &element);
	void addSample(const float &element);

	// Printers & File Management

	void printPoint(const uInt &index) const;
	void printPointVector(void) const;
	void VectorToFile(const std::string &filename);

private:
	std::vector<float> m_points;

	float m_amplitude;		// Maximum of the bell / Amplitude of the Gaussian bell
	float m_stddev; 		// Standard deviation (ou écart type en francais), symbolized in math by "sigma"
	float m_mean;			// Mean/Expected value, symbolized in math by "µ". Is 0 as default
	int   m_samplerate;		// Sample Rate with wich we be be calculating our interval of sample

	float gaussFunction(const float &x);
	void calculate_points(void);
};

#endif // GAUSSIAN_H