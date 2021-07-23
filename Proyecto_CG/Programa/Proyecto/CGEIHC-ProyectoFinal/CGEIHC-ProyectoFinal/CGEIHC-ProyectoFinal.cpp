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
Camera camera(glm::vec3(8.0f, 1.8f, 1.0f));

// Controladores para el movimiento del mouse
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// Variables para la velocidad de reproducción
// de la animación
float deltaTime = 0.0f;
float lastFrame = 0.0f;
float elapsedTime = 0.0f;

// Variables para la transformacion
// geometrica de objetos
float rotacionPuertas = 0.0f;
float traslacionVentanas = 0.0f;
float rotacionTapaInodoro = 0.0f;

// Fuentes de luz
Light sun = Light(glm::vec3(0.0f,100.0f,0.0f), glm::vec3(0.0f,-1.0f,0.0f), glm::vec4(1.0f,1.0f,1.0f,1.0f), glm::vec4(100.0f,100.0f,100.0f,100.0f),1,20.f);
Light casa = Light(glm::vec3(-11.81f, 2.70f, -3.485f), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), glm::vec4(50.0f, 50.0f, 50.0f, 50.0f), 10, 10.f);

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
	Shader ourShader("shaders/10_vertex_skinning-IT.vs", "shaders/10_fragment_skinning-IT.fs");
	Shader cubemapShader("shaders/10_vertex_cubemap.vs", "shaders/10_fragment_cubemap.fs");
	Shader basicPhongShader("shaders/11_BasicPhongShader.vs", "shaders/11_BasicPhongShader.fs"); // opacos y plásticos, cerámicas
	Shader fresnelShader("shaders/11_Fresnel.vs", "shaders/11_Fresnel.fs"); // metal y semitraslúcidos

	// Máximo número de huesos: 100
#define MAX_RIGGING_BONES 100
	ourShader.setBonesIDs(MAX_RIGGING_BONES);

	// Carga la información del modelo
		// Estaticos
	Model cubemap("models/cubemap.fbx");
	Model brillosos("models/brillosos.fbx");
	Model casa_piso("models/casa_piso.fbx");
	Model brillosos_blancos("models/brillosos_blancos.fbx");
	Model silla_1("models/silla_1.fbx");
	Model silla_2("models/silla_2.fbx");
	Model silla_3("models/silla_3.fbx");
	Model silla_4("models/silla_4.fbx");
	Model mesa("models/mesa.fbx");
	Model lowtable("models/lowtable.fbx");
	Model lavabo("models/lavabo.fbx");
	Model cama("models/cama.fbx");
	Model buro("models/buro.fbx");
	//Model opacos("models/opacos.fbx");
	Model casa_est("models/casa_estructura.fbx");
	Model cristales("models/cristales.fbx");
	Model metales("models/metales.fbx");
	Model sofa("models/sofa.fbx");
	Model libros("models/libros.fbx");
	Model tv("models/tv.fbx");
	Model ps4("models/ps4.fbx");

	// Materiales con transformaciones geometricas
	Model puertaPrincipal("models/PuertaPrincipal.fbx");
	Model temp("models/cristal_temp.fbx");
	Model puertasBanios_opacos("models/PuertasBanios_opacos.fbx");
	Model puertaCuartoA_opacos("models/PuertaCuartoA_opacos.fbx");
	Model puertaCuartoB_opacos("models/PuertaCuartoB_opacos.fbx");
	Model puertaPrincipal_metales("models/PuertaPrincipal_metales.fbx");
	Model puertasBanios_metales("models/PuertasBanios_metales.fbx");
	Model puertaCuartoA_metales("models/PuertaCuartoA_metales.fbx");
	Model puertaCuartoB_metales("models/PuertaCuartoB_metales.fbx");
	Model puertaPrincipal_cristales("models/PuertaPrincipal_cristales.fbx");
	Model ventanasMovibles_brillosos("models/ventanasMovibles_brillosos.fbx");
	Model ventanasMovibles_cristales("models/ventanasMovibles_cristales.fbx");
	Model pb_inodoro_tapa("models/pb_inodoro_tapa.fbx");
	Model p2_inodoro_tapa("models/p2_inodoro_tapa.fbx");

	//Model donuts("models/materials/donuts.fbx");
	//Model sphere("models/materials/sphere.fbx");
	//Model monkey("models/materials/monkey.fbx");

	// Cubemap
	/*vector<std::string> faces
	{
		"textures/cubemap/posx.jpg",
		"textures/cubemap/negx.jpg",
		"textures/cubemap/posy.jpg",
		"textures/cubemap/negy.jpg",
		"textures/cubemap/posz.jpg",
		"textures/cubemap/negz.jpg"
	};*/
	vector<std::string> faces
	{
		"models/cubemap.fbm/posx.jpg",
		"models/cubemap.fbm/negx.jpg",
		"models/cubemap.fbm/posy.jpg",
		"models/cubemap.fbm/negy.jpg",
		"models/cubemap.fbm/posz.jpg",
		"models/cubemap.fbm/negz.jpg"
	};
	unsigned int cubemapTexture = loadCubemap(faces);

	// Dibujar en malla de alambre
	// glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	// Definicion de fuentes de luz

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

			// Propiedades de las fuentes de luz

			basicPhongShader.setVec3("lightPosition_0", sun.Position);
			basicPhongShader.setVec3("lightDirection_0", sun.Direction);

			basicPhongShader.setVec4("LightColor_0", sun.Color);
			basicPhongShader.setVec4("LightPower_0", sun.Power);
			basicPhongShader.setInt("alphaIndex_0", sun.alphaIndex);
			basicPhongShader.setFloat("distance_0", sun.distance);

			basicPhongShader.setVec3("lightPosition_1", casa.Position);
			basicPhongShader.setVec3("lightDirection_1", casa.Direction);

			basicPhongShader.setVec4("LightColor_1", casa.Color);
			basicPhongShader.setVec4("LightPower_1", casa.Power);
			basicPhongShader.setInt("alphaIndex_1", casa.alphaIndex);
			basicPhongShader.setFloat("distance_1", casa.distance);

			basicPhongShader.setVec3("eye", camera.Position);

			// Enable blending
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

			// MADERAS
			Material brilloso;
			// Propiedades materiales
			brilloso.ambient = glm::vec4(0.5, 0.5, 0.5, 1.0);
			brilloso.diffuse = glm::vec4(0.8, 0.8, 0.8, 1.0);
			brilloso.specular = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);

			basicPhongShader.setVec4("MaterialAmbientColor", brilloso.ambient);
			basicPhongShader.setVec4("MaterialDiffuseColor", brilloso.diffuse);
			basicPhongShader.setVec4("MaterialSpecularColor", brilloso.specular);
			basicPhongShader.setFloat("transparency", brilloso.transparency);
			casa_piso.Draw(basicPhongShader);
			//brillosos.Draw(basicPhongShader);
			silla_1.Draw(basicPhongShader);
			silla_2.Draw(basicPhongShader);
			silla_3.Draw(basicPhongShader);
			silla_4.Draw(basicPhongShader);
			mesa.Draw(basicPhongShader);
			lowtable.Draw(basicPhongShader);
			lavabo.Draw(basicPhongShader);
			cama.Draw(basicPhongShader);
			buro.Draw(basicPhongShader);
			// Brillosos blancos

			brillosos_blancos.Draw(basicPhongShader);

			// MATERIALES OPACOS
			Material opaco;
			// Propiedades materiales
			opaco.ambient = glm::vec4(0.8, 0.8, 0.8, 1.0);
			opaco.diffuse = glm::vec4(0.5, 0.5, 0.5, 1.0);
			opaco.specular = glm::vec4(0.1, 0.1, 0.1, 1.0);
			basicPhongShader.setVec4("MaterialAmbientColor", opaco.ambient);
			basicPhongShader.setVec4("MaterialDiffuseColor", opaco.diffuse);
			basicPhongShader.setVec4("MaterialSpecularColor", opaco.specular);
			basicPhongShader.setFloat("transparency", opaco.transparency);
			casa_est.Draw(basicPhongShader);
			sofa.Draw(basicPhongShader);
			libros.Draw(basicPhongShader);
			ps4.Draw(basicPhongShader);
			tv.Draw(basicPhongShader);
			//opacos.Draw(basicPhongShader);

			// Puertas
			// Phong de material brilloso
			basicPhongShader.setVec4("MaterialAmbientColor", brilloso.ambient);
			basicPhongShader.setVec4("MaterialDiffuseColor", brilloso.diffuse);
			basicPhongShader.setVec4("MaterialSpecularColor", brilloso.specular);
			basicPhongShader.setFloat("transparency", brilloso.transparency);
			
			// Puerta principal
			model = glm::translate(model, glm::vec3(-6.0822f, -0.359038f, 0.435927f));
			model = glm::rotate(model, glm::radians(rotacionPuertas), glm::vec3(0.0f, 0.0f, 1.0f));
			basicPhongShader.setMat4("model", model);
			puertaPrincipal.Draw(basicPhongShader);
			model = glm::mat4(1.0f);
			model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));

			// Puerta de los banios
			model = glm::translate(model, glm::vec3(-6.542f, -2.211f, 0.2994f));
			model = glm::rotate(model, glm::radians(-rotacionPuertas), glm::vec3(0.0f, 0.0f, 1.0f));
			basicPhongShader.setMat4("model", model);
			puertasBanios_opacos.Draw(basicPhongShader);
			model = glm::mat4(1.0f);
			model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));

			// Puerta del Cuarto A
			model = glm::translate(model, glm::vec3(-7.599f, 0.2697f, 3.329f));
			model = glm::rotate(model, glm::radians(-rotacionPuertas), glm::vec3(0.0f, 0.0f, 1.0f));
			basicPhongShader.setMat4("model", model);
			puertaCuartoA_opacos.Draw(basicPhongShader);
			model = glm::mat4(1.0f);
			model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));

			// Puerta del Cuarto B
			model = glm::translate(model, glm::vec3(-10.29f, 0.2698f, 3.329f));
			model = glm::rotate(model, glm::radians(-rotacionPuertas), glm::vec3(0.0f, 0.0f, 1.0f));
			basicPhongShader.setMat4("model", model);
			puertaCuartoB_opacos.Draw(basicPhongShader);
			model = glm::mat4(1.0f);
			model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));

			// Ventanas movibles 
			model = glm::translate(model, glm::vec3(0.0f, traslacionVentanas, 0.0f));
			basicPhongShader.setMat4("model", model);
			ventanasMovibles_brillosos.Draw(basicPhongShader);
			model = glm::mat4(1.0f);
			model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));

			// Tapa inodoro PB
			model = glm::translate(model, glm::vec3(-7.6824, -5.0065, 0.51579));
			model = glm::rotate(model, glm::radians(-rotacionTapaInodoro), glm::vec3(0.0f, -1.0f, 0.0f));
			basicPhongShader.setMat4("model", model);
			pb_inodoro_tapa.Draw(basicPhongShader);
			model = glm::mat4(1.0f);
			model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));

			// Tapa inodoro P2
			model = glm::translate(model, glm::vec3(-7.6785, -5.0634, 3.54579));
			model = glm::rotate(model, glm::radians(-rotacionTapaInodoro), glm::vec3(0.0f, -1.0f, 0.0f));
			basicPhongShader.setMat4("model", model);
			pb_inodoro_tapa.Draw(basicPhongShader);
			model = glm::mat4(1.0f);
			model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
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
			//model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f)); // translate it down so it's at the center of the scene
			model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
			//model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
			//model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 0.0f, 1.0f));
			model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));	// it's a bit too big for our scene, so scale it down
			fresnelShader.setMat4("model", model);

			// Enable blending
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

			glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
			
			
			// Cristales
			Material cristal;
			// Propiedades del material
			cristal.transparency = 0.2f;
			cristal.diffuse = glm::vec4(0.0f, 0.4f, 0.7f, 1.0f);
			fresnelShader.setFloat("transparency", cristal.transparency);
			fresnelShader.setVec4("reflectColor", cristal.diffuse);
			cristales.Draw(fresnelShader);
			
			// Metales
			Material metal;
			// Propiedades del material
			metal.transparency = 1.0f;
			fresnelShader.setFloat("transparency", metal.transparency);
			fresnelShader.setVec4("reflectColor", metal.diffuse);
			metales.Draw(fresnelShader);


			// Materiales de fresnel con transformaciones geometricas

			// Metales
			// Puerta principal
			model = glm::translate(model, glm::vec3(-6.0822f, -0.359038f, 0.435927f));
			model = glm::rotate(model, glm::radians(rotacionPuertas), glm::vec3(0.0f, 0.0f, 1.0f));
			fresnelShader.setMat4("model", model);
			puertaPrincipal_metales.Draw(fresnelShader);
			model = glm::mat4(1.0f);
			model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));

			// Puerta de los banios
			model = glm::translate(model, glm::vec3(-6.542f, -2.211f, 0.2994f));
			model = glm::rotate(model, glm::radians(-rotacionPuertas), glm::vec3(0.0f, 0.0f, 1.0f));
			fresnelShader.setMat4("model", model);
			puertasBanios_metales.Draw(fresnelShader);
			model = glm::mat4(1.0f);
			model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));

			// Puerta del Cuarto A
			model = glm::translate(model, glm::vec3(-7.599f, 0.2697f, 3.329f));
			model = glm::rotate(model, glm::radians(-rotacionPuertas), glm::vec3(0.0f, 0.0f, 1.0f));
			fresnelShader.setMat4("model", model);
			puertaCuartoA_metales.Draw(fresnelShader);
			model = glm::mat4(1.0f);
			model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));

			// Puerta del Cuarto B
			model = glm::translate(model, glm::vec3(-10.29f, 0.2698f, 3.329f));
			model = glm::rotate(model, glm::radians(-rotacionPuertas), glm::vec3(0.0f, 0.0f, 1.0f));
			fresnelShader.setMat4("model", model);
			puertaCuartoB_metales.Draw(fresnelShader);
			model = glm::mat4(1.0f);
			model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));

			// Cristales
			fresnelShader.setFloat("transparency", cristal.transparency);
			fresnelShader.setVec4("reflectColor", cristal.diffuse);

			// Puerta Principal
			model = glm::translate(model, glm::vec3(-6.0822f, -0.359038f, 0.435927f));
			model = glm::rotate(model, glm::radians(rotacionPuertas), glm::vec3(0.0f, 0.0f, 1.0f));
			fresnelShader.setMat4("model", model);
			puertaPrincipal_cristales.Draw(fresnelShader);
			model = glm::mat4(1.0f);
			model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));

			// Ventanas movibles 
			model = glm::translate(model, glm::vec3(0.0f, traslacionVentanas, 0.0f));
			fresnelShader.setMat4("model", model);
			ventanasMovibles_cristales.Draw(fresnelShader);
			model = glm::mat4(1.0f);
			model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
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
	if (glfwGetKey(window,GLFW_KEY_SPACE)==GLFW_PRESS)
		camera.ProcessKeyboard(UP, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
		camera.ProcessKeyboard(DOWN, deltaTime);
	// Character movement
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
		firstMouse = true;
		camera.ProcessMouseMovement(0,1);
	}
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
		firstMouse = true;
		camera.ProcessMouseMovement(0,-1);
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
		firstMouse = true;
		camera.ProcessMouseMovement(-1,0);
	}
	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
		firstMouse = true;
		camera.ProcessMouseMovement(1,0);
	}

	if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS)
	{
		rotacionPuertas -= 0.1;
		if (rotacionPuertas < -100.0f)
			rotacionPuertas = -100.0f;
	}
	if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS)
	{
		rotacionPuertas += 0.1;
		if (rotacionPuertas > 0.0f)
			rotacionPuertas = 0.0f;
	}
	if (glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS)
	{
		rotacionTapaInodoro -= 0.1;
		if (rotacionTapaInodoro < -90.0f)
			rotacionTapaInodoro = -90.0f;
	}
	if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS)
	{
		rotacionTapaInodoro += 0.1;
		if (rotacionTapaInodoro > 0.0f)
			rotacionTapaInodoro = 0.0f;
	}
	if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS)
	{
		traslacionVentanas -= 0.01;
		if (traslacionVentanas < -0.55f)
			traslacionVentanas = -0.55f;
	}
	if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS)
	{
		traslacionVentanas += 0.01;
		if (traslacionVentanas > 0.665f)
			traslacionVentanas = 0.665f;
	}
	if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS)
	{
		casa.Power = casa.Power + glm::vec4(0.5f, 0.5f, 0.5f, 0.5f);
	}
	if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS)
	{
		casa.Power = casa.Power - glm::vec4(0.5f, 0.5f, 0.5f, 0.5f);
		if(casa.Power.w < 0)
			casa.Power = glm::vec4(0.0f, 0.0f, 0.0f, 0.0f);
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
	//std::cout <<"X offset: " << xoffset <<"Y offset: " << yoffset<<std::endl;
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
