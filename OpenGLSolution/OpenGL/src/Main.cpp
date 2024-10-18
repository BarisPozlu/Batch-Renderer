#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include "Shader.h"
#include "Renderer.h"
#include "Texture.h"
#include "Debugging.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_opengl3.h"
#include "ImGui/imgui_impl_glfw.h"


int main()
{
    glfwInit();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    SETUP_GLFW_DEBUG_CONTEXT();
    
    GLFWwindow* window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
    glfwMakeContextCurrent(window);

    glewInit();
    SETUP_GL_DEBUG();

    Shader shader("rsc\\shaders\\Texture.shader");
    shader.Use();

  
    int MVPLoc = shader.GetUniformLocation("u_MVP");
    glm::mat4 proj = glm::ortho(0.0f, 640.0f, 0.0f, 480.0f);
    glUniformMatrix4fv(MVPLoc, 1, GL_FALSE, &proj[0][0]);

    Texture texture1("rsc\\images\\example.jpg");
    Texture texture2("rsc\\images\\photomode_03102024_134218.png");


    int location = shader.GetUniformLocation("u_texture");
    int texSlots[] = { 0, 1, 2 };
    glUniform1iv(location, 3, texSlots);

    Renderer renderer;
    renderer.Init();

    ImGui::CreateContext();
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init();

    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();

        if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
        {
            glfwSetWindowShouldClose(window, 1);
        }

        renderer.Clear();
        
        for (int i = 0; i < 5; i++)
        {
            for (int j = 0; j < 5; j++)
            {
                renderer.DrawQuad({ i * 100, j * 100 }, {50, 50}, (i + j) % 2 == 0 ? texture1.GetID() : texture2.GetID());
            }
        }

        renderer.DrawQuad({590, 0}, { 50, 50 }, texture1.GetID());

        renderer.Flush();
       
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        {
            ImGui::Begin("Hello, world!");

            ImGui::End();
        }

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
    } 

    renderer.ShutDown();

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();
}
