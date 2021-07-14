// Interacción
// Laboratorio de Computación Gráfica e Interacción Humano Computadora
// Iluminación y texturizado

#include <iostream>
#include <stdlib.h>

// GLAD: Multi-Language GL/GLES/EGL/GLX/WGL Loader-Generator
// https://glad.dav1d.de/
#include <glad/glad.h>

// GLFW: https://www.glfw.org/
#include <GLFW/glfw3.h>

// GLM: OpenGL Math library
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Model loading classes
#include <shader_m.h>
#include <camera.h>
#include <model.h>
#include <Light.h>
#include <Material.h>

#include <iostream>

// Definición de callbacks
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);

unsigned int loadCubemap(vector<std::string> faces);

// Tamaño en pixeles de la ventana
const unsigned int SCR_WIDTH = 1024;
const unsigned int SCR_HEIGHT = 768;

// Definición de cámara (posición en XYZ)
Camera camera(glm::vec3(0.0f, 0.0f, 10.0f));

// Controladores para el movimiento del mouse
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// Variables para la velocidad de reproducción
// de la animación
float deltaTime = 0.0f;
float lastFrame = 0.0f;
float elapsedTime = 0.0f;

float modelRotation = 0.0f;
Light   light0;


// Entrada a función principal
int main()
{
	// Inicialización de GLFW

	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Creación de la ventana con GLFW
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "FBX Animation with OpenGL", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	// Ocultar el cursor mientras se rota la escena
	// glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// glad: Cargar todos los apuntadores
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// Activación de buffer de profundidad
	glEnable(GL_DEPTH_TEST);

	// Compilación y enlace de shaders
	Shader cubemapShader("shaders/10_vertex_cubemap.vs", "shaders/10_fragment_cubemap.fs");
	Shader basicPhongShader("shaders/11_BasicPhongShader.vs", "shaders/11_BasicPhongShader.fs"); // opacos y plásticos, cerámicas
	Shader fresnelShader("shaders/11_Fresnel.vs", "shaders/11_Fresnel.fs"); // metal y semitraslúcidos

	// Carga la información del modelo
	Model maderas("models/maderas.fbx");
	Model opacos("models/opacos.fbx");
	Model donuts("models/materials/donuts.fbx");
	Model sphere("models/materials/sphere.fbx");
	Model monkey("models/materials/monkey.fbx");
	Model cubemap("models/cubemap.fbx");

	// Cubemap
	vector<std::string> faces
	{
		"textures/cubemap/posx.jpg",
		"textures/cubemap/negx.jpg",
		"textures/cubemap/posy.jpg",
		"textures/cubemap/negy.jpg",
		"textures/cubemap/posz.jpg",
		"textures/cubemap/negz.jpg"
	};
	unsigned int cubemapTexture = loadCubemap(faces);

	// Dibujar en malla de alambre
	// glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	// Loop de renderizado
	while (!glfwWindowShouldClose(window))
	{

		// Cálculo del framerate
		float currentFrame = (float)glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		elapsedTime += deltaTime;

		// Procesa la entrada del teclado o mouse
		processInput(window);

		// Renderizado R - G - B - A
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		
		// objeto estático (cubemap)
		{
			// Activamos el shader del plano
			cubemapShader.use();

			// Aplicamos transformaciones de proyección y cámara (si las hubiera)
			glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 10000.0f);
			glm::mat4 view = camera.GetViewMatrix();
			cubemapShader.setMat4("projection", projection);
			cubemapShader.setMat4("view", view);

			// Aplicamos transformaciones del modelo
			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f)); // translate it down so it's at the center of the scene
			model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
			model = glm::scale(model, glm::vec3(1000.0f, 1000.0f, 1000.0f));	// it's a bit too big for our scene, so scale it down
			cubemapShader.setMat4("model", model);

			cubemap.Draw(cubemapShader);

		}

		glUseProgram(0);
		
		{
			// Activamos el shader estático
			basicPhongShader.use();

			// Aplicamos transformaciones de proyección y cámara (si las hubiera)
			glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
			glm::mat4 view = camera.GetViewMatrix();
			basicPhongShader.setMat4("projection", projection);
			basicPhongShader.setMat4("view", view);

			// Aplicamos transformaciones del modelo
			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f)); // translate it down so it's at the center of the scene
			model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
			model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
			model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
			model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));	// it's a bit too big for our scene, so scale it down
			basicPhongShader.setMat4("model", model);

			// Propiedades de la luz
			basicPhongShader.setVec3("lightPosition", light0.Position);
			basicPhongShader.setVec3("lightDirection", light0.Direction);
			basicPhongShader.setVec4("LightColor", light0.Color);
			basicPhongShader.setVec4("LightPower", light0.Power);
			basicPhongShader.setInt("alphaIndex", light0.alphaIndex);
			basicPhongShader.setFloat("distance", light0.distance);

			basicPhongShader.setVec3("eye", camera.Position);

			// Enable blending
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

			Material madera;
			// Propiedades materiales
			madera.diffuse = glm::vec4(0.12, 0.12, 0.12, 1.0);
			madera.specular = glm::vec4(0.8, 0.8, 0.8, 1.0);
			basicPhongShader.setVec4("MaterialAmbientColor", madera.ambient);
			basicPhongShader.setVec4("MaterialDiffuseColor", madera.diffuse);
			basicPhongShader.setVec4("MaterialSpecularColor", madera.specular);
			basicPhongShader.setFloat("transparency", madera.transparency);

			maderas.Draw(basicPhongShader);

			Material opaco;
			// Propiedades materiales
			opaco.diffuse = glm::vec4(0.5, 0.5, 0.5, 1.0);
			basicPhongShader.setVec4("MaterialAmbientColor", opaco.ambient);
			basicPhongShader.setVec4("MaterialDiffuseColor", opaco.diffuse);
			basicPhongShader.setVec4("MaterialSpecularColor", opaco.specular);
			basicPhongShader.setFloat("transparency", opaco.transparency);

			opacos.Draw(basicPhongShader);

			Material mat2;
			// Propiedades materiales
			mat2.diffuse = glm::vec4(1.0, 0.7, 0.7, 1.0);
			mat2.specular = glm::vec4(0.5, 0.5, 0.5, 1.0);
			mat2.transparency = 0.8f;
			basicPhongShader.setVec4("MaterialAmbientColor", mat2.ambient);
			basicPhongShader.setVec4("MaterialDiffuseColor", mat2.diffuse);
			basicPhongShader.setVec4("MaterialSpecularColor", mat2.specular);
			basicPhongShader.setFloat("transparency", mat2.transparency);

			donuts.Draw(basicPhongShader);

			Material mat3;
			mat3.diffuse = glm::vec4(0.7, 0.7, 0.7, 1.0);
			mat3.specular = glm::vec4(1.0, 1.0, 1.0, 1.0);
			mat3.transparency = 0.8f;
			basicPhongShader.setVec4("MaterialAmbientColor", mat3.ambient);
			basicPhongShader.setVec4("MaterialDiffuseColor", mat3.diffuse);
			basicPhongShader.setVec4("MaterialSpecularColor", mat3.specular);
			basicPhongShader.setFloat("transparency", mat3.transparency);

			monkey.Draw(basicPhongShader);

		}

		glUseProgram(0);

		{
			// Activamos el shader estático
			fresnelShader.use();

			// Aplicamos transformaciones de proyección y cámara (si las hubiera)
			glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
			glm::mat4 view = camera.GetViewMatrix();
			fresnelShader.setMat4("projection", projection);
			fresnelShader.setMat4("view", view);

			// Aplicamos transformaciones del modelo
			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f)); // translate it down so it's at the center of the scene
			model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
			model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
			model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 0.0f, 1.0f));
			model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));	// it's a bit too big for our scene, so scale it down
			fresnelShader.setMat4("model", model);

			// Enable blending
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

			glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
			monkey.Draw(fresnelShader);

		}

		glUseProgram(0);

		// glfw: swap buffers 
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// glfw: Terminamos el programa y liberamos memoria
	glfwTerminate();
	return 0;
}

// Procesamos entradas del teclado
void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessKeyboard(FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboard(LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboard(RIGHT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	if (glfwGetKey(window, GLFW_KEY_N) == GLFW_PRESS)
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	if (glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS)
		glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);

	// Character movement
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {

	}
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {

	}
	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {

	}

	if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS)
	{
		modelRotation += 0.1;
	}
	if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS)
	{
		modelRotation -= 0.1;
	}
}

// glfw: Actualizamos el puerto de vista si hay cambios del tamaño
// de la ventana
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

// glfw: Callback del movimiento y eventos del mouse
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = (float)xpos;
		lastY = (float)ypos;
		firstMouse = false;
	}

	float xoffset = (float)xpos - lastX;
	float yoffset = lastY - (float)ypos;

	lastX = (float)xpos;
	lastY = (float)ypos;

	camera.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: Complemento para el movimiento y eventos del mouse
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll((float)yoffset);
}

unsigned int loadCubemap(vector<std::string> faces)
{
	unsigned int textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

	int width, height, nrChannels;
	for (unsigned int i = 0; i < faces.size(); i++)
	{
		unsigned char* data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
		if (data)
		{
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
				0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data
			);
			stbi_image_free(data);
		}
		else
		{
			std::cout << "Cubemap tex failed to load at path: " << faces[i] << std::endl;
			stbi_image_free(data);
		}
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	return textureID;
}
