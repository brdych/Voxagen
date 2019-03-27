#ifndef WORLDGENERATOR_HPP
#define WORLDGENERATOR_HPP

#include "utility/Chunk/chunk.hpp"
#include <unordered_map>

class WorldGenerator
{
public:
    static inline std::unordered_map<unsigned char,glm::vec3> BLOCK_TYPES = std::unordered_map<unsigned char, glm::vec3>();
    WorldGenerator(){}
    virtual ~WorldGenerator(){}
    virtual void GenerateChunk(Chunk* c) = 0;
};

#endif // WORLDGENERATOR_HPP
