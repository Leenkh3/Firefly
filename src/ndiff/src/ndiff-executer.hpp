#pragma once
#include <string>
#include <iostream>

class NdiffExecuter{


    private:
    std::string firstFile;
    std::string secondFile;
    std::string configFile;

    bool fileExists(const std::string& filePath);

    public:
    //! \brief A utility class for comparing two files using the ndiff tool.
    //!
    //! This class provides functionality to execute the ndiff command-line tool
    //! for comparing two files with an optional configuration file. It includes
    //! methods to set and retrieve file paths, check file existence, and capture
    //! the output of the ndiff command.
    NdiffExecuter(std::string firstFile, std::string secondFile, std::string configFile);
    std::string execute();
    void setFirstFile(std::string firstFile);
    void setSecondFile(std::string secondFile);
    void setConfigFile(std::string configFile);
    std::string getFirstFile();
    std::string getSecondFile();
    std::string getConfigFile();
};
