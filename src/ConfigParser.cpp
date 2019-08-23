/**
 * Copyright (C) 2019 Clovis Durand
 *
 * @file ConfigParser.cpp
 */

/* Includes -------------------------------------------- */
/* Config */
#include "ConfigParser.hpp"

/* System */
#include <iostream>
#include <fstream>
#include <sstream>

/* Class implemntation ---------------------------------*/
void StringUtility::trim(std::string &pStr)
{
    pStr.erase(0U, pStr.find_first_not_of(" \n\r\t"));
    pStr.erase(pStr.find_last_not_of(" \n\r\t") + 1);
}

std::vector<std::string> ConfigParser::getNextLineAndSplitIntoTokens(std::ifstream &pSs, bool &pEof, const char &pToken)
{
    std::vector<std::string>    result;
    std::string                 line;

    if ((pEof = pSs.eof())) {
        return result;
    }
    std::getline(pSs, line);

    /* Check if empty line */
    auto tmp = line.find(pToken);
    if (std::string::npos == tmp) {
        return result;
    }

    /* Check if commented line */
    if ('#' == line.at(0)) {
        return result;
    }

    std::stringstream   lineStream(line);
    std::string         cell;

    /* Check if comment line */
    unsigned int i = 0U;
    do {
        if (i++ >= line.length()) {
            break;
        }
    } while (' ' == line.at(i));

    /* Reading cells */
    while (std::getline(lineStream, cell, pToken)) {
        StringUtility::trim(cell);
        result.push_back(cell);
    }

    return result;
}

bool ConfigParser::caseInsensitiveStringCompare(const std::string &str1, const std::string &str2)
{
    if (str1.size() != str2.size()) {
        return false;
    }
    for (std::string::const_iterator c1 = str1.begin(), c2 = str2.begin(); c1 != str1.end(); ++c1, ++c2) {
        if (tolower(*c1) != tolower(*c2)) {
            return false;
        }
    }
    return true;
}

void ConfigParser::replaceInString(std::string &str, const std::string &from, const std::string &to)
{
    size_t start_pos = 0U;

    while ((start_pos = str.find(from, start_pos)) != std::string::npos) {
        str.replace(start_pos, from.length(), to);
        start_pos += to.length(); /* Handles case where 'to' is a substring of 'from' */
    }
}

bool ConfigParser::parseConfig(const std::string pFileName, int &pSampleRate, float &pAmp, float &pStdDev, float &pMean)
{
    unsigned int lLineCount = 0U;

    std::string lLine, lFilePath(pFileName);
    std::string lHomeDir(std::getenv("HOME"));

    /* Check filepath integrity */
    /* Check if CSV */
    if (("csv" != lFilePath.substr(lFilePath.find_last_of(".") + 1U)) && ("CSV" != lFilePath.substr(lFilePath.find_last_of(".") + 1U))) {
        std::cerr << "[ERROR] Dictionary file " << lFilePath + " is not a CSV file !" << std::endl;
        return false;
    }

    /* Replace tilde by home path */
    if (std::string::npos != lFilePath.find("~")) {
        ConfigParser::replaceInString(lFilePath, "~", lHomeDir);
    }

    /* Open file */
    std::ifstream fileStream(lFilePath);

    /* Check if file exists */
    if (fileStream.bad() || !fileStream) {
        std::cerr << "[ERROR] Config file " << lFilePath << " does not exists or cannot be opened !" << std::endl;
        return false;
    }

    /* Read the file */
    std::vector<std::string>    cells;
    bool                        eof = false;
    do {
        ++lLineCount;

        cells = ConfigParser::getNextLineAndSplitIntoTokens(fileStream, eof, ',');

        if (cells.empty()) {
            continue;
        } else if (2 != cells.size()) {
            std::cerr << "[WARN] Wrong number of cells on line " << lLineCount << std::endl;

            std::cerr << "Setting this entry as invalid" << std::endl;

            /* TODO : Check if complete cell line is on two text lines */
            continue;
        } else if ("Index" == cells.at(0)) {
            /* First line contains the collumn names */
            continue;
        }

        if ("SampleRate" == cells.at(0U)) {
            try {
                pSampleRate = std::stoi(cells.at(0U).c_str(), 0U, 10);
            } catch (...) {
                std::cerr << "[ERROR] Line " << std::to_string(lLineCount) << " : Index cell is invalid ! (Got : " << pSampleRate << ")" << std::endl;
                return false;
            }
        } else if ("Amplitude" == cells.at(0U)) {
            try {
                pAmp = std::stof(cells.at(0U).c_str());
            } catch (...) {
                std::cerr << "[ERROR] Line " << std::to_string(lLineCount) << " : Index cell is invalid ! (Got : " << pAmp << ")" << std::endl;
                return false;
            }
        } else if ("StdDev" == cells.at(0U)) {
            try {
                pStdDev = std::stof(cells.at(0U).c_str());
            } catch (...) {
                std::cerr << "[ERROR] Line " << std::to_string(lLineCount) << " : Index cell is invalid ! (Got : " << pStdDev << ")" << std::endl;
                return false;
            }
        } else if ("Mean" == cells.at(0U)) {
            try {
                pMean = std::stof(cells.at(0U).c_str());
            } catch (...) {
                std::cerr << "[ERROR] Line " << std::to_string(lLineCount) << " : Index cell is invalid ! (Got : " << pMean << ")" << std::endl;
                return false;
            }
        } else {
            std::cerr << "[ERROR] Config file is not valid" << std::endl;
            return false;
        }
    } while (!eof);

    return true;
}
