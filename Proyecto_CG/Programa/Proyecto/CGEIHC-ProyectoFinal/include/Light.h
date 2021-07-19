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

	Light(glm::vec3 position, glm::vec3 direction, glm::vec4 color, glm::vec4 power, int alphaIndex_in, float distance_in) {
		Position = position; // Posición de la fuente de luz
		Direction = direction; // Dirección (no aplica para fuentes puntuales)
		Color = color; // Color de la fuente de luz
		Power = power; // Potencia en Watts
		alphaIndex = alphaIndex_in; // potencia del brillo especular
		distance = distance_in;
	}
	~Light() {}

private:

};

#endif
