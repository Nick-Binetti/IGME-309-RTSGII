# Performance Analysis
### 500 particles - hovers around an average of 2100 fps
### 5,000 particles - hovers around an average of 2000 fps
### 50,000 particles - hovers around an average of 800 fps
### 500,000 particles - hovers around an average of 120 fps
### 5,000,000 particles - hovers around an average of 12 fps

# Implementation
What I implemented for this assignment was instantiateing the positions, velocities, colors, and lifetimes arrays for each particle. Also implemented the updateng for each particle using these arrays. Arrays would fall using a gravity constant apply to the y position of each particle and the color's alpha value would slowly fade to zero. Additionally, I also implemented resetting the particles once the lifetime for a particle grew greater than the maximum lifetime. One little quirk about my pass on this assignment is that upon the first second or so of run time a burst of black particles can be seen. Overall though there weren't any requirements I couldn't complete.



