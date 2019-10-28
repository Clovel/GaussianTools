/* @brief Gaussian calculation on n points
 * This Class calculates n points of a gaussian bell
 * and stores the info in a vector as it claculates them
 */

#ifndef GAUSSIAN_HPP
#define GAUSSIAN_HPP

/* Includes -------------------------------------------- */
/* System */
#include <vector>
#include <string>

/* C System */
#include <cmath>

/* Defines --------------------------------------------- */

#define PI 3.14159265359f   /* Definition of the approximate pi number */

/* Classes --------------------------------------------- */
class Gaussian
{
    public:
        /* Con/Destructor */
        Gaussian(const int &pSamplerate,
                 const float &pAmplitude    = 1.0f,
                 const float &pStdDev       = 1.48631f * (float)pow(10, -6),
                 const float &pMean         = 0.0f);
        ~Gaussian();

        /* Getters */
        unsigned int        getNbPoints(void) const;
        std::vector<float>  getPointVector(void) const;
        float               getPointAtIndex(const unsigned int &pIndex) const;

        float   getAmplitude(void) const;
        float   getMean(void) const;
        float   getStdDev(void) const;
        float   getSamplerate(void) const;

        /* Setters : None because it is a constant object. */

        /* Modifiers */
        void    addSample(const unsigned int &pIndex, const float &pElement);
        void    addSample(const float &pElement);

        /* Printers & File Management */
        void    printPoint(const unsigned int &pIndex) const;
        void    printPointVector(void) const;
        void    vectorToFile(const std::string &pFilename);

    protected:
        std::vector<float> mPoints;

        int mSampleRate;    /* Sample Rate with wich we be be calculating our interval of sample */
        float mAmplitude;   /* Maximum of the bell / Amplitude of the Gaussian bell */
        float mStdDev;      /* Standard deviation (ou écart type en francais), symbolized in math by "sigma" */
        float mMean;        /* Mean/Expected value, symbolized in math by "µ". Is 0 as default */

        float   gaussFunction(const float &pX);
        void    calculatePoints(void);
};

#endif /* GAUSSIAN_HPP */
