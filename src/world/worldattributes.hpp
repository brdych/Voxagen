#ifndef WORLDATTRIBUTES_H
#define WORLDATTRIBUTES_H

#include <map>
#include <glm/vec3.hpp>

class WorldAttributes
{
public:
    WorldAttributes();
    static std::map<int, glm::vec3> VOXEL_MATERIALS;
};

#endif // WORLDATTRIBUTES_H
