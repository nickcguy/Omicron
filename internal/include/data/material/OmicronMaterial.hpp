//
// Created by Guy on 07/08/2017.
//

#ifndef OMICRONRENDER_OMICRONMATERIAL_HPP
#define OMICRONRENDER_OMICRONMATERIAL_HPP

#include <string>
#include <render/shader/Shader.hpp>
#include <map>
#include <vector>
#include <atomic>
#include <functional>
#include <deque>
#include "Texture.hpp"

namespace Omicron {

    struct InterpolatingValue {
        std::string name;
        float value;
        float start;
        float target;
        float life;
        float duration;
        bool active;
        bool hasCallback;
        std::function<void()> callback;
    };

    class OmicronMaterial {
    public:

        OmicronMaterial();
        OmicronMaterial(OmicronMaterial& other);
        OmicronMaterial(OmicronMaterial* other);

        void LoadFromFile(std::string hostPath);

        std::string LoadNameFromFile(std::string hostPath);
        void LoadFromMemory(std::string contents = "");

        void SetUniforms(std::map<std::string, float> externalUniforms = std::map<std::string, float>());
        Shader& GetShader();
        std::vector<std::string> GetIncludes();
        std::string GetName();

        void SetName(std::string name);
        void SetShader(Shader& shader);

        bool IsReady();
        void SetReady(bool ready);

        void Update(float delta);

        void AddInterpolatingValue(std::string name, float target, float duration);
        void AddInterpolatingValue_CB(std::string name, float target, float duration, std::function<void()> callback);
        void InvalidateSamplers();
        float GetUniform(std::string name);
        void SetUniform(std::string name, float value);

    protected:
        std::atomic_bool ready;

        std::string name;
        Shader shader;
        std::string hostPath;
        std::vector<std::string> includes;
        std::map<std::string, float> uniforms;
        std::map<std::string, Texture*> samplers;
        std::deque<Omicron::InterpolatingValue*> interps;

        std::string fileContents;

    };

}

#endif //OMICRONRENDER_OMICRONMATERIAL_HPP
