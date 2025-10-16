#include <GL/glew.h>
#include <GL/freeglut.h>
#include "ParticleSystem.h"
#include <iostream>

ParticleSystem::ParticleSystem(int _numParticles)
{
	numParticles = _numParticles;	// Set the number of particles
	
	// Allocate memory for positions, velocities, colors, and lifetimes.
	positions = new float[numParticles * 3];
	velocities = new float[numParticles * 3];
	colors = new float[numParticles * 4];	
	lifeTimes = new float[numParticles];

	for (int i = 0; i < numParticles; i++) {
		// Initialize the life times
		lifeTimes[i] = maxLifeTime - maxLifeTime * i / numParticles;

		/***************************/
		// Write your code below
		// Please add initializations for other arrays as you see appropriate.
		positions[i] = 0.0f; // x
		positions[i + 1] = 0.0f; // y
		positions[i + 2] = 0.0f; // z

		velocities[i] = getRandomValue(minSpeedX, maxSpeedX);
		velocities[i+1] = getRandomValue(minSpeedY, maxSpeedY);
		velocities[i+2] = getRandomValue(minSpeedZ, maxSpeedZ);

		colors[i] = 0.0f; //r
		colors[i+1] = 0.0f; // g
		colors[i+2] = 1.0f; // b
		colors[i+3] = 1.0f; // a
		/***************************/
	}
}

void ParticleSystem::update(float deltaTime)
{
	for (int i = 0; i < numParticles; i++) {
		/***************************/
		// Write your code below
		// Update lifetime, velocity, position, and color.
		// Reset particle states (positions, velocities, colors, and lifetimes) when the lifetime reaches the maxLifeTime
		lifeTimes[i] += deltaTime;

		//update velocity(grav & deltaTime), position(velocities), and color(ratio of cur vals in lifeTimes to maxLifeTime)

		if (lifeTimes[i] > maxLifeTime)
		{
			//reset everything
		}
		
		// Write your code above
		/***************************/
	}
}

void ParticleSystem::draw()
{
	/***************************/
	// Write your code below
	// Use GL_POINTS for rendering
	
	// Write your code above
	/***************************/
}

float ParticleSystem::getRandomValue(float min_value, float max_value)
{
	return min_value + (std::rand()) / (RAND_MAX / (max_value - min_value));;
}
