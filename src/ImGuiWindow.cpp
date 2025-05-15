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
    const glm::vec3& cameraPosition,
    const std::vector<std::unique_ptr<Object>>& objects
)
{
    ImGui::Begin("Debug");

    ImGui::Text("Frame Duration: %.3f ms", static_cast<float>(frameDuration));
    ImGui::Text("FPS: %.1f", 1000.0f / static_cast<float>(frameDuration));

    ImGui::Text("Camera Position: (%.2f, %.2f, %.2f)", cameraPosition.x, cameraPosition.y, cameraPosition.z);

    ImGui::Text("Scene Objects:");
    std::unordered_map<std::string, int> objectCounts;
    for (size_t i = 0; i < objects.size(); ++i)
    {
        Object* object = objects[i].get();

        std::string objectName = object->getName();
        int count = ++objectCounts[objectName];

        std::string title = objectName + " " + std::to_string(count);

        if (ImGui::CollapsingHeader(title.c_str()))
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
        }
    }

    ImGui::End();
}