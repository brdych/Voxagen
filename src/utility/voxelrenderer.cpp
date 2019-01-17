#include "voxelrenderer.hpp"
#include "iostream"

Shader* VoxelRenderer::_VoxelShader;
GLint VoxelRenderer::_VoxelShaderMatrixID;
VoxagenSettings* VoxelRenderer::_Settings;

void VoxelRenderer::SetupShader()
{
    _VoxelShader->use();
    _VoxelShader->setVec3("GLOBAL_LIGHT_COL",    *_Settings->GLOBAL_LIGHT_COL);
    _VoxelShader->setVec3("GLOBAL_LIGHT_DIR",    *_Settings->GLOBAL_LIGHT_DIR);
    _VoxelShader->setVec3("CLEAR_COL",           *_Settings->CLEAR_COLOUR);
    _VoxelShader->setVec3("FOG_INFO",            *_Settings->FOG_INFO);
}

VoxelRenderer::VoxelRenderer()
{
    _vertices = new std::vector<GLfloat>();
    _indices = new std::vector<GLuint>();
    if(_VoxelShader == nullptr)
    {
        _VoxelShader = new Shader("../src/shaders/BlockShader.vert","../src/shaders/BlockShader.frag");
        _VoxelShaderMatrixID = glGetUniformLocation(_VoxelShader->ID, "MVP");
        _Settings = VoxagenSettings::SettingsInstance();
    }
}

GLuint VoxelRenderer::AddVertex(glm::vec3 p, GLfloat n, glm::vec3 c)
{
    _vertices->push_back(p.x);
    _vertices->push_back(p.y);
    _vertices->push_back(p.z);
    _vertices->push_back(n);
    return (_vertices->size()/4)-1;
}

void VoxelRenderer::AddTriangle(GLuint v1, GLuint v2, GLuint v3)
{
    _indices->push_back(v1);
    _indices->push_back(v2);
    _indices->push_back(v3);
}

void VoxelRenderer::StartMesh()
{

}

void VoxelRenderer::FinishMesh()
{
    //VAO
    glGenVertexArrays(1, &_chunkVAO);
    glBindVertexArray(_chunkVAO);

    //VBO
    glGenBuffers(1, &_chunkVBO);
    glBindBuffer(GL_ARRAY_BUFFER, _chunkVBO);
    glBufferData(GL_ARRAY_BUFFER, _vertices->size()*sizeof(GLfloat), _vertices->data(), GL_STATIC_DRAW);

    //EBO
    glGenBuffers(1, &_chunkEBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _chunkEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, _indices->size() * sizeof(GLuint), _indices->data(), GL_STATIC_DRAW);

    //Coords Pointer
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), nullptr);
    glEnableVertexAttribArray(0);

    //Normal Pointer
    glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)(3*sizeof(GLfloat)));
    glEnableVertexAttribArray(1);
}

void VoxelRenderer::Render(glm::mat4 mvp)
{
    glUniformMatrix4fv(_VoxelShaderMatrixID, 1, GL_FALSE, &mvp[0][0]);
    glBindVertexArray(_chunkVAO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _chunkEBO);
    glDrawElements(GL_TRIANGLES, _indices->size(), GL_UNSIGNED_INT, nullptr);
}
