#pragma once

using namespace std;

class ParticleSystem
{
private:
	// You can modify any values as needed.
	// Define new variables and functions if necessary.

	GLfloat* positions;		// holds particle positions in the format (x, y, z).
	GLfloat* colors;		// holds particle colors in the format (r, g, b, a).
	GLfloat* velocities;	// contains particles’ velocities in the format (x, y, z).
	GLfloat* lifeTimes;		// represents the current lifetime of every particle. 

	int numParticles;		// Number of particles in the system

	float acceleration[3] = { 0.0f, -9.8f, 0.0f }; // Gravity
	float maxLifeTime = 5.0f;	// The maximum life time the particle could exsit
	
	// speed ranges
	float minSpeedX = -5.0f;
	float maxSpeedX = 5.0f;
	float minSpeedY = 10.0f;
	float maxSpeedY = 20.0f;
	float minSpeedZ = -5.0f;
	float maxSpeedZ = 5.0f;

public:
	ParticleSystem(int _numParticles);	
	void update(float deltaTime);
	void draw();
	int getNumParticles() {
		return numParticles;
	}

private:
	float getRandomValue(float min_value, float max_value);	// Return a random float value between min and max
};

