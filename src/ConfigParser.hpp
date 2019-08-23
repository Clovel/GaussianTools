/**
 * Copyright (C) 2019 Clovis Durand
 *
 * @file ConfigParser.hpp
 */

#ifndef EZW_TOOLS_CSVPARSER_HPP
#define EZW_TOOLS_CSVPARSER_HPP

/* Includes -------------------------------------------- */
#include <fstream>
#include <vector>

/* Class ----------------------------------------------- */
class StringUtility {
    public:
        static void trim(std::string &pStr);
};

class ConfigParser {
    public:
        static bool parseConfig(const std::string pFileName, int &pSampleRate, float &pAmp, float &pStdDev, float &pMean);

    protected:
        static std::vector<std::string> getNextLineAndSplitIntoTokens(std::ifstream &pSs, bool &pEof, const char &pToken = ',');
        static bool                     caseInsensitiveStringCompare(const std::string &str1, const std::string &str2);
        static void                     replaceInString(std::string &str, const std::string &from, const std::string &to);
};

#endif /* EZW_TOOLS_CSVPARSER_HPP */
