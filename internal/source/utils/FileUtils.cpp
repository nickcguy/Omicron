//
// Created by Guy on 06/08/2017.
//

#include <fstream>
#include <vector>
#include <dirent.h>

namespace Omicron {

    namespace Files {

        std::vector<std::string> ReadLines(std::string file) {
            std::ifstream in(file);
            std::string line;
            std::vector<std::string> lines;
            while(std::getline(in, line))
                lines.push_back(line);
            in.close();
            return lines;
        }

        std::string ReadFile(std::string file) {
            std::vector<std::string> lines = ReadLines(file);
            std::string content;
            for(std::string line : lines)
                content += line + "\n";
            return content;
        }

        std::vector<std::string> GetFilesInDirectory(std::string directory) {
            std::vector<std::string> files;

            auto path = directory.c_str();

            DIR* dir;
            struct dirent* ent;

            if((dir = opendir(path)) != NULL) {
                while((ent = readdir(dir)) != NULL) {
                    files.emplace_back(ent->d_name);
                    printf("%s\n", ent->d_name);
                }
                closedir(dir);
            }else printf("Could not open directory: %s\n", directory);

            return files;
        }

        std::vector<std::string> GetFilesInDirectory(std::string directory, std::string extension) {
            std::vector<std::string> files;

            auto path = directory.c_str();

            DIR* dir;
            struct dirent* ent;

            if((dir = opendir(path)) != NULL) {
                while((ent = readdir(dir)) != NULL) {
                    std::string fileName(ent->d_name);
                    if(fileName.substr(fileName.find_last_of('.')) == extension)
                        files.push_back(fileName);
                }
                closedir(dir);
            }else printf("Could not open directory: %s\n", directory);



            return files;
        }

    };

};