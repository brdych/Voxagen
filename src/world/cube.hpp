#ifndef CUBE_H
#define CUBE_H

#include <glm/vec3.hpp>

class Cube {
public:    
    Cube();

    ~Cube();
    
    bool active;
    glm::vec3 color;
};

#endif // CUBE_H
