#ifndef WORLDATTRIBUTES_H
#define WORLDATTRIBUTES_H

#include <map>
#include <glm/vec3.hpp>

class WorldAttributes
{
public:
    WorldAttributes();
    static std::map<int, glm::vec3> VOXEL_MATERIALS;
    constexpr static const float BLOCK_SIZE = 0.5f;
    static const uint CHUNK_SIZE = 16;
};

#endif // WORLDATTRIBUTES_H
