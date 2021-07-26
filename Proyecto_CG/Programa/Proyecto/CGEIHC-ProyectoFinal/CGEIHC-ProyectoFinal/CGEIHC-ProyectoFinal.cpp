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

// irrKlang sound library
#include <irrKlang.h>

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

float tiempoPasadoFiesta;

// Variables para la transformacion
// geometrica de objetos
float rotacionPuertas = 0.0f;
float traslacionVentanas = 0.0f;
float rotacionTapaInodoro = 0.0f;
float escalamientoGlobo = 1.0f;
float desplazamientoGlobo;
float tiempo = 0.0f;

float velocidadPuerta = 110.0f / 3.0f;
float velocidadVentanas = 1.215f / 2.0f;
float velocidadTapaInodoro = 90.0f / 2.0f;
float velocidadGlobo = 1.0f / 4.0f;

// Fuentes de luz
Light sun = Light(glm::vec3(0.0f,100.0f,0.0f), glm::vec3(0.0f,-1.0f,0.0f), glm::vec4(1.0f,1.0f,1.0f,1.0f), glm::vec4(100.0f,100.0f,100.0f,100.0f),1,20.f);
Light casa = Light(glm::vec3(-11.81f, 2.70f, -3.485f), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), glm::vec4(50.0f, 50.0f, 50.0f, 50.0f), 10, 10.f);

// Color de luz
glm::vec4 color_casa = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
bool modoFiesta = false;
float duracionLuces = 1.0f;
int colores_fiesta = 0;

//Vectores de posicion y rotacion del personaje
glm::vec3 positionCharacter(0.0f);
float rotateCharacter = 0.0f;
int animType = 0;

//VEctores camara
glm::vec3 positionCamera = camera.Position;
float YawCamera=camera.Yaw;
float PitchCamera=camera.Pitch;
int cameraMode = 0;
float delayCameraMode = 0;
bool changeCameraMode = true;
float rotAjuste = 1.0f;

//UFO poss
glm::vec3 ufoPoss = glm::vec3(0.0f);
float ufoAngle = 0.0f;
float ufoRadio = 8.0f;

//Sonido
irrklang::ISoundEngine* SoundEngine = irrklang::createIrrKlangDevice();
bool music = false;
float delayMusic = 0.0f;
bool changeMusic = true;
irrklang::ISound* snd;
irrklang::ISound* sndGrass = SoundEngine->play3D("sounds/footstep.mp3", irrklang::vec3df(positionCharacter.x, positionCharacter.y, positionCharacter.z), true, true);
float grassTime;
bool walking = false;


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
	//Model casa_piso("models/casa_piso.fbx");
	Model brillosos_blancos("models/brillosos_blancos.fbx");
	//Model silla_1("models/silla_1.fbx");
	//Model silla_2("models/silla_2.fbx");
	//Model silla_3("models/silla_3.fbx");
	//Model silla_4("models/silla_4.fbx");
	//Model mesa("models/mesa.fbx");
	//Model lowtable("models/lowtable.fbx");
	//Model lavabo("models/lavabo.fbx");
	//Model cama("models/cama.fbx");
	//Model buro("models/buro.fbx");
	Model opacos("models/opacos.fbx");
	//Model casa_est("models/casa_estructura.fbx");
	Model cristales("models/cristales.fbx");
	Model metales("models/metales.fbx");
	Model coche("models/coche.fbx");
	Model ufo("models/ufo.fbx");
	Model beam("models/beam.fbx");
	Model globo("models/globo.fbx");
	//Model sofa("models/sofa.fbx");
	//Model libros("models/libros.fbx");
	//Model tv("models/tv.fbx");
	//Model ps4("models/ps4.fbx");
	
	// Modelos con animacion
	Model character("models/character.fbx");
	
	
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
	//Personaje Animado pose inicial
	glm::mat4 gBones[MAX_RIGGING_BONES];
	//character.SetPose(0.0f, gBones, animType);
	character.SetPose(0.0f, gBones);
	float fps = (float)character.getFramerate();
	fps *= 2;
	int keys = (int)character.getNumFrames();
	int animationCount = 0;

	// Dibujar en malla de alambre
	// glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	//volumen
	sndGrass->setMinDistance(2.0f);
	sndGrass->setVolume(0.4);
	// Definicion de fuentes de luz
	delayCameraMode= (float)glfwGetTime();
	// Loop de renderizado
	while (!glfwWindowShouldClose(window))
	{

		// Cálculo del framerate
		float currentFrame = (float)glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		elapsedTime += deltaTime;
		tiempoPasadoFiesta += deltaTime;
		tiempo += deltaTime;

		if (elapsedTime > 1.0f / fps) {
			animationCount++;
			if (animationCount > keys - 1) {
				animationCount = 0;
			}
			// Configuración de la pose en el instante t
			//character.SetPose((float)animationCount, gBones, animType);
			character.SetPose((float)animationCount, gBones);
			elapsedTime = 0.0f;
		}

		if (modoFiesta) {
			if (tiempoPasadoFiesta > duracionLuces) {
				colores_fiesta += 1;
				colores_fiesta = colores_fiesta % 6;
				switch (colores_fiesta) {
				case 0:
					casa.Color = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
					break;
				case 1:
					casa.Color = glm::vec4(1.0f, 1.0f, 0.0f, 1.0f);
					break;
				case 2:
					casa.Color = glm::vec4(0.0f, 1.0f, 0.0f, 1.0f);
					break;
				case 3:
					casa.Color = glm::vec4(0.0f, 1.0f, 1.0f, 1.0f);
					break;
				case 4:
					casa.Color = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f);
					break;
				case 5:
					casa.Color = glm::vec4(1.0f, 0.0f, 1.0f, 1.0f);
					break;
				default:
					casa.Color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
				}
				tiempoPasadoFiesta = 0.0f;
			}
		}

		desplazamientoGlobo = 0.10f*sin(2*3.14159*(1.0f/3.0f)*tiempo);

		//Espero de 0.5 s para volver a cambiar la camara
		if((currentFrame-delayCameraMode)>=0.5f){
			changeCameraMode = true;
		}
		//Espero de 0.5 s para prender o apagar la musica
		if ((currentFrame - delayMusic) >= 0.5f) {
			changeMusic = true;
		}

		if ((currentFrame - grassTime) >= 0.5f and walking==false){
			walking = true;
			sndGrass->setIsPaused(true);
		}
		if (sndGrass)
			sndGrass->setPosition(irrklang::vec3df(positionCharacter.x, positionCharacter.y, positionCharacter.z));
		if (snd)
			snd->setPosition(irrklang::vec3df(positionCharacter.x, positionCharacter.y, positionCharacter.z));

		
		//cout << currentFrame - delayCameraMode << endl;
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
			ourShader.use();
			glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
			glm::mat4 view = camera.GetViewMatrix();
			ourShader.setMat4("projection", projection);
			ourShader.setMat4("view", view);
			// Aplicamos transformaciones del modelo
			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(model, positionCharacter); // translate it down so it's at the center of the scene
			model = glm::rotate(model, glm::radians(rotateCharacter), glm::vec3(0.0f, 1.0f, 0.0f));
			model = glm::scale(model, glm::vec3(0.01f, 0.01f, 0.01f));	// it's a bit too big for our scene, so scale it down

			ourShader.setMat4("model", model);

			ourShader.setMat4("gBones", MAX_RIGGING_BONES, gBones);

			// Dibujamos el modelo
			character.Draw(ourShader);

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
			
			brillosos.Draw(basicPhongShader);
			coche.Draw(basicPhongShader);
			
			/*
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
			*/

			basicPhongShader.setVec4("MaterialAmbientColor", glm::vec4(0.6, 0.6, 0.6, 1.0));
			basicPhongShader.setVec4("MaterialDiffuseColor", glm::vec4(0.5, 0.5, 0.5, 1.0));
			basicPhongShader.setVec4("MaterialSpecularColor", glm::vec4(0.5, 0.5, 0.5, 1.0));

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
			/*
			casa_est.Draw(basicPhongShader);
			sofa.Draw(basicPhongShader);
			libros.Draw(basicPhongShader);
			ps4.Draw(basicPhongShader);
			tv.Draw(basicPhongShader);
			*/
			opacos.Draw(basicPhongShader);

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
			
			// Globo
			model = glm::translate(model, glm::vec3(-1.0316f, 1.0074f, 4.7883f+desplazamientoGlobo));
			model = glm::scale(model, glm::vec3(escalamientoGlobo, escalamientoGlobo, escalamientoGlobo));
			basicPhongShader.setMat4("model", model);
			globo.Draw(basicPhongShader);
			model = glm::mat4(1.0f);
			model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));

			// Brillosos blancos
			basicPhongShader.setVec4("MaterialAmbientColor", glm::vec4(0.6, 0.6, 0.6, 1.0));
			basicPhongShader.setVec4("MaterialDiffuseColor", glm::vec4(0.5, 0.5, 0.5, 1.0));
			basicPhongShader.setVec4("MaterialSpecularColor", glm::vec4(0.5, 0.5, 0.5, 1.0));

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

			// Ventanas movibles 
			model = glm::translate(model, glm::vec3(0.0f, traslacionVentanas, 0.0f));
			basicPhongShader.setMat4("model", model);
			ventanasMovibles_brillosos.Draw(basicPhongShader);
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


			//UFO
			model = glm::mat4(1.0f);
			model = glm::translate(model, glm::vec3(ufoPoss.x, 0, ufoPoss.z));
			model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
			fresnelShader.setMat4("model", model);
			Material beamMat;
			beamMat.transparency = 0.4f;
			beamMat.diffuse = glm::vec4(0.4f, 0.4f, 0.0f, 1.0f);
			fresnelShader.setFloat("transparency", beamMat.transparency);
			fresnelShader.setVec4("reflectColor", beamMat.diffuse);
			beam.Draw(fresnelShader);

			Material ufoMat;
			ufoMat.transparency = 1.0f;
			fresnelShader.setFloat("transparency", ufoMat.transparency);
			fresnelShader.setVec4("reflectColor", ufoMat.diffuse);
			ufo.Draw(fresnelShader);

		}

		glUseProgram(0);
		{
			ufoAngle += 4;
			if (ufoAngle >= 360)
				ufoAngle = 0;
			ufoPoss.x = ufoRadio * glm::cos(glm::radians(ufoAngle));
			ufoPoss.z = ufoRadio * glm::sin(glm::radians(ufoAngle));
		}

		// glfw: swap buffers 
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	if (sndGrass) {
		sndGrass->drop();
		sndGrass = 0;
	}
	SoundEngine->removeAllSoundSources();
	// glfw: Terminamos el programa y liberamos memoria
	glfwTerminate();
	return 0;
}

// Procesamos entradas del teclado
void processInput(GLFWwindow* window)
{
	//Camera 
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		camera.ProcessKeyboard(FORWARD,deltaTime);
		if (cameraMode == 1) {
			float velocity = camera.MovementSpeed * deltaTime;
			positionCharacter += camera.Front * velocity;
			if (walking == true) {

				sndGrass->setIsPaused(false);
				walking = false;
				grassTime = (float)glfwGetTime();
			}

		}

	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		camera.ProcessKeyboard(BACKWARD, deltaTime);
		if (cameraMode == 1) {
			float velocity = camera.MovementSpeed * deltaTime;
			positionCharacter -= camera.Front * velocity;
			if (walking == true) {

				sndGrass->setIsPaused(false);
				walking = false;
				grassTime = (float)glfwGetTime();
			}
		}

	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		camera.ProcessKeyboard(LEFT, deltaTime);
		if (cameraMode == 1) {
			float velocity = camera.MovementSpeed * deltaTime;
			positionCharacter -= camera.Right * velocity;
			if (walking == true) {

				sndGrass->setIsPaused(false);
				walking = false;
				grassTime = (float)glfwGetTime();
			}
		}
	
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		camera.ProcessKeyboard(RIGHT, deltaTime);
		if (cameraMode == 1) {
			float velocity = camera.MovementSpeed * deltaTime;
			positionCharacter += camera.Right * velocity;
			if (walking == true) {

				sndGrass->setIsPaused(false);
				walking = false;
				grassTime = (float)glfwGetTime();
			}
		}


	}
	if (glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	if (glfwGetKey(window, GLFW_KEY_N) == GLFW_PRESS)
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	if (glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS)
		glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
	if (glfwGetKey(window,GLFW_KEY_SPACE)==GLFW_PRESS and cameraMode == 0)
		camera.ProcessKeyboard(UP, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS and cameraMode == 0)
		camera.ProcessKeyboard(DOWN, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS) {
		if (changeCameraMode == true) {
			changeCameraMode = false;
			delayCameraMode = (float)glfwGetTime();
			if (cameraMode==0) {
				cameraMode = 1;
				positionCamera=camera.Position;
				YawCamera = camera.Yaw;
				PitchCamera = camera.Pitch;
				camera.SwichCameraMode(positionCharacter+glm::vec3(
					-2*glm::cos(glm::radians(-rotateCharacter+90))+ 0.4 * glm::sin(glm::radians(-rotateCharacter + 90)),
					2.0f,
					 -2 * glm::sin(glm::radians(-rotateCharacter + 90))+0.4 * glm::cos(glm::radians(-rotateCharacter + 90))),
					-rotateCharacter * rotAjuste +90);
			}
			else
			{
				cameraMode = 0;
				camera.SwichCameraMode(positionCamera, YawCamera, PitchCamera);
			}	
		}
	}

	// Character movement
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS and cameraMode==0) {
		firstMouse = true;
		camera.ProcessMouseMovement(0,2);
	}
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS and cameraMode == 0) {
		firstMouse = true;
		camera.ProcessMouseMovement(0,-2);
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
		firstMouse = true;
		if (cameraMode == 1) {
			if (walking == true) {

				sndGrass->setIsPaused(false);
				walking = false;
				grassTime = (float)glfwGetTime();
			}
			rotateCharacter += 2;
			camera.SwichCameraMode(positionCharacter + glm::vec3(
				-2 * glm::cos(glm::radians(-rotateCharacter * rotAjuste + 90)) + 0.4 * glm::sin(glm::radians(-rotateCharacter * rotAjuste + 90)),
				2.0f,
				-2 * glm::sin(glm::radians(-rotateCharacter * rotAjuste + 90)) + 0.4 * glm::cos(glm::radians(-rotateCharacter * rotAjuste + 90))),
				-rotateCharacter * rotAjuste + 90);
		}
		else {
		camera.ProcessMouseMovement(-2,0);
		}
	}
	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
		firstMouse = true;
		if (cameraMode==1) {
			if (walking == true){

				sndGrass->setIsPaused(false);
				walking = false;
				grassTime = (float)glfwGetTime();
			}
			rotateCharacter -= 2;
			camera.SwichCameraMode(positionCharacter + glm::vec3(
				-2 * glm::cos(glm::radians(-rotateCharacter * rotAjuste + 90)) + 0.4 * glm::sin(glm::radians(-rotateCharacter * rotAjuste + 90)),
				2.0f,
				-2 * glm::sin(glm::radians(-rotateCharacter * rotAjuste + 90)) + 0.4 * glm::cos(glm::radians(-rotateCharacter * rotAjuste + 90))),
				-rotateCharacter * rotAjuste + 90);
		}
		else {
		camera.ProcessMouseMovement(2,0);
		}
	}

	if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS)
	{
		rotacionPuertas -= elapsedTime*velocidadPuerta;
		if (rotacionPuertas < -100.0f)
			rotacionPuertas = -100.0f;
	}
	if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS)
	{
		rotacionPuertas += elapsedTime*velocidadPuerta;
		if (rotacionPuertas > 0.0f)
			rotacionPuertas = 0.0f;
	}
	if (glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS)
	{
		rotacionTapaInodoro -= elapsedTime*velocidadTapaInodoro;
		if (rotacionTapaInodoro < -90.0f)
			rotacionTapaInodoro = -90.0f;
	}
	if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS)
	{
		rotacionTapaInodoro += elapsedTime*velocidadTapaInodoro;
		if (rotacionTapaInodoro > 0.0f)
			rotacionTapaInodoro = 0.0f;
	}
	if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS)
	{
		traslacionVentanas -= elapsedTime*velocidadVentanas;
		if (traslacionVentanas < -0.55f)
			traslacionVentanas = -0.55f;
	}
	if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS)
	{
		traslacionVentanas += elapsedTime*velocidadVentanas;
		if (traslacionVentanas > 0.665f)
			traslacionVentanas = 0.665f;
	}
	if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS)
	{
		escalamientoGlobo -= elapsedTime * velocidadGlobo;
		if (escalamientoGlobo < 0.1f)
			escalamientoGlobo = 0.1f;
	}
	if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
	{
		escalamientoGlobo += elapsedTime * velocidadGlobo;
		if (escalamientoGlobo > 1.0f)
			escalamientoGlobo = 1.0f;
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
	
	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
	{
		modoFiesta = true;
	}
	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
	{
		modoFiesta = false;
		casa.Color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	}
	if (glfwGetKey(window, GLFW_KEY_Y) == GLFW_PRESS and changeMusic == true) {
		delayMusic = (float)glfwGetTime();
		changeMusic = false;
		if (music == false) {
			//SoundEngine->play2D("sounds/100real.mp3", true);
			
			snd= SoundEngine->play3D("sounds/100real.mp3", irrklang::vec3df(positionCharacter.x, positionCharacter.y, positionCharacter.z),true,true);
			music = true;
			if (snd){
				snd->setMinDistance(2.0f);
				//snd->setMaxDistance(6.0f);
				snd->setVolume(0.4);
				snd->setIsPaused(false);
			}

		}
		else {
			if (snd){
				snd->drop();
				snd = 0;
			}
			SoundEngine->removeSoundSource("sounds/100real.mp3");
			music = false;
		}

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
	if (cameraMode == 0)
	{
		camera.ProcessMouseMovement(xoffset, yoffset);

	}
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
