//
// Created by Guy on 06/08/2017.
//

#ifndef OMICRONRENDER_FILEUTILS_HPP
#define OMICRONRENDER_FILEUTILS_HPP

#include <vector>

namespace Omicron {

    namespace Files {

        std::vector<std::string> ReadLines(std::string file);
        std::string ReadFile(std::string file);

        std::vector<std::string> GetFilesInDirectory(std::string directory);
        std::vector<std::string> GetFilesInDirectory(std::string directory, std::string extension);

    };

};

#endif //OMICRONRENDER_FILEUTILS_HPP
