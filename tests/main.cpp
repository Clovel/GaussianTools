/**
 * Copyright (C) 2019 Clovis Durand
 *
 * @brief Gaussian lib tests
 */

/* Includes -------------------------------------------- */
/* Gaussian */
#include "Gaussian.hpp"

/* System */
#include <iostream>

/* C System */
#include <cstring>

/* Defines --------------------------------------------- */

/* Notes ----------------------------------------------- */

/* Variable declaration -------------------------------- */

/* Support functions ----------------------------------- */
static void print_usage(const char * const pProgName)
{
    std::cout << "[USAGE] " << pProgName << " test#" << std::endl;
    std::cout << "        Test -1 : default/no test" << std::endl;
}

/* Tests ----------------------------------------------- */

/* ----------------------------------------------------- */
/* main function --------------------------------------- */
/* ----------------------------------------------------- */
int main(const int argc, const char * const * const argv)
{
    /* Test function initialization */
    int32_t lTestNum;
    int16_t lResult = 0;

    if ((argc < 2) || (std::strcmp(argv[1], "--help") == 0)) {
        print_usage(argv[0]);
        return -1;
    }

    lTestNum = strtol(argv[1], NULL, 10);

    std::cout << "[TEST ] Executing test #" << lTestNum << std::endl;

    /* Executing test */
    switch (lTestNum) {
        default:
            std::cout << "[INFO ] test #" << lTestNum << " not available" << std::endl;
            fflush(stdout);
            break;
    }

    return 0;
}
