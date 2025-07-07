#include "ImGuiWindow.hpp"

ImGuiWindow::ImGuiWindow(GLFWwindow* window, const char* glslVersion)
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    (void)io;

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glslVersion);
    ImGui::StyleColorsDark();
}

void ImGuiWindow::newFrame()
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}

void ImGuiWindow::update()
{
}

void ImGuiWindow::render()
{
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void ImGuiWindow::close()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    std::cout << "ImGuiWindow closed.\n";
}

DebugWindow::DebugWindow(
    GLFWwindow* window,
    const char* glslVersion
)
    : ImGuiWindow(window, glslVersion)
{
}

void DebugWindow::update(
    int frameDuration,
    Scene& scene
)
{
    ImGui::Begin("Debug");
    Camera* camera = scene.getCamera();

    // performance
    ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "Performance");
    ImGui::Dummy(ImVec2(0.0f, 5.0f));
    ImGui::Text("Frame Duration: %.3f ms", static_cast<float>(frameDuration));
    ImGui::Text("FPS: %.1f", 1000.0f / static_cast<float>(frameDuration));
    ImGui::Separator();

    // camera
    ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "Camera");
    ImGui::Dummy(ImVec2(0.0f, 5.0f));
    const glm::vec3& cameraPosition = camera->getPosition();
    ImGui::Text("Pos: x = %.2f, y = %.2f, z = %.2f", cameraPosition.x, cameraPosition.y, cameraPosition.z);

    ImGui::Text("Reset Camera [C]");
    ImGui::SameLine();
    if (ImGui::Button("Reset##ResetCamera") || ImGui::IsKeyPressed(ImGuiKey_C))
    {
        camera->resetPosition();
    }

    ImGui::Separator();

    // external forces
    ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "External Forces");
    ImGui::Dummy(ImVec2(0.0f, 5.0f));
    glm::vec3& gravitationalAcceleration = scene.getGravitationalAcceleration();
    ImGui::Text("Gravity");
    ImGui::SameLine();
    ImGui::SliderFloat("##Gravity", &gravitationalAcceleration.y, -9.81f, 9.81f);
    ImGui::Separator();


    // XPBD Parameters
    ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "XPBD");
    ImGui::Dummy(ImVec2(0.0f, 5.0f));

    int& pbdSubsteps = scene.getPBDSubsteps();
    ImGui::Text("Substeps");
    ImGui::SameLine();
    ImGui::SliderInt("##Substeps n", &pbdSubsteps, 1, 100);

    static bool enableDistanceConstraints = true;
    ImGui::Checkbox("Enable Distance Constraints", &enableDistanceConstraints);
    scene.setEnableDistanceConstraints(enableDistanceConstraints);

    static bool enableVolumeConstraints = false;
    ImGui::Checkbox("Enable Volume Constraints", &enableVolumeConstraints);
    scene.setEnableVolumeConstraints(enableVolumeConstraints);

    ImGui::Dummy(ImVec2(0.0f, 5.0f));

    float& alpha = scene.getAlpha();
    ImGui::Text("alpha");
    ImGui::SameLine();
    ImGui::SliderFloat("##alpha", &alpha, 0.0f, 0.05f);

    ImGui::Dummy(ImVec2(0.0f, 5.0f));
    float& beta = scene.getBeta();
    ImGui::Text("beta");
    ImGui::SameLine();
    ImGui::SliderFloat("##beta", &beta, 1.0f, 10.0f);
    ImGui::Separator();

    // scene objects
    ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "Scene Objects:");
    ImGui::Dummy(ImVec2(0.0f, 5.0f));

    ImGui::Text("Reset Scene [S]");
    ImGui::SameLine();
    if (ImGui::Button("Reset##ResetScene")  || ImGui::IsKeyPressed(ImGuiKey_S))
    {
        for (auto& obj : scene.getObjects())
        {
            obj->resetVertexTransforms();
        }
    }

    const std::vector<std::unique_ptr<Object>>& objects = scene.getObjects();
    std::unordered_map<std::string, int> objectCounts;
    for (size_t i = 0; i < objects.size(); ++i)
    {
        Object* object = objects[i].get();

        std::string objectName = object->getName();
        int count = ++objectCounts[objectName];

        std::string title = objectName + " " + std::to_string(count);

        if (ImGui::CollapsingHeader(title.c_str()))
        {
            if (ImGui::TreeNode(("Vertex Transforms##" + std::to_string(i)).c_str()))
            {
                const auto& vertexTransforms = object->getVertexTransforms();
                ImGui::Separator();
                for (size_t j = 0; j < vertexTransforms.size(); ++j)
                {
                    const glm::vec3& position = vertexTransforms[j].getPosition();
                    const glm::vec3& velocity = vertexTransforms[j].getVelocity();
                    const glm::vec3& acceleration = vertexTransforms[j].getAcceleration();
                    ImGui::BulletText(
                        "Vertex %zu:\nPos: (%.2f, %.2f, %.2f)\nVel: (%.2f, %.2f, %.2f)\nAcc: (%.2f, %.2f, %.2f)",
                        j,
                        position.x, position.y, position.z,
                        velocity.x, velocity.y, velocity.z,
                        acceleration.x, acceleration.y, acceleration.z
                    );
                }

                ImGui::TreePop();
            }

            // if (ImGui::TreeNode(("XPBD##" + std::to_string(i)).c_str()))
            // {
            //     // Get lambdaDifference and positionDifference
            //     const std::vector<float>& lambdaDifference = object->getLambdaDifference();
            //     const std::vector<float>& positionDifference = object->getPositionDifference();

            //     // Display lambdaDifference
            //     ImGui::Text("Lambda Differences:");
            //     for (size_t j = 0; j < lambdaDifference.size(); ++j)
            //     {
            //         ImGui::BulletText("dLambda_%zu = %.6f", j, lambdaDifference[j]);
            //     }

            //     // Display positionDifference
            //     ImGui::Text("Position Differences:");
            //     for (size_t j = 0; j < positionDifference.size(); ++j)
            //     {
            //         ImGui::BulletText("dx_%zu = %.6f", j, positionDifference[j]);
            //     }

            //     ImGui::TreePop();
            // }

            if (ImGui::TreeNode(("Polygon Mode##" + std::to_string(i)).c_str()))
            {
                GLenum currentMode = object->getPolygonMode();
                if (ImGui::RadioButton(("Fill##" + std::to_string(i)).c_str(), currentMode == GL_FILL))
                {
                    object->setPolygonMode(GL_FILL);
                }

                if (ImGui::RadioButton(("Wireframe##" + std::to_string(i)).c_str(), currentMode == GL_LINE))
                {
                    object->setPolygonMode(GL_LINE);
                }

                if (ImGui::RadioButton(("Points##" + std::to_string(i)).c_str(), currentMode == GL_POINT))
                {
                    object->setPolygonMode(GL_POINT);
                }

                ImGui::TreePop();
            }



        }
    }

    ImGui::End();
}