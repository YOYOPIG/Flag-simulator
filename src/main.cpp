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
#include "Camera.h"
#include "Flag.h"
#include "Physics.h"
#include <queue>
#include "Spring.h"


static void error_callback(int error, const char* description)
{
    std::cerr<<fmt::format("Error: {0}\n", description);
}

GLFWwindow* window;

glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 10.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -3.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
TPCamera TPcamera(cameraPos, cameraUp);
float dt = 0;
float last_frame = 0;

int input_mode = 0;

float press_delay = 0;

void processInput()
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		TPcamera.processKeyboard(FORWARD, dt);
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		TPcamera.processKeyboard(BACKWARD, dt);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		TPcamera.processKeyboard(LEFT, dt);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		TPcamera.processKeyboard(RIGHT, dt);
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_RELEASE && glfwGetKey(window, GLFW_KEY_S) == GLFW_RELEASE && glfwGetKey(window, GLFW_KEY_A) == GLFW_RELEASE && glfwGetKey(window, GLFW_KEY_D) == GLFW_RELEASE) {
		TPcamera._v.x *= 0.5;
		TPcamera._v.z *= 0.5;
	}
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
		TPcamera.processKeyboard(SPACE, dt);
	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
		TPcamera.processKeyboard(SHIFT, dt);
	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS){
		float current_time = glfwGetTime();
		if (current_time - press_delay >= 0.2){
			press_delay = current_time;
			if (input_mode == 0) {
				glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
				input_mode = 1;
			} else if (input_mode == 1){
				glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
				input_mode = 0;
			}
		}
	}
}

bool firstMouse = true;
float lastX = 1080 / 2, lastY = 760 / 2;

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

								  //	std::cerr << "xoffset : " << xoffset << std::endl;

	lastX = xpos;
	lastY = ypos;
	TPcamera.processMouseEvent(xoffset, yoffset);
}

void drawAABB(AABB _aabb, unsigned int vbo, unsigned int vao) {
		/////////////////////////
	float aabb[54] = {
		_aabb._left_down_back.x, _aabb._left_down_back.y, _aabb._left_down_back.z,
		_aabb._right_top_front.x, _aabb._left_down_back.y, _aabb._left_down_back.z,
		_aabb._right_top_front.x, _aabb._right_top_front.y, _aabb._left_down_back.z,
		_aabb._left_down_back.x,_aabb._right_top_front.y, _aabb._left_down_back.z,
		_aabb._left_down_back.x, _aabb._left_down_back.y, _aabb._left_down_back.z,
		_aabb._left_down_back.x,_aabb._right_top_front.y, _aabb._left_down_back.z,
		_aabb._left_down_back.x, _aabb._right_top_front.y, _aabb._right_top_front.z,
		_aabb._left_down_back.x, _aabb._left_down_back.y, _aabb._right_top_front.z,
		_aabb._left_down_back.x, _aabb._left_down_back.y, _aabb._left_down_back.z,
		_aabb._right_top_front.x, _aabb._left_down_back.y, _aabb._left_down_back.z,
		_aabb._right_top_front.x, _aabb._right_top_front.y, _aabb._left_down_back.z,
		_aabb._right_top_front.x, _aabb._right_top_front.y, _aabb._right_top_front.z,
		_aabb._right_top_front.x, _aabb._left_down_back.y, _aabb._right_top_front.z,
		_aabb._right_top_front.x, _aabb._left_down_back.y, _aabb._left_down_back.z,
		_aabb._right_top_front.x, _aabb._left_down_back.y, _aabb._right_top_front.z,
		_aabb._left_down_back.x, _aabb._left_down_back.y, _aabb._right_top_front.z,
		_aabb._left_down_back.x, _aabb._right_top_front.y, _aabb._right_top_front.z,
		_aabb._right_top_front.x, _aabb._right_top_front.y, _aabb._right_top_front.z,
	};
	/////////////////////////
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(aabb), aabb, GL_STATIC_DRAW);
	glBindVertexArray(vao);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 18);
	glBindVertexArray(0);
	///////////////////////////
}

Program prog2;
unsigned int aabb_vao;
unsigned int aabb_vbo;

void aabbDebuger() {
	//////////////////////////////////////////////////////////////////////draw aabb

		int count = 0;
		prog2["vp"] = glm::perspective(45/180.0f*3.1415926f, 1024.0f/768.0f, 0.1f, 10000.0f)*TPcamera.getViewMatrix();
		prog2["object_color"] = glm::vec3(1.0, 0.0, 0.0);
		prog2.use();
		
		AABBNode *current = ObjectManager::aabb_tree._root;
		std::queue<AABBNode *> que;
		que.push(current);
		while (!que.empty()) {
			drawAABB(current->_aabb, aabb_vbo, aabb_vao);
			if (!current->isLeaf()) {
				que.push(current->_children[0]);
				que.push(current->_children[1]);
			}
			current = que.front();
			que.pop();
		}

	//////////////////////////////////////////////////////////////////////draw aabb
}

void initialAABB(){
	float vertices[] = {
		-0.5f, -0.5f, -0.5f,
		0.5f,  0.5f, -0.5f,
		0.5f, -0.5f, -0.5f,
		0.5f,  0.5f, -0.5f,
		-0.5f, -0.5f, -0.5f,
		-0.5f,  0.5f, -0.5f,

		-0.5f, -0.5f,  0.5f,
		0.5f, -0.5f,  0.5f,
		0.5f,  0.5f,  0.5f,
		0.5f,  0.5f,  0.5f,
		-0.5f,  0.5f,  0.5f,
		-0.5f, -0.5f,  0.5f,

		-0.5f,  0.5f,  0.5f,
		-0.5f,  0.5f, -0.5f,
		-0.5f, -0.5f, -0.5f,
		-0.5f, -0.5f, -0.5f,
		-0.5f, -0.5f,  0.5f,
		-0.5f,  0.5f,  0.5f,

		0.5f,  0.5f,  0.5f,
		0.5f, -0.5f, -0.5f,
		0.5f,  0.5f, -0.5f,
		0.5f, -0.5f, -0.5f,
		0.5f,  0.5f,  0.5f,
		0.5f, -0.5f,  0.5f,

		-0.5f, -0.5f, -0.5f,
		0.5f, -0.5f, -0.5f,
		0.5f, -0.5f,  0.5f,
		0.5f, -0.5f,  0.5f,
		-0.5f, -0.5f,  0.5f,
		-0.5f, -0.5f, -0.5f,

		-0.5f,  0.5f, -0.5f,
		0.5f,  0.5f,  0.5f,
		0.5f,  0.5f, -0.5f,
		0.5f,  0.5f,  0.5f,
		-0.5f,  0.5f, -0.5f,
		-0.5f,  0.5f,  0.5f,

	};
    glGenBuffers(1, &aabb_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, aabb_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glGenVertexArrays(1, &aabb_vao);
	glBindVertexArray(aabb_vao);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
}

void initialSpring(Spring **spring_list, Entity **entity_list, float k, float d){
	for (int i = 0; i < 15; ++i){
		for (int j = 0; j < 19; ++j){
			spring_list[i * 19 + j]->setEndPoint(entity_list[i * 20 + j], entity_list[i * 20 + (j + 1)]);
			spring_list[i * 19 + j]->setCoefficient(k, d);
		}
	}
	for (int i = 0; i < 14; ++i){
		for (int j = 0; j < 20; ++j){
			spring_list[i * 20 + j + 285]->setEndPoint(entity_list[i * 20 + j], entity_list[(i + 1) * 20 + j]);
			spring_list[i * 20 + j + 285]->setCoefficient(k, d);
		}
	}
	for (int i = 0; i < 14; ++i){
		for (int j = 0; j < 19; ++j){
			spring_list[(i * 19 + j) * 2 + 565]->setEndPoint(entity_list[i * 20 + j], entity_list[(i + 1) * 20 + (j + 1)]);
			spring_list[(i * 19 + j) * 2 + 565]->setCoefficient(k, d);
			spring_list[(i * 19 + j) * 2 + 566]->setEndPoint(entity_list[(i + 1) * 20 + j], entity_list[i * 20 + (j + 1)]);
			spring_list[(i * 19 + j) * 2 + 566]->setCoefficient(k, d);
		}
	}
	Entity *pole1 = new Entity();
	Entity *pole2 = new Entity();
	pole1->rigid.setPosition(-1.1, 1, 0);
	pole2->rigid.setPosition(-1.1, -0.4, 0);
	pole1->rigid._mass = 10000;
	pole2->rigid._mass = 10000;
	spring_list[1097]->setEndPoint(pole1, entity_list[0]);
	spring_list[1098]->setEndPoint(pole2, entity_list[280]);
	spring_list[1097]->setCoefficient(10, 1);
	spring_list[1098]->setCoefficient(10, 1);
}

void springUpdate(Spring **spring_list){
	for (int i = 0; i < 1099; ++i){
		Physics::calcSpring(spring_list[i], dt);
	}
}


int main(void)
{
	// Open window
    
    glfwSetErrorCallback(error_callback);
    if (!glfwInit())
        exit(EXIT_FAILURE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    window = glfwCreateWindow(1024, 768, "Simple example", NULL, NULL);
	
	//set cursor disabled
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	if (ImmIsIME(GetKeyboardLayout(0))) {
		ImmSimulateHotKey(FindWindow(NULL,"Simple example"), 16);
	}
	

	glfwSetCursorPosCallback(window, mouse_callback);
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
	
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);

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
    prog2 = Program::LoadFromFile(
        "../resource/vs.vert",
        "../resource/gs.geom",
        "../resource/fs_light.frag"
    );
	
	//*************************************					vvvv	flag initialize 	vvvv				****************************************//
	Entity *entityArr[300];
	for (int i = 0; i < 300; ++i){
		entityArr[i] = new Entity();
		entityArr[i]->rigid.type = SPHERE;
		entityArr[i]->rigid.setRadius(0.05);
		entityArr[i]->rigid._mass = 0.09;
	}
	
	Flag flag(entityArr);
	flag.bindBufferObjects();
	
	for (int i = 0; i < 300; ++i){
		ObjectManager::aabb_tree.add(entityArr[i]);
	}
	
	//14 * 20 + 15 * 19 + 14 * 19 * 2 + 2
	Spring *springArr[1099];
	for (int i = 0; i < 1099; ++i){
		springArr[i] = new Spring();
	}
	initialSpring(springArr, entityArr, 1, 0.15);
	
	
	initialAABB();
	//*************************************					^^^^	flag initialize 	^^^^				****************************************//


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
		
		
		glm::vec3 wind_direction{1.0f};
		float wind_force = 0;
		
		int lastClock = clock();
		int count = 0;
		bool first = true;
		while (!glfwWindowShouldClose(window))
		{
			float current_frame = glfwGetTime();
			dt = current_frame - last_frame;
			if (first){
				first = false;
				dt = 0;
			}
			last_frame = current_frame;
			int currentClock = clock();
			processInput();
			TPcamera.Position += TPcamera._v * dt;
			
			glfwPollEvents();
			int display_w, display_h;
			glfwGetFramebufferSize(window, &display_w, &display_h);
			glViewport(0, 0, display_w, display_h);
			glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			glEnable(GL_DEPTH_TEST);
			
			//physic compute
			wind_direction = glm::normalize(wind_direction);
			springUpdate(springArr);
			for (int i = 0; i < 300; ++i){
				Physics::wind(entityArr[i], dt, wind_direction, wind_force);
				Physics::update(entityArr[i], dt, entityArr);
			}
			for (int i = 0; i < 300; ++i){
				flag.updatePositions(entityArr);
			}
			ObjectManager::aabb_tree.update();
			flag.bindBufferObjects();
			//aabbDebuger();

			

			prog["vp"] = glm::perspective(45/180.0f*3.1415926f, 1024.0f/768.0f, 0.1f, 10000.0f)*TPcamera.getViewMatrix();
				//glm::lookAt(glm::vec3{0, 0, 10}, glm::vec3{0, 0, 0}, glm::vec3{0, 1, 0});
			prog["model"] = glm::rotate(glm::mat4(1.0f), degree*3.1415926f/180.0f, glm::vec3(0, 1, 0)) * glm::scale(glm::mat4(1.0f), glm::vec3(0.14f, 0.18f, 0.14f));
			prog["object_color"] = object_color;
			prog["light_pos"] = TPcamera.Position + TPcamera.radius * TPcamera.Front;
			prog["eye_pos"] = TPcamera.Position;
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
				prog2["vp"] = glm::perspective(45/180.0f*3.1415926f, 1024.0f/768.0f, 0.1f, 10000.0f)*TPcamera.getViewMatrix();
				//glm::lookAt(glm::vec3{0, 0, 10}, glm::vec3{0, 0, 0}, glm::vec3{0, 1, 0});
				// point light
				prog2["model"] = glm::translate(glm::mat4(1.0f), TPcamera.Position + TPcamera.radius * TPcamera.Front)*glm::scale(glm::mat4(1.0f), glm::vec3{0.2f});
				prog2["flat_shading"] = 0;
				
				prog2["object_color"] = glm::vec3(1.0, 1.0, 0.0);

				prog2.use();
				lightMesh.draw();
				
				prog2["model"] = glm::translate(glm::mat4(1.0f), glm::vec3(2.0f,1.6f,0.0f))*glm::scale(glm::mat4(1.0f), glm::vec3{2.0f});
				flag.drawFlag();
				
				prog2["object_color"] = glm::vec3(0.5, 0.0, 0.0);
				prog2["model"] = glm::mat4(1.0);
				//aabbDebuger();
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
				ImGui::SliderFloat3("wind direction", glm::value_ptr(wind_direction), -1, 1);
				ImGui::SliderFloat("shininess", &shininess, 1.0f, 100.0f);
				ImGui::SliderFloat3("Position", glm::value_ptr(light_pos), -10, 10);
				ImGui::SliderFloat("wind force", &wind_force, 0, 0.01);
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
