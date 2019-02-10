#include "mesh.hpp"

Mesh::Mesh()
{
    _vertices = std::vector<GLfloat>();
    _indices = std::vector<GLuint>();
    _vertexMap = std::unordered_map<std::string, uint>();
}

void Mesh::SetupShader()
{
    VoxagenRenderer::TERRAIN_SHADER.use();
    VoxagenRenderer::TERRAIN_SHADER.setVec3("GLOBAL_LIGHT_COL",    WorldVariables::GLOBAL_LIGHT_COL);
    VoxagenRenderer::TERRAIN_SHADER.setVec3("GLOBAL_LIGHT_DIR",    WorldVariables::GLOBAL_LIGHT_DIR);
    VoxagenRenderer::TERRAIN_SHADER.setVec3("CLEAR_COL",           WorldVariables::CLEAR_COLOUR);
    VoxagenRenderer::TERRAIN_SHADER.setVec3("FOG_INFO",            WorldVariables::FOG_INFO);

    if(WorldVariables::CULLING_ENABLED) { glEnable(GL_CULL_FACE); }
    else { glDisable(GL_CULL_FACE); }
    if(WorldVariables::Z_BUFFER_ENABLED) { glEnable(GL_DEPTH_TEST); }
    else { glDisable(GL_DEPTH_TEST); }
    if(WorldVariables::USE_WIREFRAME) { glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); }
    else { glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); }
}

void Mesh::SetupWaterShader()
{
    VoxagenRenderer::WATER_SHADER.use();
    VoxagenRenderer::WATER_SHADER.setVec3("GLOBAL_LIGHT_COL",    WorldVariables::GLOBAL_LIGHT_COL);
    VoxagenRenderer::WATER_SHADER.setVec3("GLOBAL_LIGHT_DIR",    WorldVariables::GLOBAL_LIGHT_DIR);
    VoxagenRenderer::WATER_SHADER.setVec3("CLEAR_COL",           WorldVariables::CLEAR_COLOUR);
    VoxagenRenderer::WATER_SHADER.setVec3("FOG_INFO",            WorldVariables::FOG_INFO);
    glUniform1i(glGetUniformLocation(VoxagenRenderer::WATER_SHADER.ID, "normalMap"), 0); // set it manually

    if(WorldVariables::CULLING_ENABLED) { glEnable(GL_CULL_FACE); }
    else { glDisable(GL_CULL_FACE); }
    if(WorldVariables::Z_BUFFER_ENABLED) { glEnable(GL_DEPTH_TEST); }
    else { glDisable(GL_DEPTH_TEST); }
    if(WorldVariables::USE_WIREFRAME) { glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); }
    else { glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); }

    /*unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    // set the texture wrapping/filtering options (on the currently bound texture object)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // load and generate the texture
    int width, height, nrChannels;
    unsigned char *data = stbi_load("../src/textures/water_normal.jpg", &width, &height, &nrChannels, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);*/

}

void Mesh::StartMesh()
{
    if(meshFinished)
    {
        meshFinished = false;
        _vertices.clear();
        _indices.clear();
        _vertexMap.clear();
        _vertexCount = 0;
        _indexCount = 0;
    }
}

void Mesh::FinishMesh()
{
    // VAO
    glGenVertexArrays(1, &_chunkVAO);
    glBindVertexArray(_chunkVAO);

    // VBO
    glGenBuffers(1, &_chunkVBO);
    glBindBuffer(GL_ARRAY_BUFFER, _chunkVBO);
    glBufferData(GL_ARRAY_BUFFER, _vertices.size() * sizeof(GLfloat), _vertices.data(), GL_STATIC_DRAW);

    // EBO
    glGenBuffers(1, &_chunkEBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _chunkEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, _indices.size() * sizeof(GLuint), _indices.data(), GL_STATIC_DRAW);

    // Coords Pointer
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(GLfloat), nullptr);
    glEnableVertexAttribArray(0);

    // Normal Pointer
    glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, 7 * sizeof(GLfloat), (GLvoid*)(3*sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

    // Color Pointer
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(GLfloat), (GLvoid*)(4*sizeof(GLfloat)));
    glEnableVertexAttribArray(2);

    meshFinished = true;
}

Mesh::~Mesh()
{
    if(meshFinished)
    {
        if(WorldVariables::PROGRAM_SHOULD_EXIT)
        {
            std::cout << "\t\t\t\tDeleting Mesh:" << "\n\t\t\t\t\tVBO: " << _chunkVBO << "\n\t\t\t\t\tEBO: " << _chunkEBO << "\n\t\t\t\t\tVAO: " << _chunkVAO << std::endl;
        }
        glDeleteBuffers(1,&_chunkVBO);
        glDeleteBuffers(1, &_chunkEBO);
        glDeleteVertexArrays(1, &_chunkVAO);
    }
}

GLuint Mesh::AddVertex(glm::vec3 p, GLfloat n, glm::vec3 c)
{
    int i = GetVertex(p,n,c);
    if(i<0)
    {
        // Position
        _vertices.push_back(p.x);
        _vertices.push_back(p.y);
        _vertices.push_back(p.z);
        // Normal
        _vertices.push_back(n);
        // Color
        _vertices.push_back(c.x);
        _vertices.push_back(c.y);
        _vertices.push_back(c.z);
        //_vertexMap.emplace(HashVertex(p,n,c), _vertexCount);
        return _vertexCount++;
    }
    return i;
}

void Mesh::AddTriangle(GLuint v1, GLuint v2, GLuint v3)
{
    _indices.push_back(v1);
    _indices.push_back(v2);
    _indices.push_back(v3);
    _indexCount++;
}

int Mesh::GetVertex(glm::vec3 p, GLfloat n, glm::vec3 col)
{
    std::string hash = HashVertex(p,n,col);
    if(_vertexMap.find(hash)!=_vertexMap.end())
    {
        return _vertexMap.at(hash);
    }
    return -1;
}

std::string Mesh::HashVertex(glm::vec3 p, GLfloat n, glm::vec3 col)
{
    return  std::to_string(p.x) + "|" + std::to_string(p.y) + "|" + std::to_string(p.z) +
            std::to_string(n) + "|" +
            std::to_string(col.x) + "|" + std::to_string(col.y) + "|" + std::to_string(col.z);
}

void Mesh::Render(glm::mat4 mvp)
{
    if(!meshFinished)
        FinishMesh();
    glUniformMatrix4fv(glGetUniformLocation(VoxagenRenderer::TERRAIN_SHADER.ID, "MVP"), 1, GL_FALSE, &mvp[0][0]);
    glBindVertexArray(_chunkVAO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _chunkEBO);
    glDrawElements(GL_TRIANGLES, _indices.size(), GL_UNSIGNED_INT, nullptr);
}

void Mesh::RenderWater(glm::mat4 mvp)
{
    if(!meshFinished)
        FinishMesh();
    glUniformMatrix4fv(glGetUniformLocation(VoxagenRenderer::WATER_SHADER.ID, "MVP"), 1, GL_FALSE, &mvp[0][0]);
    glBindVertexArray(_chunkVAO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _chunkEBO);
    glDrawElements(GL_TRIANGLES, _indices.size(), GL_UNSIGNED_INT, nullptr);
}

bool Mesh::ShouldRender()
{
    return _indexCount > 0;
}

bool Mesh::EmptyMesh()
{
    return _indexCount == 0;
}
