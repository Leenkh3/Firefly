// *****************************************************************************
/*!
  \file      src/ndiff/ndiff-executer.cpp
  \brief     ndiff tool for comparing two files
*/
// *****************************************************************************

#include "ndiff-executer.hpp"
#include <cstdlib>
#include <string>
#include <sstream>
#include <fstream>
#include <iostream>
#include <stdexcept>

NdiffExecuter::NdiffExecuter(std::string firstFile, std::string secondFile, std::string configFile){
    this->firstFile = firstFile;
    this->secondFile = secondFile;
    this->configFile = configFile;
}

std::string NdiffExecuter::execute(){
    // Check if files exist
    if (!fileExists(firstFile) || !fileExists(secondFile) || !fileExists(configFile)) {
        throw std::runtime_error("One or more input files do not exist.");
    }

    // Construct the command
    std::string command = "ndiff " + firstFile + " " + secondFile + " " + configFile + " 2>&1";

    // Use popen to capture the output of the command
    std::ostringstream output;
    FILE* pipe = popen(command.c_str(), "r");
    if (!pipe) {
        throw std::runtime_error("Failed to execute command: " + command);
    }

    char buffer[128];
    while (fgets(buffer, sizeof(buffer), pipe) != nullptr) {
        output << buffer;
    }

    int returnCode = pclose(pipe);
    if (returnCode != 0) {
        throw std::runtime_error("Command failed with return code " + std::to_string(returnCode) + ": " + output.str());
    }

    return output.str();
}

void NdiffExecuter::setFirstFile(std::string firstFile){
    this->firstFile = firstFile;
}

void NdiffExecuter::setSecondFile(std::string secondFile){
    this->secondFile = secondFile;
}

void NdiffExecuter::setConfigFile(std::string configFile){
    this->configFile = configFile;
}

std::string NdiffExecuter::getFirstFile(){
    return firstFile;
}

std::string NdiffExecuter::getSecondFile(){
    return secondFile;
}

std::string NdiffExecuter::getConfigFile(){
    return configFile;
}

// Helper function to check if a file exists
bool NdiffExecuter::fileExists(const std::string& filePath) {
    std::ifstream file(filePath);
    return file.good();
}
