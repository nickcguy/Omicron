#include <engine/OmicronEngineWrapper.hpp>
#include <render/OpenGLContext.hpp>
#include <window/GLFWWindowFactory.hpp>
#include <render/AggregateRenderProvider.hpp>
#include <engine/component/MeshComponent.hpp>
#include <engine/component/PointCloudComponent.hpp>
#include <engine/component/MaterialComponent.hpp>
#include <engine/system/PhysicsSystem.hpp>
#include <engine/component/PhysicsComponent.hpp>
#include <BulletCollision/CollisionShapes/btSphereShape.h>
#include <BulletCollision/CollisionShapes/btBoxShape.h>
#include <engine/system/PsychicInteractionSystem.hpp>
#include <script/ScriptHost.hpp>
#include <io/EngineLoader.hpp>
#include <render/ovr/OVRContext.hpp>

#define USE_SOURCE_WORLD false

using namespace Omicron;

bool useOVR = false;
#define OVR_FLAG "-ovr"

int main(int argc, char** argv) {

    for(int i = 0; i < argc; i++) {
        auto arg = argv[i];
        if(strcmp(arg, OVR_FLAG))
            useOVR = true;
    }

    std::cout << "Press any key to continue...";
    std::cin.ignore();
    if(useOVR) {
        printf("\"%s\" launch flag detected, initialising in OVR mode...\n", OVR_FLAG);
    }
    printf("Beginning startup...\n");

    #if USE_SOURCE_WORLD

    float worldScale = 1.f;

    glm::vec3 cubePositions[] = {
        glm::vec3( 0.0f,  0.0f,  0.0f),
        glm::vec3( 2.0f,  5.0f, -15.0f),
        glm::vec3(-1.5f, -2.2f, -2.5f),
        glm::vec3(-3.8f, -2.0f, -12.3f),
        glm::vec3( 2.4f, -0.4f, -3.5f),
        glm::vec3(-1.7f,  3.0f, -7.5f),
        glm::vec3( 1.3f, -2.0f, -2.5f),
        glm::vec3( 1.5f,  2.0f, -2.5f),
        glm::vec3( 1.5f,  0.2f, -1.5f),
        glm::vec3(-1.3f,  1.0f, -1.5f)
    };

    #endif

    GLFWWindowFactory factory;
    GLFWwindow* window = factory.SetVersion(4, 5)
            .SetTitle("Omicron Renderer")
            .SetProfile(GLFW_OPENGL_CORE_PROFILE)
            .SetResizable(true)
            .Build();

    DEBUG_PRINT(OmicronEngineWrapper engineWrapper("CoreEngine", 30));

    DEBUG_PRINT(AggregateRenderProvider renderProvider);
    DEBUG_PRINT(renderProvider.AddProvider(engineWrapper.GetChild()));
    OpenGLContext* context;

    if(useOVR) {
        context = new OVRContext(window, &renderProvider);
        context->Init();
        ((OVRContext*)context)->InitOVR();
    }else{

        context = new OpenGLContext(window, &renderProvider);
        context->Init();
    }

    DEBUG_PRINT(engineWrapper.GetChild()->SetInputProvider(context->inputProvider));

    OmicronEngine* engine = engineWrapper.GetChild();

    #if !USE_SOURCE_WORLD
    EngineLoader::LoadIntoEngine("engines/testEngine.xml", engine);
    if(ovr::OVRRenderer* renderer = dynamic_cast<ovr::OVRRenderer*>(context->renderer)) {
        std::vector<OmicronEntity*> leftHandEntities = engine->GetTaggedEntities_All({"ovrHand", "ovrHand_Left"});
        if(!leftHandEntities.empty()) {
            auto e = leftHandEntities[0];
            renderer->SetHandEntity(ovrHand_Left, e);
        }
        std::vector<OmicronEntity*> rightHandEntities = engine->GetTaggedEntities_All({"ovrHand", "ovrHand_Right"});
        if(!rightHandEntities.empty()) {
            auto e = rightHandEntities[0];
            renderer->SetHandEntity(ovrHand_Right, e);
        }

    }

    #else

    ScriptHost* scriptHost = new ScriptHost;
    scriptHost->RegisterScriptableAdapters();
    engineWrapper.GetChild()->AddSystem(scriptHost);
    scriptHost->InitLuaState();

    auto cubeVert = MeshFactory::CubeVertices();
    DEBUG_PRINT(auto sphereVertPair = MeshFactory::SphereVertices(64, 64));

    PhysicsSystem* physics = new PhysicsSystem;
    btSphereShape* sphereShape = new btSphereShape(1.f * worldScale);
    engineWrapper.GetChild()->AddSystem(physics);

    PyschicInteractionSystem* psychic = new PyschicInteractionSystem(physics);
    engineWrapper.GetChild()->AddSystem(psychic);

    for(int i = 0; i < 10; i++) {
        glm::vec3 pos = cubePositions[i];
        pos += glm::vec3(0.f, 1000.f, 0.f);
        OmicronEntity* entity = new OmicronEntity;
        entity->transform.Translation = pos;
        float angle = 20.0f * i;
        entity->transform.Rotation = {1.0f, 0.3f, 0.5f, glm::radians(angle)};
        entity->transform.SetScale(glm::vec3(worldScale));

        MeshComponent* mesh = new MeshComponent(sphereVertPair.first, sphereVertPair.second);
        entity->SetComponent<MeshComponent>(mesh);

        PhysicsComponent* physicsComponent = new PhysicsComponent(physics);

        btVector3 startPos(pos.x, pos.y, pos.z);
        btQuaternion startRot = {1.0f, 0.3f, 0.5f, glm::radians(angle)};
        btTransform startTransform(startRot, startPos);

        entity->SetComponent(physicsComponent);
//        physicsComponent->AddCollisionShape(sphereShape, startTransform, 1.0f);
        physicsComponent->MatchEntityMesh(1.f);
        physicsComponent->AddToWorld();

        if(i % 4 == 0) {
            MaterialComponent* mtlComp = new MaterialComponent("Rusted Iron");
            entity->SetComponent<MaterialComponent>(mtlComp);
        }else if(i % 3 == 0) {
            MaterialComponent* mtlComp = new MaterialComponent("Oak Floor");
            entity->SetComponent<MaterialComponent>(mtlComp);
        }else if(i % 2 == 0) {
            MaterialComponent* mtlComp = new MaterialComponent("Brick Wall");
            entity->SetComponent<MaterialComponent>(mtlComp);
        }

        engineWrapper.GetChild()->AddEntity(entity);
    }
    // Floor
    {
        glm::vec3 floorPos = {0.f, 0.f, 0.f};
        OmicronEntity* floorEntity = new OmicronEntity;
        floorEntity->transform.SetScale(glm::vec3(100.f, 1.f, 100.f) * worldScale);
        floorEntity->transform.SetTranslation(floorPos);
        MeshComponent* mesh = new MeshComponent(cubeVert);
        floorEntity->SetComponent<MeshComponent>(mesh);
        MaterialComponent* mtlComp = new MaterialComponent("Oak Floor");
        floorEntity->SetComponent<MaterialComponent>(mtlComp);
        PhysicsComponent* physicsComponent = new PhysicsComponent(physics);
        btVector3 startPos(floorPos.x, floorPos.y, floorPos.z);
        btQuaternion startRot;
        btTransform startTransform(startRot, startPos);
        floorEntity->SetComponent(physicsComponent);

        btVector3 halfExtents(100.f * worldScale, 1.f * worldScale, 100.f * worldScale);
        btBoxShape* boxShape = new btBoxShape(halfExtents);

//        physicsComponent->AddCollisionShape(boxShape, startTransform, 0.f);
        physicsComponent->MatchEntityMesh(0.f);
        physicsComponent->GetBody()->setCollisionFlags(btCollisionObject::CollisionFlags::CF_STATIC_OBJECT);
        physicsComponent->AddToWorld();
        engineWrapper.GetChild()->AddEntity(floorEntity);
    }

    ovrHandType hands[2] = {
        ovrHand_Left,
        ovrHand_Right
    };

    btVector3 halfExtents(0.1, 0.1, 0.1);
    btBoxShape* boxShape = new btBoxShape(halfExtents);

    for(int i = 0; i < 2; i++) {
        OmicronEntity* entity = new OmicronEntity;
        MeshComponent* mesh = new MeshComponent(cubeVert);
        entity->SetComponent<MeshComponent>(mesh);
        MaterialComponent* mtlComp = new MaterialComponent("Brick Wall");
        entity->SetComponent<MaterialComponent>(mtlComp);

        glm::vec3 pos = entity->transform.Translation;

        btVector3 startPos(pos.x, pos.y, pos.z);
        btQuaternion startRot;
        btTransform startTransform(startRot, startPos);

        PhysicsComponent* physicsComponent = new PhysicsComponent(physics);
        entity->SetComponent<PhysicsComponent>(physicsComponent);
        physicsComponent->AddCollisionShape(boxShape, startTransform);
        physicsComponent->AddToWorld();

        engineWrapper.GetChild()->AddEntity(entity);

        if(ovr::OVRRenderer* renderer = dynamic_cast<ovr::OVRRenderer*>(context.renderer))
            renderer->SetHandEntity(hands[i], entity);
    }

    #endif


//    OmicronEntity* pointCloudEntity = new OmicronEntity;
//    std::vector<MeshFactory::PointVertex> pointcloudPoints = PointCloudComponent::RandomPointCloud(1000000);
//    PointCloudComponent* comp = new PointCloudComponent(pointcloudPoints);
//    pointCloudEntity->SetComponent<PointCloudComponent>(comp);
//    engineWrapper.GetChild()->AddEntity(pointCloudEntity);

    context->Loop();

    engineWrapper.Stop();

    CLEAR_PTR(context);

    return 0;
}