#ifndef LIGHT_H
#define LIGHT_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>

class Light 
{
public:
	// Light Attributes
	glm::vec3 Position;
	glm::vec3 Direction;
	glm::vec4 Color;
	glm::vec4 Power;
	int       alphaIndex;
	float     distance;

	Light() {
		Position = glm::vec3(0.0f, 100.0f, 0.0f); // Posición de la fuente de luz
		Direction = glm::vec3(0.0f, -1.0f, 0.0f); // Dirección (no aplica para fuentes puntuales)
		Color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f); // Color de la fuente de luz
		Power = glm::vec4(100.0f, 100.0f, 100.0f, 100.0f); // Potencia en Watts
		alphaIndex = 10; // potencia del brillo especular
		distance = 10.0f;
	}
	~Light() {}

private:

};

#endif
