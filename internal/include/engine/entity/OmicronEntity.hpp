//
// Created by Guy on 06/08/2017.
//

#ifndef OMICRONRENDER_OMICRONENTITY_HPP
#define OMICRONRENDER_OMICRONENTITY_HPP

#include <typeindex>
#include <typeinfo>
#include <map>
#include <engine/component/OmicronComponent.hpp>
#include <pch.hpp>
#include <list>
#include <vector>
#include "Transform.hpp"
#include <algorithm>
#include <sstream>

namespace Omicron {


    class OmicronEntity {
    public:

        inline OmicronEntity() = default;

        inline OmicronEntity(OmicronEntity& other) : OmicronEntity(&other) {}
        inline OmicronEntity(OmicronEntity* other) {
            this->transform = other->transform;
            this->tags = other->tags;
            for(auto item : other->componentMap)
                SetComponent(item.first, item.second->Clone());
        }

        inline ~OmicronEntity() {
            for(auto iter = componentMap.begin(); iter != componentMap.end(); iter++)
                delete iter->second;
            componentMap.clear();
        }

        Transform transform;

        template <typename T> inline T* GetCastComponent() {
            return static_cast<T*>(GetComponent<T>());
        }

        template <typename T> inline OmicronComponent* GetComponent() {
            if(!HasComponent<T>())
                return nullptr;
            return componentMap[typeid(T)];
        }

        inline OmicronComponent* GetComponentByName(std::string name) {
            for(auto item : componentMap) {
                if(item.second->Name() == name)
                    return item.second;
            }
            return nullptr;
        }

        template <typename T> inline void SetComponent(T* component) {
            if(HasComponent<T>()) {
                CLEAR_PTR(componentMap[typeid(T)]);
            }
            componentMap[typeid(T)] = component;
            componentNames.insert(componentNames.end(), typeid(T).name());
            component->owningEntity = this;
        }

        template <typename T> inline T* RemoveComponent(bool keepComponent = true) {
            if(HasComponent<T>()) {
                componentNames.remove(typeid(T).name());
                if(keepComponent) {
                    auto comp = componentMap[typeid(T)];
                    componentMap[typeid(T)] = nullptr;
                    return comp;
                }
                CLEAR_PTR(componentMap[typeid(T)]);
            }
            return nullptr;
        }

        template <typename T> inline bool HasComponent() {
            return componentMap.find(typeid(T)) != componentMap.end();
        }

        inline bool HasTag(const std::string& tag) {
            for(std::string t : tags)
                if(t == tag) return true;
            return false;
        }

        inline bool HasAllTags(std::vector<std::string> tags) {
            for(std::string t : tags) {
                if(!HasTag(t)) return false;
            }
            return true;
        }

        inline void AddTag(const std::string& tag) {
            if(!HasTag(tag))
                tags.push_back(tag);
        }
        inline void RemoveTag(const std::string& tag) {
            if(HasTag(tag))
                tags.erase(std::remove(tags.begin(), tags.end(), tag), tags.end());
        }

        inline std::string TagString() {
            std::stringstream ss;

            ss << "[";
            for(std::string tag : tags)
                ss << "\t" << tag << "\n";
            ss << "]";

            return ss.str();
        }

        inline std::vector<std::string> GetTags() {
            return tags;
        }

    protected:

        inline void SetComponent(std::type_index id, OmicronComponent* component) {
            componentMap[id] = component;
        }

        std::vector<std::string> tags;

        std::map<std::type_index, OmicronComponent*> componentMap;
        std::list<const char*> componentNames;
    };

};

#endif //OMICRONRENDER_OMICRONENTITY_HPP
