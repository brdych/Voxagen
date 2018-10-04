#ifndef CHUNK_H
#define CHUNK_H

#include"cube.h"

class Chunk {
public:
    
    static const int CHUNK_SIZE = 8;
    

    Chunk();

    ~Chunk();
    
    void render();
    
    bool getActive();
    
    void setActive(bool active);
    
private:
    
    bool _active;
    Cube*** _cubes;
    
    void renderCube();

};

#endif // CHUNK_H
