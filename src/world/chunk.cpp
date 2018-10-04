#include "chunk.h"

Chunk::Chunk() {
    _cubes = new Cube**[CHUNK_SIZE];
    for(int i=0;i<CHUNK_SIZE;i++) {
        _cubes[i] = new Cube*[CHUNK_SIZE];
        for(int j=0;j<CHUNK_SIZE;j++) {
            _cubes[i][j] = new Cube[CHUNK_SIZE];
        }
    }
}

Chunk::~Chunk() {
    for(int i=0;i<CHUNK_SIZE;i++) {
        for(int j=0;j<CHUNK_SIZE;j++) {
            delete _cubes[i][j];
        }
        delete _cubes[i];
    }
    delete _cubes;
}

bool Chunk::getActive() {
    return _active;
}

void Chunk::setActive(bool active) {
    _active = active;
}
