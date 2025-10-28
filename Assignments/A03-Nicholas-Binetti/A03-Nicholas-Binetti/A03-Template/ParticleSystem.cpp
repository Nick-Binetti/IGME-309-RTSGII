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

		int posIndex = i * 3;
		int colIndex = i * 4;
		
		lifeTimes[i] += deltaTime;
		
		velocities[posIndex+1] += acceleration[1] * deltaTime; //only affect y velocity(down)

		positions[posIndex] += velocities[posIndex] * deltaTime; //x pos
		positions[posIndex+1] += velocities[posIndex+1] * deltaTime; //y pos
		positions[posIndex+2] += velocities[posIndex+2] * deltaTime; //z pos

		colors[colIndex + 3] = 1.0f- lifeTimes[i] / maxLifeTime; 

		// Reset particle states (positions, velocities, colors, and lifetimes) when the lifetime reaches the maxLifeTime
		if (lifeTimes[i] > maxLifeTime)
		{
			//reset everything
			positions[posIndex] = 0.0f; // x
			positions[posIndex + 1] = 0.0f; // y
			positions[posIndex + 2] = 0.0f; // z

			velocities[posIndex] = getRandomValue(minSpeedX, maxSpeedX);
			velocities[posIndex + 1] = getRandomValue(minSpeedY, maxSpeedY);
			velocities[posIndex + 2] = getRandomValue(minSpeedZ, maxSpeedZ);

			colors[colIndex] = 0.0f; //r
			colors[colIndex + 1] = 0.0f; // g
			colors[colIndex + 2] = 1.0f; // b
			colors[colIndex + 3] = 1.0f; // a

			lifeTimes[i] = 0.0f;
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
		glPointSize(3.0f);

		glBegin(GL_POINTS);

		for (int i = 0; i < numParticles; i++) {
			int posIndex = i * 3;
			int colIndex = i * 4;

			glColor4f(colors[colIndex], colors[colIndex + 1], colors[colIndex + 2], colors[colIndex + 3]);
			glVertex3f(positions[posIndex], positions[posIndex + 1], positions[posIndex + 2]);
		}
		glEnd();
	// Write your code above
	/***************************/
}

float ParticleSystem::getRandomValue(float min_value, float max_value)
{
	return min_value + (std::rand()) / (RAND_MAX / (max_value - min_value));;
}
