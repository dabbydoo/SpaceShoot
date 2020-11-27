#include "ParticleSystem.h"
#include "Random.h"



/*
emitter
*/



SimpleEmitter::SimpleEmitter()
{
	// create 100 particles
	this->particles.resize(100);
	for (int i = 0; i < this->particles.size(); ++i)
	{
		// give every particle a random position
		this->particles[i].position = glm::vec3(Random::Float()*2 - 1.0f);//lstd_random_between(vec3(-1.0f), vec3(1.0f));
		this->particles[i].lifetime = Random::Float() + 1.0f; // lstd_random_between(1.0f, 2.0f);
	}

	// create a vertex and position buffer
	glGenBuffers(1, &this->vertexBuffer);
	glGenBuffers(1, &this->positionBuffer);
	//this->vertexBuffer = lglGenBuffer();
	//this->positionBuffer = lglGenBuffer();

	// fill the vertex buffer
	std::vector< float > vertices;
	vertices.push_back(0.0f);
	vertices.push_back(0.0f);
	vertices.push_back(0.0f);

	vertices.push_back(1.0f);
	vertices.push_back(0.0f);
	vertices.push_back(0.0f);

	vertices.push_back(0.0f);
	vertices.push_back(1.0f);
	vertices.push_back(0.0f);

	vertices.push_back(1.0f);
	vertices.push_back(1.0f);
	vertices.push_back(0.0f);

	glBindBuffer(GL_ARRAY_BUFFER, this->vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

	// fill the position buffer
	glBindBuffer(GL_ARRAY_BUFFER, this->positionBuffer);
	glBufferData(GL_ARRAY_BUFFER, this->particles.size() * 4 * sizeof(float), this->positions, GL_DYNAMIC_DRAW);
}



SimpleEmitter::~SimpleEmitter()
{
	glDeleteBuffers(1, &this->vertexBuffer);
	glDeleteBuffers(1, &this->positionBuffer);
}



void SimpleEmitter::update(const float dt)
{
	for (int i = 0; i < this->particles.size(); ++i)
	{
		// subtract from the particles lifetime
		this->particles[i].lifetime -= dt;

		// if the lifetime is below 0 respawn the particle
		if (this->particles[i].lifetime <= 0.0f)
		{
			this->particles[i].position = glm::vec3(Random::Float() * 2 - 1.0f);//lstd_random_between(vec3(-1.0f), vec3(1.0f));
			this->particles[i].lifetime = Random::Float() + 1.0f; //lstd_random_between(1.0f, 2.0f);
		}

		// move the particle down depending on the delta time
		this->particles[i].position -= glm::vec3(0.0f, dt * 2.0f, 0.0f);

		// update the position buffer
		this->positions[i * 4 + 0] = this->particles[i].position[0];
		this->positions[i * 4 + 1] = this->particles[i].position[1];
		this->positions[i * 4 + 2] = this->particles[i].position[2];
		this->positions[i * 4 + 3] = this->particles[i].lifetime;
	}
}



void SimpleEmitter::draw()
{
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(4);

	// update the position buffer
	glBindBuffer(GL_ARRAY_BUFFER, this->positionBuffer);
	glBufferSubData(GL_ARRAY_BUFFER, 0, this->particles.size() * 4 * sizeof(float), this->positions);

	// vertex buffer
	glBindBuffer(GL_ARRAY_BUFFER, this->vertexBuffer);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

	// position buffer
	glBindBuffer(GL_ARRAY_BUFFER, this->positionBuffer);
	glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, 0, nullptr);
	glVertexAttribDivisor(4, 1);

	// draw triangles
	glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, this->particles.size());

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(4);
}



/*
shader
*/



//const std::string SimpleParticleShader::VS = "#version 330 core\n"
//"layout ( location = 0 ) in vec3 vertex_position;"
//"layout ( location = 4 ) in vec4 position;"
//"uniform mat4 M_v;"
//"uniform mat4 M_p;"
//"uniform float particleSize;"
//"out float lifetime;"
//"void main()"
//"{"
//"   vec4 position_viewspace = M_v * vec4( position.xyz , 1 );"
//"   position_viewspace.xy += particleSize * (vertex_position.xy - vec2(0.5));"
//"   gl_Position = M_p * position_viewspace;"
//"   lifetime = position.w;"
//"}";
//
//const std::string SimpleParticleShader::FS = "#version 330 core\n"
//"in float lifetime;"
//"out vec4 fragColor;"
//"void main()"
//"{"
//"   fragColor = vec4( 1.0 );"
//"}";



//SimpleParticleShader::SimpleParticleShader() : lglShader("SimpleParticleShader")
//{
//	this->createFromString(VS, FS);
//
//	this->u_M_v = this->getUniformLocation("M_v");
//	this->u_M_p = this->getUniformLocation("M_p");
//	this->u_particleSize = this->getUniformLocation("particleSize");
//}
//
//
//
//SimpleParticleShader::~SimpleParticleShader()
//{
//
//}
//
//
//
//void SimpleParticleShader::setUniformViewMatrix(const glm::mat4& M_v)
//{
//	this->uniformMatrix4fv(this->u_M_v, M_v);
//}
//
//
//
//void SimpleParticleShader::setUniformProjectionMatrix(const glm::mat4& M_p)
//{
//	this->uniformMatrix4fv(this->u_M_p, M_p);
//}
//
//
//
//void SimpleParticleShader::setUniformParticleSize(const float particleSize)
//{
//	this->uniform1f(this->u_particleSize, particleSize);
//}



/*
controller
*/



SimpleController::SimpleController()
{
	this->myShader = new Shader(4, 4, "particles_vertex_shader.glsl", "particles_fragment_shader.glsl");
	this->emitter = new SimpleEmitter();
}



SimpleController::~SimpleController()
{
	delete this->myShader;
	delete this->emitter;
}



void SimpleController::draw(const float dt, const glm::mat4 M_p, const glm::mat4& M_v)
{
	this->emitter->update(dt);

	//this->myShader->use();
	this->myShader->setMat4fv(M_p, "M_p"); //this->shader->setUniformProjectionMatrix(M_p);
	this->myShader->setMat4fv(M_v, "M_v"); //this->shader->setUniformViewMatrix(M_v);
	this->myShader->set1f(5.f, "particleSize"); //this->shader->setUniformParticleSize(0.1f);
	this->myShader->use();
	this->emitter->draw();
}