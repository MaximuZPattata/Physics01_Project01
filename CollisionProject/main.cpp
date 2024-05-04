#include <cControlGameEngine.h>
#include "GLWF_CallBacks.h"
#include "cMesh.h"
#include "cJsonReader.h"
#include "cModelManipulator.h"

#include <iostream>
#include <vector>

//--------------------------------Global Objects--------------------------------------------

GLFWwindow* window;
cControlGameEngine gameEngine;
cJsonReader jsonReader;
sCameraDetailsFromFile camDetails;
cModelManipulator modelManip;


std::vector<sModelDetailsFromFile> modelDetailsList;
std::vector<sLightDetailsFromFile> lightDetailsList;
std::vector<sPhysicsDetailsFromFile> physicsDetailsList;

int main()
{
    int result = 0;

    //--------------------------------Initialize Game Engine----------------------------------------

    window = gameEngine.InitializeGameEngine();

    if (window == NULL)
        exit(EXIT_FAILURE);

    //--------------------------------Loading Models, Lights and initial camera position from Json file---------------------------------------------

    modelManip.Initialize(&gameEngine);

    bool jsonresult = jsonReader.ReadScene("SceneDescription.json", modelDetailsList, physicsDetailsList, lightDetailsList, camDetails);

    if (jsonresult)
    {
        std::cout << "File read successfully !" << std::endl;

        std::string modelName;
        int lightId;

        // Loading Models
        for (int index = 0; index < modelDetailsList.size(); index++)
        {
            modelName = modelDetailsList[index].modelName;

            gameEngine.LoadModelsInto3DSpace(modelDetailsList[index].modelFilePath, modelName,
                modelDetailsList[index].modelPosition.x, modelDetailsList[index].modelPosition.y, modelDetailsList[index].modelPosition.z);

            float angleRadians = glm::radians(modelDetailsList[index].modelOrientation.w);

            gameEngine.RotateMeshModel(modelName, angleRadians, modelDetailsList[index].modelOrientation.x,
                modelDetailsList[index].modelOrientation.y, modelDetailsList[index].modelOrientation.z);

            gameEngine.ScaleModel(modelName, modelDetailsList[index].modelScaleValue);

            if (modelDetailsList[index].wireframeModeOn)
                gameEngine.TurnWireframeModeOn(modelName);

            if (modelDetailsList[index].meshLightsOn)
                gameEngine.TurnMeshLightsOn(modelName);

            if (modelDetailsList[index].manualColors)
            {
                gameEngine.UseManualColors(modelName, true);
                gameEngine.ChangeColor(modelName, modelDetailsList[index].modelColorRGB.x, modelDetailsList[index].modelColorRGB.y, modelDetailsList[index].modelColorRGB.z);
            }

            //------------------------------Adding Physics----------------------------------------------

            if (modelDetailsList[index].physicsMeshType == "Sphere") // Sphere Physics
            {
                for (int index = 0; index < physicsDetailsList.size(); index++)
                {
                    if (physicsDetailsList[index].modelName == modelName)
                    {
                        gameEngine.AddSpherePhysicsToMesh(modelName, modelDetailsList[index].physicsMeshType, physicsDetailsList[index].modelRadius);

                        if (physicsDetailsList[index].randomVelocity)
                        {
                            float randomVelocity = gameEngine.getRandomFloat(0.7, 5.0);

                            gameEngine.ChangeModelPhysicsVelocity(modelName, glm::vec3(0.0f, -5.0f, 0.0f));
                        }
                        else
                            gameEngine.ChangeModelPhysicsVelocity(modelName, glm::vec3(physicsDetailsList[index].modelVelocity.x, physicsDetailsList[index].modelVelocity.y,
                                physicsDetailsList[index].modelVelocity.z));

                        gameEngine.ChangeModelPhysicsAcceleration(modelName, glm::vec3(physicsDetailsList[index].modelAcceleration.x, physicsDetailsList[index].modelAcceleration.y / 50.0,
                            physicsDetailsList[index].modelAcceleration.z));

                        result = gameEngine.ChangeModelPhysicalMass(modelName, physicsDetailsList[index].modelMass);

                        if (result != 0)
                            std::cout << "Mass provided for the sphere - '" << modelName << "' is not above 0. Hence default mass 1.0/10.0 is used for this sphere model." << std::endl;
                    }

                }
            }
            else if (modelDetailsList[index].physicsMeshType == "Plane" || modelDetailsList[index].physicsMeshType == "Box") // Plane Physics
            {
                gameEngine.AddPlanePhysicsToMesh(modelName, modelDetailsList[index].physicsMeshType);
            }
            else
                std::cout << "The Physics mesh type cannot be identified for the model name - " << modelDetailsList[index].modelName << std::endl;
        }

        // Loading Lights
        for (int index = 0; index < lightDetailsList.size(); index++)
        {
            lightId = lightDetailsList[index].lightId;

            gameEngine.CreateLight(lightId, lightDetailsList[index].lightPosition.x, lightDetailsList[index].lightPosition.y, lightDetailsList[index].lightPosition.z);
            gameEngine.ChangeLightType(lightId, lightDetailsList[index].lightType);
            gameEngine.ChangeLightIntensity(lightId, lightDetailsList[index].linearAttenuation, lightDetailsList[index].quadraticAttenuation);
            gameEngine.ChangeLightDirection(lightId, lightDetailsList[index].lightDirection.x, lightDetailsList[index].lightDirection.y, lightDetailsList[index].lightDirection.z);
            gameEngine.ChangeLightColour(lightId, lightDetailsList[index].lightColorRGB.r, lightDetailsList[index].lightColorRGB.g, lightDetailsList[index].lightColorRGB.b);
            gameEngine.ChangeLightAngle(lightId, lightDetailsList[index].innerAngle, lightDetailsList[index].outerAngle);

            if (lightDetailsList[index].lightOn)
                gameEngine.TurnOffLight(lightId, false);
            else
                gameEngine.TurnOffLight(lightId, true);
        }

        // Loading Initial Camera Position
        gameEngine.MoveCameraPosition(camDetails.initialCameraPosition.x, camDetails.initialCameraPosition.y, camDetails.initialCameraPosition.z);
    }
    else
        return -1;
    //-------------------------------Frame loop---------------------------------------------

    double lastTime = glfwGetTime();

    while (!glfwWindowShouldClose(window))
    {
        //-------------------Calculate Delta time-------------------------------------------

        double currentTime = glfwGetTime();
        gameEngine.deltaTime = currentTime - lastTime;

        const double LARGEST_DELTA_TIME = 1.0f / 30.0f;

        if (gameEngine.deltaTime > LARGEST_DELTA_TIME)
            gameEngine.deltaTime = LARGEST_DELTA_TIME;

        lastTime = currentTime;

        //------------------Calculate Physics-----------------------------------------------

        gameEngine.ComparePhysicalAttributesWithOtherModels();

        gameEngine.AnimateTheCubes();

        //--------------------Run Engine----------------------------------------------------

        gameEngine.RunGameEngine(window);
    }

    glfwDestroyWindow(window);

    glfwTerminate();

    exit(EXIT_SUCCESS);

    std::cout << "Window closed !" << std::endl;

    return 0;
}