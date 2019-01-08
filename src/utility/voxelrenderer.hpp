#ifndef VOXELRENDERER_H
#define VOXELRENDERER_H

#include <vector>
#include <glm/vec3.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>


class VoxelRenderer
{
public:
    VoxelRenderer();
    GLuint AddVertex(glm::vec3 p, glm::vec3 n, glm::vec3 c);
    void Addtriangle(GLuint v1, GLuint v2, GLuint v3);
    void StartMesh();
    void FinishMesh();
    void Render();

private:
    struct VertexInfo
    {
        GLfloat px, py, pz, nx, ny, nz, r, g, b;
        /*bool operator==(const VertexInfo& other)
        {
            return  px==other.px && py==other.py && pz==other.pz &&
                    nx==other.nx && ny==other.ny && nz==other.nz &&
                    r==other.r && g==other.g && b==other.b ? true : false;
        }*/
    };
    const static GLfloat voxel_size;

    GLuint vertex_count = 0;
    GLuint index_count = 0;
    std::vector<VertexInfo> vertices;
    std::vector<GLuint> indices;

};

#endif // VOXELRENDERER_H
