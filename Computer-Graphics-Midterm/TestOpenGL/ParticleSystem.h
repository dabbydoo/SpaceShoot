#pragma once
#include "libs.h"

class SimpleEmitter
{
public:
	SimpleEmitter();
	~SimpleEmitter();

	void update(const float dt);

	void draw();

private:
	struct Particle
	{
		glm::vec3 position;
		float lifetime;
	};
	std::vector<Particle> particles;

	float positions[400];

	GLuint vertexBuffer;
	GLuint positionBuffer;
};



/*
shader
*/
//class SimpleParticleShader 
//{
//public:
//	SimpleParticleShader();
//	~SimpleParticleShader();
//
//	void setUniformViewMatrix(const glm::mat4& M_v);
//	void setUniformProjectionMatrix(const glm::mat4& M_p);
//	void setUniformParticleSize(const float particleSize);
//
//private:
//	static const std::string VS;
//	static const std::string FS;
//
//	GLuint u_M_v;
//	GLuint u_M_p;
//	GLuint u_particleSize;
//};



/*
controller
*/
class SimpleController
{
public:
	SimpleController();
	~SimpleController();

	void draw(const float dt, const glm::mat4 M_p, const glm::mat4& M_v);

private:
	//SimpleParticleShader* shader;
	Shader* myShader;
	SimpleEmitter* emitter;
};