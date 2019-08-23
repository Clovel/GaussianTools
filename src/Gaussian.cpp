// Clovis Durand
// ENSEIRB-Matmeca - LASSENA (ETS)
// 12/06/2017

// Gaussian calculation on n points
/*
* This Class calculates n points of a gaussian bell 
* and stocks the info in a vector as it claculates them
*/

// Includes ---------------------------------------------------------

#include "Gaussian.hpp"			// Header file for Gaussian class

// Namespace --------------------------------------------------------
using namespace std;

// Con/Destructors --------------------------------------------------

Gaussian::Gaussian(const int &samplerate,
		const float &amplitude, 
		const float &stddev,
		const float &mean):
			m_samplerate(samplerate), 
			m_amplitude(amplitude),
			m_stddev(stddev),
			m_mean(mean)
{
	// Default constructor
	calculate_points();
}

Gaussian::~Gaussian()
{
	// Default Destructor
}

// Getters ----------------------------------------------------------

uInt Gaussian::getNbPoints(void) const
{
	return m_points.size();
}

vector<float> Gaussian::getPointVector(void) const
{
	return m_points;
}

float Gaussian::getPointAtIndex(const uInt &index) const
{
	if(index < getNbPoints())
		return m_points[index];
	else 
		return 0.0f;
}

float Gaussian::getAmplitude(void) const
{
	return m_amplitude;
}

float Gaussian::getMean(void) const
{
	return m_mean;
}

float Gaussian::getStddev(void) const
{
	return m_stddev;
}

float Gaussian::getSamplerate(void) const
{
	return m_samplerate;
}

// Setters/Mutators -------------------------------------------------

	// Once an attribute is change you need to recalculate
	// all the points. 

	// BEWARE : These functions do not recalculate all the points

void Gaussian::addSample(const uInt &index, const float &element)
{
	m_points.insert(m_points.begin() + index, element);
}

void Gaussian::addSample(const float &element)
{
	m_points.push_back(element);
}

// Printers ---------------------------------------------------------

void Gaussian::printPoint(const uInt &index) const
{
	cout << "g[" << index << "] = " << getPointAtIndex(index);
}

void Gaussian::printPointVector(void) const
{
	cout << "\n\n----------------------------------\n\n";
	cout << "Printing Gaussian Vector values : \n\n";
	for(uInt i = 0; i < getNbPoints() - 1; i++)
	{
		printPoint(i);
		cout << ", \n";
	}
	printPoint(getNbPoints() - 1);
	cout << endl;
}

void Gaussian::VectorToFile(const string &filename)
{
	ofstream flux(filename);

	if(flux)
	{
		string line;
		for(uInt i = 0; i < getNbPoints(); i++)
		{
			line = to_string(getPointAtIndex(i));
			line += "\n";

			flux << line;
		}
		flux.close();
	}
	else
		cerr << "[ERROR]  : Could not open file [" << filename << "]\n";
}

// Misc. Private methods --------------------------------------------

float Gaussian::gaussFunction(const float &x)
{
	return getAmplitude()*exp(-pow(x - getMean(), 2)/(2*pow(getStddev(), 2)));
}

void Gaussian::calculate_points(void)
{
	// Here goes the algorithm that calculated the points 
	// and adds them to the vector

	/*
	Lets recall that a Gaussian function is of the form : 
		y(x) = A.exp[ (-x-mean)^2 / (2.stddev^2) ]		(1)

	Wolfram Alpha solved the following equation
		exp(-((3.5/2*10^-6)^2)/(2*y^2)) = 0.5			(2)
	And found y = +1.48631*10^6
	
	For us, we shall take mean as 0 as default for signal creation. 
	Indeed, we can always translate it by an offset. 

	We shall take a 1 amplitude as default value because the 
	gaussian can always be flattened by a factor afterwards. 

	For a 3.5 us interval in between the two 50% of amp points
	we need a +1.48631*10^6 stddev. This will be our default value 
	because of LASSENA R.A.S. Project DME modulas
	*/

	float element	= 0;
	int i 			= 0; 		// x in the math fomula

	float interval 	= 1.0/(float)getSamplerate();

	addSample(0, getAmplitude()); // On ajoute le premier element

	do
	{
		i++;
		element = gaussFunction(i*interval);

		// Adding right side value
		if(element >=0.01f)
			addSample(0, element);

		// Here, nb_pts = 2*i

		// Adding left side value
		//addSample(0, element);

		// Here, nb_pts = 2*i + 1

	}while(element >= 0.01f);


	// Test
	/*cout << "\nElement at index = agetNbPoints() - 1 is " << getPointAtIndex(getNbPoints() - 1) << endl;
	addSample(getPointAtIndex(getNbPoints() - 2 - 0));
	cout << "\nElement at index = agetNbPoints() - 1 is " << getPointAtIndex(getNbPoints() - 1) << endl;
	addSample(getPointAtIndex(getNbPoints() - 4 - 0));
	cout << "\nElement at index = agetNbPoints() - 1 is " << getPointAtIndex(getNbPoints() - 1) << endl;
	addSample(getPointAtIndex(getNbPoints() - 6 - 0));
	cout << "\nElement at index = agetNbPoints() - 1 is " << getPointAtIndex(getNbPoints() - 1) << endl;*/
	

	for(uInt j = 2; j < getNbPoints() - 1; j += 2)
		addSample(getPointAtIndex(getNbPoints() - j));


}