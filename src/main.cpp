#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <cstdlib>
#define GLM_FORCE_RADIAN
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <fmt/format.h>
#include <imgui.h>
#include <memory>
#include "imgui_impl_opengl3.h"
#include "imgui_impl_glfw.h"
#include "WenCheng.h"


static void error_callback(int error, const char* description)
{
    std::cerr<<fmt::format("Error: {0}\n", description);
}

int main(void)
{
	// Open window
    GLFWwindow* window;
    glfwSetErrorCallback(error_callback);
    if (!glfwInit())
        exit(EXIT_FAILURE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    window = glfwCreateWindow(1024, 768, "Simple example", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }
    glfwMakeContextCurrent(window);
    if(!gladLoadGL()) {
        exit(EXIT_FAILURE);
    }
    // Setup ImGui binding
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;  // Enable Keyboard Controls
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;   // Enable Gamepad Controls

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");

    // Setup style
    ImGui::StyleColorsDark();

    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

	// Load textures and meshes
	auto text = Texture2D::LoadFromFile("../resource/image.png");
    auto flagpoleMesh = StaticMesh::LoadMesh("../resource/flagpole.obj", false);
	auto lightMesh = StaticMesh::LoadMesh("../resource/sphere.obj", false);

	// Create vertex and fragment shader programs
    auto prog = Program::LoadFromFile(
        "../resource/vs.vert",
        "../resource/gs.geom",
        "../resource/fs.frag"
    );
    auto prog2 = Program::LoadFromFile(
        "../resource/vs.vert",
        "../resource/gs.geom",
        "../resource/fs_light.frag"
    );


    {
		// text and mesh, shader => garbage collector
		auto g1 = Protect(text);
		auto g2 = Protect(flagpoleMesh);
		auto g3 = Protect(prog);
		auto g4 = Protect(lightMesh);

		if(!flagpoleMesh.hasUV()) {
			std::cerr<<"Mesh does not have uv\n";
			//glfwSetWindowShouldClose(window, GLFW_TRUE);
		}
		if(!flagpoleMesh.hasNormal()) {
			std::cerr << "Mesh does not have normal\n";
			//glfwSetWindowShouldClose(window, GLFW_TRUE);
		}

		float degree = 0.0f;
		float shininess = 1.0f;
		glm::vec3 object_color{1.0f};
		glm::vec3 light_color{ 1.0f };
		bool flat_shading = false;
		bool prev_flat = false;
		glm::vec3 light_pos;
		bool blinn_phong_on = false;

		while (!glfwWindowShouldClose(window))
		{
			glfwPollEvents();
			int display_w, display_h;
			glfwGetFramebufferSize(window, &display_w, &display_h);
			glViewport(0, 0, display_w, display_h);
			glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			glEnable(GL_DEPTH_TEST);

			prog["vp"] = glm::perspective(45/180.0f*3.1415926f, 1024.0f/768.0f, 0.1f, 10000.0f)*
				glm::lookAt(glm::vec3{0, 0, 10}, glm::vec3{0, 0, 0}, glm::vec3{0, 1, 0});
			prog["model"] = glm::rotate(glm::mat4(1.0f), degree*3.1415926f/180.0f, glm::vec3(0, 1, 0)) * glm::scale(glm::mat4(1.0f), glm::vec3(0.14f, 0.18f, 0.14f));
			prog["object_color"] = object_color;
			prog["light_pos"] = light_pos;
			prog["eye_pos"] = glm::vec3{0, 0, 10};
			prog["blinn_phong_on"] = blinn_phong_on;
			prog["light_color"] = light_color;
			prog["shininess"] = shininess;
			prog["text"] = 0;
			text.bindToChannel(0);
			prog.use();
			prog["flat_shading"] = static_cast<int>(flat_shading);

			prog.use();
			flagpoleMesh.draw();
		
			// Program 2 uses fs_light.frag, which is, the sun
			{
				prog2["vp"] = glm::perspective(45/180.0f*3.1415926f, 1024.0f/768.0f, 0.1f, 10000.0f)*
				glm::lookAt(glm::vec3{0, 0, 10}, glm::vec3{0, 0, 0}, glm::vec3{0, 1, 0});
				// point light
				prog2["model"] = glm::translate(glm::mat4(1.0f), light_pos)*glm::scale(glm::mat4(1.0f), glm::vec3{0.2f});
				prog["flat_shading"] = 0;

				prog2.use();
				lightMesh.draw();
			}

			glDisable(GL_DEPTH_TEST);

			// Start the Dear ImGui frame
			ImGui_ImplOpenGL3_NewFrame();
			ImGui_ImplGlfw_NewFrame();
			ImGui::NewFrame();

			bool new_shader = false;
			// 2. Show a simple window that we create ourselves. We use a Begin/End pair to created a named window.
			{
				static int counter = 0;

				ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

				ImGui::SliderFloat("degree", &degree, 0.0f, 360.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
				ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color
				ImGui::ColorEdit3("object color", glm::value_ptr(object_color)); // Edit 3 floats representing a color
				ImGui::ColorEdit3("light color", glm::value_ptr(light_color));
				ImGui::SliderFloat("shininess", &shininess, 1.0f, 100.0f);
				ImGui::SliderFloat3("Position", glm::value_ptr(light_pos), -10, 10);
				if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
					counter++;
				ImGui::SameLine();
				ImGui::Text("counter = %d", counter);
				ImGui::Checkbox("Flat Shading", &flat_shading);
				ImGui::Checkbox("Blinn Phong", &blinn_phong_on);

				ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
				if(ImGui::Button("Reload Shader")) {
					auto new_prog = Program::LoadFromFile("../resource/vs.vert", "../resource/fs.frag");
					// because we would like to switch value of prog
					// we need to release object on our own.
					prog.release();
					prog = new_prog;
				}
				ImGui::End();
			}
        
			// Rendering
			ImGui::Render();
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

			glfwSwapBuffers(window);
		}
    }
    glfwDestroyWindow(window);
    glfwTerminate();
    exit(EXIT_SUCCESS);
}