//
// Created by Guy on 15/08/2017.
//

#include <render/Cubemap.hpp>
#include <stb_image.h>

namespace Omicron {

    int Cubemap::LoadCubemap(std::vector<std::string> faces) {
        unsigned int texId;
        glGenTextures(1, &texId);

        glBindTexture(GL_TEXTURE_CUBE_MAP, texId);

        int width, height, nrChannels;
        for(int i = 0; i < faces.size(); i++) {
            unsigned char* data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
            if(data) {
                glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
                    0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            }else{
                printf("Cubemap texture failed to load, \"%s\"\n", faces[i].c_str());
            }
            stbi_image_free(data);
        }

        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

        return texId;
    }
}