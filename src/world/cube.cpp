#include "world/cube.h"

Cube::Cube()
{

}

Cube::~Cube()
{

}

bool Cube::getActive() {
    return _active;
}

void Cube::setActive(bool active) {
         _active = active;
}
