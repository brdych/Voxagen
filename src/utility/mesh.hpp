#ifndef MESH_HPP
#define MESH_HPP

#include "stb_image.h"

#include <vector>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <unordered_map>

#include "worldvariables.hpp"
#include "utility/shader.hpp"
#include "voxagenrenderer.hpp"


class Mesh
{
public:
    Mesh();
    ~Mesh();
    GLuint AddVertex(glm::vec3 p, GLfloat n, glm::vec3 c);
    void AddTriangle(GLuint v1, GLuint v2, GLuint v3);
    void StartMesh();
    void FinishMesh();
    void Render(glm::mat4 mvp);
    void RenderWater(glm::mat4 mvp);
    bool ShouldRender();
    bool EmptyMesh();
    static void SetupShader();
    static void SetupWaterShader();
    bool meshFinished = false;

private:

    GLuint _chunkVAO = 0, _chunkVBO = 0, _chunkEBO = 0;
    GLuint _vertexCount = 0, _indexCount = 0;
    std::vector<GLfloat> _vertices;
    std::vector<GLuint> _indices;
    std::unordered_map<std::string, uint> _vertexMap;

    int GetVertex(glm::vec3 p, GLfloat n, glm::vec3 col);
    std::string HashVertex(glm::vec3 p, GLfloat n, glm::vec3 col);
};

#endif // MESH_HPP
