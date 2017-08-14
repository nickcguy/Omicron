//
// Created by Guy on 07/08/2017.
//

#include <data/material/OmicronMaterial.hpp>
#include <utility>
#include <utils/FileUtils.hpp>
#include <json.hpp>

#ifndef JSON_CONTAINS
#define JSON_CONTAINS(json, field, defVal) json.find(field) != json.end() ? json[field] : defVal
#endif

#ifndef DEBUG_PRINT
#define DEBUG_PRINT(format, ...) printf(format, ...)
#endif

using json = nlohmann::json;

namespace Omicron {

    void OmicronMaterial::LoadFromFile(std::string hostPath) {
        std::string contents = Files::ReadFile(std::move(hostPath));
        LoadFromMemory(contents);
    }

    void OmicronMaterial::SetUniforms(std::map<std::string, float> externalUniforms) {
        shader.Use();

        for(auto pair : uniforms)
            shader.SetFloat(pair.first.c_str(), pair.second);

        for(auto pair : externalUniforms)
            shader.SetFloat(pair.first.c_str(), pair.second);

        int index = 0;
        for(auto pair : samplers) {
            glActiveTexture(GL_TEXTURE0 + index);
            pair.second->Bind();
            shader.SetInteger((pair.first + "Map").c_str(), index);
            index++;
        }
    }


    Shader& OmicronMaterial::GetShader() {
        return shader;
    }

    std::string OmicronMaterial::GetName() {
        return name;
    }

    void OmicronMaterial::SetName(std::string name) {
        this->name = name;
    }

    void OmicronMaterial::SetShader(Shader& shader) {
        this->shader = shader;
    }

    std::vector<std::string> OmicronMaterial::GetIncludes() {
        return includes;
    }

    bool OmicronMaterial::IsReady() {
        return ready.load();
    }

    void OmicronMaterial::SetReady(bool ready) {
        this->ready.store(ready);
    }

    OmicronMaterial::OmicronMaterial() {
        SetReady(false);
    }

    std::string OmicronMaterial::LoadNameFromFile(std::string hostPath) {
        std::string contents = Files::ReadFile(std::move(hostPath));
        this->fileContents = contents;
        auto meta = json::parse(contents);

        // Create sampler textures
        auto samplersMeta = meta["Samplers"];
        for(json::iterator it = samplersMeta.begin(); it != samplersMeta.end(); it++)
            samplers[it.key()] = new Texture();

        return this->name = meta["Name"];
    }

    void OmicronMaterial::LoadFromMemory(std::string contents) {

        if(contents.empty())
            contents = this->fileContents;

        printf("%s contents loaded\n", name.c_str());

        auto meta = json::parse(contents);

        printf("%s JSON parsed\n", name.c_str());

        name = meta["Name"];
        auto shaderMeta = meta["Shader"];
        shader.SetSourcesFromFiles(shaderMeta["Vertex"], shaderMeta["Fragment"], JSON_CONTAINS(shaderMeta, "Geometry", ""));

        auto includesMeta = meta["Includes"];
        for(const auto& include : includesMeta)
            includes.push_back(include);

        printf("%s includes stored\n", name.c_str());

        auto uniformsMeta = meta["Uniforms"];
        for(json::iterator it = uniformsMeta.begin(); it != uniformsMeta.end(); it++) {
            if(uniforms.find(it.key()) == uniforms.end())
                uniforms[it.key()] = it.value();
        }

        printf("%s uniforms mapped\n", name.c_str());

        auto samplersMeta = meta["Samplers"];
        for(json::iterator it = samplersMeta.begin(); it != samplersMeta.end(); it++)
            samplers[it.key()]->LoadFromFile(it.value());

        printf("%s samplers initialized\n", name.c_str());

    }

    void OmicronMaterial::InvalidateSamplers() {
        for(auto pair : samplers)
            pair.second->Invalidate();
    }

    void OmicronMaterial::Update(float delta) {
        bool shouldRemove = false;
        for(auto value : interps) {
            if(!value->active) {
                shouldRemove = true;
                continue;
            }
            value->life -= delta;
            if(value->life < 0) {
                value->life = 0;
                value->active = false;
                uniforms[value->name] = value->target;
            }

            float perc = 1.f - (value->life / value->duration);
            value->value = (value->start + (value->target - value->start) * perc);
            uniforms[value->name] = value->value;
        }

        if(shouldRemove) {
            interps.erase(std::remove_if(interps.begin(), interps.end(), [](auto val) {
                return !val->active;
            }));
        }

    }

    void OmicronMaterial::AddInterpolatingValue(std::string name, float target, float duration) {
        InterpolatingValue* value = new InterpolatingValue;
        value->name = name;
        value->target = target;
        value->start = uniforms[name];
        value->value = uniforms[name];
        value->life = duration;
        value->duration = duration;
        value->active = true;
        value->hasCallback = false;
        interps.push_back(value);
    }

    void OmicronMaterial::AddInterpolatingValue_CB(std::string name, float target, float duration,
                                                std::function<void()> callback) {
        InterpolatingValue* value = new InterpolatingValue;
        value->name = name;
        value->target = target;
        value->start = uniforms[name];
        value->value = uniforms[name];
        value->life = duration;
        value->duration = duration;
        value->active = true;
        value->callback = callback;
        value->hasCallback = true;
        interps.push_back(value);
    }

    float OmicronMaterial::GetUniform(std::string name) {
        return uniforms[name];
    }

    void OmicronMaterial::SetUniform(std::string name, float value) {
        uniforms[name] = value;
    }

    OmicronMaterial::OmicronMaterial(OmicronMaterial& other) {
        ready.store(other.ready);
        name = other.name;
        shader = other.shader;
        hostPath = other.hostPath;
        includes = other.includes;
        uniforms = other.uniforms;
        samplers = other.samplers;
        fileContents = other.fileContents;
    }

    OmicronMaterial::OmicronMaterial(OmicronMaterial *other) {
        ready.store(other->ready);
        name = other->name;
        shader = other->shader;
        hostPath = other->hostPath;
        includes = other->includes;
        uniforms = other->uniforms;
        samplers = other->samplers;
        fileContents = other->fileContents;
    }

    const std::map<std::string, Texture*>& OmicronMaterial::GetSamplers() const {
        return samplers;
    }
}