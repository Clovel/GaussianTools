/* @brief Gaussian calculation on n points
 * This Class calculates n points of a gaussian bell
 * and stores the info in a std::vector as it claculates them
 */

/* Includes -------------------------------------------- */
/* Gaussian */
#include "Gaussian.hpp"         // Header file for Gaussian class

/* System */
#include <iostream>     /* To print to stdout */
#include <fstream>      /* To print in a file */
#include <vector>       /* For main vector attribute */
#include <string>       /* For strings and filenames */
#include <cmath>

/* Class implementation */
/* Con/Destructors */
Gaussian::Gaussian(const int    &pSamplerate,
                   const float  &pAmplitude,
                   const float  &pStdDev,
                   const float  &pMean) :
    mSampleRate(pSamplerate),
    mAmplitude(pAmplitude),
    mStdDev(pStdDev),
    mMean(pMean)
{
    /* Default constructor */
    calculatePoints();
}

Gaussian::~Gaussian()
{
    /* Default Destructor */
}

/* Getters */
unsigned int Gaussian::getNbPoints(void) const
{
    return mPoints.size();
}

std::vector<float> Gaussian::getPointVector(void) const
{
    return mPoints;
}

float Gaussian::getPointAtIndex(const unsigned int &pIndex) const
{
    if (pIndex < getNbPoints()) {
        return mPoints[pIndex];
    } else {
        return .0f;
    }
}

float Gaussian::getAmplitude(void) const
{
    return mAmplitude;
}

float Gaussian::getMean(void) const
{
    return mMean;
}

float Gaussian::getStdDev(void) const
{
    return mStdDev;
}

float Gaussian::getSamplerate(void) const
{
    return mSampleRate;
}

/* Setters/Mutators */
/* Once an attribute is changed you need to recalculate
 * all the points.
 *
 * BEWARE : These functions do not recalculate all the points
 */

void Gaussian::addSample(const unsigned int &pIndex, const float &pElement)
{
    mPoints.insert(mPoints.begin() + pIndex, pElement);
}

void Gaussian::addSample(const float &pElement)
{
    mPoints.push_back(pElement);
}

/* Printers */
void Gaussian::printPoint(const unsigned int &pIndex) const
{
    std::cout << "g[" << pIndex << "] = " << getPointAtIndex(pIndex);
}

void Gaussian::printPointVector(void) const
{
    std::cout   << std::endl << std::endl
                << "----------------------------------" << std::endl << std::endl;
    std::cout << "Printing Gaussian std::vector values : " << std::endl << std::endl;

    for (unsigned int i = 0U; i < getNbPoints() - 1; ++i) {
        printPoint(i);
        std::cout << ", " << std::endl;
    }
    printPoint(getNbPoints() - 1);
    std::cout << std::endl;
}

void Gaussian::vectorToFile(const std::string &pFilename)
{
    std::ofstream lFlux(pFilename);

    if (lFlux) {
        std::string lLine;
        for (unsigned int i = 0U; i < getNbPoints(); ++i) {
            lLine = std::to_string(getPointAtIndex(i));

            lFlux << lLine << std::endl;
        }
        lFlux.close();
    } else {
        std::cerr << "[ERROR] Could not open file [" << pFilename << "]" << std::endl;
    }
}

//* Misc. Private methods */
float Gaussian::gaussFunction(const float &pX)
{
    return getAmplitude() * exp(-pow(pX - getMean(), 2) / (2 * pow(getStdDev(), 2)));
}

void Gaussian::calculatePoints(void)
{
    /* Here goes the algorithm that calculated the points
     * and adds them to the :vector
     */

    /*
     * Lets recall that a Gaussian function is of the form :
     *  y(x) = A.exp[ (-x-mean)^2 / (2.stdDev^2) ]		(1)
     *
     * Wolfram Alpha solved the following equation
     *  exp(-((3.5/2*10^-6)^2)/(2*y^2)) = 0.5			(2)
     * And found y = +1.48631*10^6
     *
     * For us, we shall take mean as 0 as default for signal creation.
     * Indeed, we can always translate it by an offset.
     *
     * We shall take a 1 amplitude as default value because the
     * gaussian can always be flattened by a factor afterwards.
     *
     * For a 3.5 us interval in between the two 50% of amp points
     * we need a +1.48631*10^6 stdDev. This will be our default value
     */

    float   lElement    = .0f;
    int     i           = 0;    /* x in the math fomula */

    float interval = 1.0f / (float)getSamplerate();

    addSample(0, getAmplitude()); /* Adding the first lElement */

    do {
        lElement = gaussFunction(++i * interval);

        /* Adding right side value */
        if (.01f <= lElement) {
            addSample(0U, lElement);
        }

        /* Here, nb_pts = 2*i */

        /* Adding left side value */
        //addSample(0, lElement);

        /* Here, nb_pts = 2*i + 1 */
    } while (.01f <= lElement);

    // Test
    /*std::cout << "\nElement at index = agetNbPoints() - 1 is " << getPointAtIndex(getNbPoints() - 1) << endl;
     * addSample(getPointAtIndex(getNbPoints() - 2 - 0));
     * std::cout << "\nElement at index = agetNbPoints() - 1 is " << getPointAtIndex(getNbPoints() - 1) << endl;
     * addSample(getPointAtIndex(getNbPoints() - 4 - 0));
     * std::cout << "\nElement at index = agetNbPoints() - 1 is " << getPointAtIndex(getNbPoints() - 1) << endl;
     * addSample(getPointAtIndex(getNbPoints() - 6 - 0));
     * std::cout << "\nElement at index = agetNbPoints() - 1 is " << getPointAtIndex(getNbPoints() - 1) << endl;*/

    for (unsigned int j = 2U; j < getNbPoints() - 1; j += 2) {
        addSample(getPointAtIndex(getNbPoints() - j));
    }
}
