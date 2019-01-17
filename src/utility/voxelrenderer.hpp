#ifndef VOXELRENDERER_H
#define VOXELRENDERER_H

#include <vector>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <utility/shader.hpp>
#include <utility/voxagensettings.hpp>


class VoxelRenderer
{
public:
    VoxelRenderer();
    GLuint AddVertex(glm::vec3 p, GLfloat n, glm::vec3 c);
    void AddTriangle(GLuint v1, GLuint v2, GLuint v3);
    void StartMesh();
    void FinishMesh();
    void Render(glm::mat4 mvp);
    static void SetupShader();

private:
    static Shader* _VoxelShader;
    static VoxagenSettings* _Settings;
    static GLint _VoxelShaderMatrixID;
    GLuint vertex_count = 0;
    GLuint index_count = 0;
    GLuint _chunkVAO, _chunkVBO, _chunkEBO;
    std::vector<GLfloat>* _vertices;
    std::vector<GLuint>* _indices;
};

#endif // VOXELRENDERER_H
