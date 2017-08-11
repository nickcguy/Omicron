CMAKE_MINIMUM_REQUIRED(VERSION 3.7)
PROJECT(OmicronRender)

SET(INCLUDE_AVATAR FALSE)
SET(INCLUDE_ASSIMP TRUE)
SET(USE_LUAJIT FALSE)
SET(USE_LUABRIDGE FALSE)

SET(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)

ADD_DEFINITIONS("-DINCLUDE_AVATAR=${INCLUDE_AVATAR}")
ADD_DEFINITIONS("-DINCLUDE_ASSIMP=${INCLUDE_ASSIMP}")
ADD_DEFINITIONS("-DUSE_LUAJIT=${USE_LUAJIT}")
ADD_DEFINITIONS("-DUSE_LUABRIDGE=${USE_LUABRIDGE}")

INCLUDE(CMakeFunctions.cmake)

SET(CMAKE_CXX_STANDARD 14)
SET(CMAKE_EXE_LINKER_FLAGS "-static-libgcc -static-libstdc++")
#SET(CMAKE_BUILD_TYPE RelWithDebInfo)
SET(CMAKE_BUILD_TYPE Debug)
SET(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} -fpermissive)

SET(HEADER_FILES internal/include/render/OpenGLContext.hpp internal/include/window/GLFWWindowFactory.hpp internal/include/engine/OmicronEngine.hpp internal/include/engine/OmicronEngineWrapper.hpp internal/include/pch.hpp internal/include/render/IRenderProvider.hpp internal/include/engine/entity/OmicronEntity.hpp internal/include/render/RenderCommand.hpp internal/include/render/AggregateRenderProvider.hpp internal/include/render/shader/Shader.hpp internal/include/render/OpenGLRenderer.hpp internal/include/utils/FileUtils.hpp internal/include/render/camera/Camera.hpp internal/include/engine/component/OmicronComponent.hpp internal/include/engine/component/MeshComponent.hpp internal/include/engine/mesh/MeshFactory.hpp internal/include/utils/FilteredVector.hpp internal/include/utils/TextUtils.hpp internal/include/engine/component/PointCloudComponent.hpp internal/include/utils/MathsUtils.hpp internal/include/data/UBO.hpp internal/include/data/SSBO.hpp internal/include/data/material/OmicronMaterial.hpp internal/include/data/material/Texture.hpp internal/include/data/material/MaterialManager.hpp internal/include/engine/component/MaterialComponent.hpp external/stb/ctpl_stl.h internal/include/render/ovr/OVRRenderer.hpp internal/include/render/camera/RollCamera.hpp internal/include/render/ovr/TextureBuffer.hpp internal/include/render/ovr/OVRCMDProcessor.hpp internal/include/data/Framebuffer.hpp internal/include/render/PrimitiveRenderer.hpp internal/include/engine/system/OmicronSystem.hpp internal/include/engine/system/PhysicsSystem.hpp internal/include/engine/component/PhysicsComponent.hpp internal/include/engine/system/OmicronMotionState.hpp internal/include/engine/input/OVRInputProvider.hpp internal/include/engine/input/InputProvider.hpp internal/include/engine/system/PsychicInteractionSystem.hpp internal/include/script/IScriptable.hpp internal/include/script/ScriptHost.hpp internal/include/script/adapters/OmicronMaterialAdapter.hpp internal/include/script/adapters/SystemAdapter.hpp internal/include/io/EngineLoader.hpp internal/include/engine/system/MaterialSystem.hpp internal/include/data/assimp/AssimpData.hpp internal/include/data/assimp/AssimpModel.hpp internal/include/script/ScriptComponent.hpp internal/include/script/adapters/OmicronEngineAccessor.hpp internal/include/script/adapters/OmicronEntityAccessor.hpp internal/include/script/adapters/componentAdapters/MaterialComponentAccessor.hpp  internal/include/script/adapters/componentAdapters/PhysicsComponentAccessor.hpp)
SET(SOURCE_FILES internal/source/window/GLFWWindowFactory.cpp internal/source/render/OpenGLContext.cpp internal/source/engine/OmicronEngine.cpp internal/source/engine/OmicronEngineWrapper.cpp internal/source/render/AggregateRenderProvider.cpp internal/source/render/IRenderProvider.cpp internal/source/render/shader/Shader.cpp internal/source/render/OpenGLRenderer.cpp internal/source/utils/FileUtils.cpp internal/source/render/RenderCommand.cpp internal/source/render/camera/Camera.cpp internal/source/engine/mesh/MeshFactory.cpp internal/source/engine/component/MeshComponent.cpp internal/source/utils/FilteredVector.cpp internal/include/engine/entity/Transform.hpp internal/source/engine/component/PointCloudComponent.cpp internal/source/utils/MathsUtils.cpp internal/source/data/UBO.cpp internal/source/data/SSBO.cpp internal/source/data/material/OmicronMaterial.cpp internal/source/data/material/Texture.cpp internal/source/data/material/MaterialManager.cpp internal/source/engine/component/MaterialComponent.cpp internal/source/utils/TextUtils.cpp internal/source/render/ovr/OVRRenderer.cpp internal/source/render/camera/RollCamera.cpp internal/source/render/ovr/OVRCMDProcessor.cpp internal/source/data/Framebuffer.cpp internal/source/render/PrimitiveRenderer.cpp internal/source/engine/system/OmicronSystem.cpp internal/source/engine/component/PhysicsComponent.cpp internal/source/engine/system/PhysicsSystem.cpp internal/source/engine/system/OmicronMotionState.cpp internal/source/engine/system/PsychicInteractionSystem.cpp internal/source/script/ScriptHost.cpp internal/source/script/adapters/OmicronMaterialAdapter.cpp internal/source/script/adapters/SystemAdapter.cpp internal/source/io/EngineLoader.cpp internal/source/engine/system/MaterialSystem.cpp internal/source/pch.cpp internal/source/data/assimp/AssimpModel.cpp internal/source/script/adapters/OmicronEngineAccessor.cpp internal/source/script/adapters/OmicronEntityAccessor.cpp internal/source/script/adapters/componentAdapters/MaterialComponentAccessor.cpp internal/source/script/IScriptable.cpp internal/source/script/adapters/componentAdapters/PhysicsComponentAccessor.cpp internal/source/script/adapters/componentAdapters/EntityComponentAccessor.cpp)

IF(INCLUDE_AVATAR)
    SET(HEADER_FILES ${HEADER_FILES} internal/include/render/ovr/OVRAvatar.hpp)
    SET(SOURCE_FILES ${SOURCE_FILES} internal/source/render/ovr/OVRAvatar.cpp)
ENDIF()

INCLUDE_DIRECTORIES(external/glfw/include)
INCLUDE_DIRECTORIES(external/glm)
INCLUDE_DIRECTORIES(external/stb)
INCLUDE_DIRECTORIES(external/json)
INCLUDE_DIRECTORIES(internal/include)

SET(OpenCL_INCLUDE_DIR external/OpenCL/include)
find_package(OpenCL REQUIRED)
message("OpenCL LIB: ${OpenCL_LIBRARIES}")
message("OpenCL INC: ${OpenCL_INCLUDE_DIRS}")
INCLUDE_DIRECTORIES(${OpenCL_INCLUDE_DIRS})

ADD_SOURCE_LIBRARY(external/stb external/stb)
ADD_SOURCE_LIBRARY(external/Bullet/src external/Bullet/src)
ADD_SOURCE_LIBRARY(external/glad/include external/glad/src)
ADD_SOURCE_LIBRARY(external/OculusSDK/LibOVR/Include external/OculusSDK/LibOVR/Src)
ADD_SOURCE_LIBRARY(external/tinyXML/Source external/tinyXML/Source)

IF(INCLUDE_AVATAR)
    INCLUDE_DIRECTORIES(external/OVRAvatarSDK/Include)
ENDIF()

IF(INCLUDE_ASSIMP)
    INCLUDE_DIRECTORIES(external/Assimp/include)
ENDIF()

INCLUDE_DIRECTORIES(external/OVRPlatformSDK/Include)

IF(USE_LUABRIDGE)
    INCLUDE_DIRECTORIES(external/LuaBridge/Source)
ELSE()
    INCLUDE_DIRECTORIES(external/Selene/include)
ENDIF()

IF(USE_LUAJIT)
    INCLUDE_DIRECTORIES(external/LuaJit/src)
ELSE()
    INCLUDE_DIRECTORIES(external/Lua/src)
ENDIF()

SET(SOURCE_FILES ${SOURCE_FILES} external/OVRPlatformSDK/Windows/OVR_PlatformLoader.cpp)

ADD_EXECUTABLE(OmicronRender main.cpp ${SOURCE_FILES} ${HEADER_FILES})

ADD_CUSTOM_COMMAND(TARGET OmicronRender PRE_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy_directory
                   ${CMAKE_SOURCE_DIR}/_assets $<TARGET_FILE_DIR:OmicronRender>/assets)

ADD_CUSTOM_COMMAND(TARGET OmicronRender PRE_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy_directory
                   ${CMAKE_SOURCE_DIR}/_content $<TARGET_FILE_DIR:OmicronRender>)

ADD_CUSTOM_COMMAND(TARGET OmicronRender PRE_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy_directory
                   ${CMAKE_SOURCE_DIR}/_runtime $<TARGET_FILE_DIR:OmicronRender>)

TARGET_LINK_LIBRARIES(OmicronRender ${CMAKE_BINARY_DIR}/libglfw3.a)
TARGET_LINK_LIBRARIES(OmicronRender ${CMAKE_BINARY_DIR}/LibOVRPlatform64_1.lib)
TARGET_LINK_LIBRARIES(OmicronRender ${OpenCL_LIBRARIES})
IF(USE_LUAJIT)
    TARGET_LINK_LIBRARIES(OmicronRender ${CMAKE_BINARY_DIR}/lua51.dll)
ELSE()
    TARGET_LINK_LIBRARIES(OmicronRender ${CMAKE_BINARY_DIR}/lua53.dll)
ENDIF()

IF(INCLUDE_AVATAR)
    TARGET_LINK_LIBRARIES(OmicronRender ${CMAKE_BINARY_DIR}/libovravatar.lib)
ENDIF()
IF(INCLUDE_ASSIMP)
    TARGET_LINK_LIBRARIES(OmicronRender ${CMAKE_BINARY_DIR}/libassimp.dll)
ENDIF()