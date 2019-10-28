/**
 * Copyright (C) 2019 Clovis Durand
 *
 * 12/06/2017
 *
 * @brief Gaussian generator utility
 */

/* Includes -------------------------------------------- */
/* Gaussian */
#include "Gaussian.hpp"

/* config file parsing */
#include "ConfigParser.hpp"

/* System */
#include <iostream>

/* C System */
#include <cstdlib>
#include <cmath>
#include <cstring>

/* Defines --------------------------------------------- */
#ifndef UTILITY_TEST
#define UTILITY_TEST
#endif /* UTILITY_TEST */

/* Support functions ----------------------------------- */
static void print_usage(const char * const pProgName)
{
    std::cout << "[USAGE] " << pProgName << std::endl;
    std::cout << "\t\tThis program generates a Gaussian bell curve and prints the data in a .txt file" << std::endl;
}

/* Main routine ---------------------------------------- */
int main(const int argc, char const * const *argv)
{
    if ((2 > argc) || (0 == std::strcmp("--help", argv[1]))) {
        print_usage(argv[0]);
        exit(EXIT_FAILURE);
    }

    Gaussian    *g0 = nullptr;
    std::string lDestFile("./GaussPoints.csv");
    std::string lConfigFile;

    int     lSampleRate = std::pow(10, 7);
    float   lAmplitude  = 1.0f;
    float   lStdDev     = 1.48631f * (float)pow(10, -6);
    float   lMean       = .0f;

    if (3 == argc) {
        lDestFile   = std::string((char *)argv[1]);
        lConfigFile = std::string((char *)argv[2]);

        /* Check order of the arguments */
        std::string tmpstr;
        if (".csv" == lDestFile.substr(lDestFile.length() - 4)) {
            /* Swap arguments */
            tmpstr      = lDestFile;
            lDestFile   = lConfigFile;
            lConfigFile = tmpstr;
        }

        /* Check if config file is a CSV file */
        if (".csv" != lConfigFile.substr(lConfigFile.length() - 4)) {
            /* Config file is (still) not a csv file */
            std::cerr << "[ERROR] config file is not a CSV, aborting..." << std::endl;
            exit(EXIT_FAILURE);
        }

        if (!ConfigParser::parseConfig(lConfigFile, lSampleRate, lAmplitude, lStdDev, lMean)) {
            std::cerr << "[ERROR] Faield to parse config file" << std::endl;
            exit(EXIT_FAILURE);
        }
    } else {
        std::cerr << "[ERROR] Wrong number of arguments" << std::endl;
        exit(EXIT_FAILURE);
    }

    g0 = new Gaussian(lSampleRate, lAmplitude, lStdDev, lMean);

#ifdef UTILITY_TEST
    int f_ech = lSampleRate;
    std::cout << "f_ech = " << f_ech << std::endl;

    float T_ech = 1.0f / f_ech;
    std::cout << "T_ech = " << T_ech << std::endl;

    float   temp    = .0f;
    int     i       = 0;

    while (.5f > temp) {
        temp = g0->getPointAtIndex(i++);
        //std::cout << "g0[" << i << "] = " << temp << std::endl;
    }

    int g0_pt1 = i;

    while (.5f <= temp) {
        temp = g0->getPointAtIndex(i++);
        //std::cout << "g0[" << i << "] = " << temp << std::endl;
    }

    int g0_pt2 = i;
    std::cout << "g0_pt1 = " << g0_pt1 << std::endl;
    std::cout << "g0_pt2 = " << g0_pt2 << std::endl;

    int deltaPt(g0_pt2 - g0_pt1);
    std::cout << "deltaPt = " << deltaPt << " samples" << std::endl;

    float deltaT_50 = (float)deltaPt * T_ech;
    std::cout << "deltaT_50 = " << deltaT_50 << "s" << std::endl;

    float deltaT_100 = (float)g0->getNbPoints() * T_ech;
    std::cout << "Total number of samples : " << g0->getNbPoints() << std::endl;
    std::cout << "deltaT_100 = " << deltaT_100 << "s" << std::endl;
#endif /* UTILITY_TEST */

    //g0.printPointVector();
    g0->vectorToFile(lDestFile);

    if (nullptr != g0) {
        delete g0;
    }

    return 0;
}
