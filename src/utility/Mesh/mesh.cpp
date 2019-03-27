#include "mesh.hpp"
#include "utility/Voxagen/voxagenengine.hpp"

Mesh::Mesh()
{
    _vertices = std::vector<GLfloat>();
    _indices = std::vector<GLuint>();
    //_vertexMap = std::unordered_map<std::string, uint>();
}

void Mesh::SetupShader()
{
    VoxagenEngine::TERRAIN_SHADER.use();
    VoxagenEngine::TERRAIN_SHADER.setVec3("GLOBAL_LIGHT_COL",    WorldVariables::GLOBAL_LIGHT_COL);
    VoxagenEngine::TERRAIN_SHADER.setVec3("GLOBAL_LIGHT_DIR",    WorldVariables::GLOBAL_LIGHT_DIR);
    VoxagenEngine::TERRAIN_SHADER.setVec3("CLEAR_COL",           WorldVariables::CLEAR_COLOUR);
    VoxagenEngine::TERRAIN_SHADER.setVec3("FOG_INFO",            WorldVariables::FOG_INFO);

    if(WorldVariables::CULLING_ENABLED) { glEnable(GL_CULL_FACE); }
    else { glDisable(GL_CULL_FACE); }
    if(WorldVariables::Z_BUFFER_ENABLED) { glEnable(GL_DEPTH_TEST); }
    else { glDisable(GL_DEPTH_TEST); }
    if(WorldVariables::USE_WIREFRAME) { glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); }
    else { glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); }
}

void Mesh::SetupWaterShader()
{
    VoxagenEngine::WATER_SHADER.use();
    VoxagenEngine::WATER_SHADER.setVec3("GLOBAL_LIGHT_COL",    WorldVariables::GLOBAL_LIGHT_COL);
    VoxagenEngine::WATER_SHADER.setVec3("GLOBAL_LIGHT_DIR",    WorldVariables::GLOBAL_LIGHT_DIR);
    VoxagenEngine::WATER_SHADER.setVec3("CLEAR_COL",           WorldVariables::CLEAR_COLOUR);
    VoxagenEngine::WATER_SHADER.setVec3("FOG_INFO",            WorldVariables::FOG_INFO);
    glUniform1i(glGetUniformLocation(VoxagenEngine::WATER_SHADER.ID, "normalMap"), 0); // set it manually

    if(WorldVariables::CULLING_ENABLED) { glEnable(GL_CULL_FACE); }
    else { glDisable(GL_CULL_FACE); }
    if(WorldVariables::Z_BUFFER_ENABLED) { glEnable(GL_DEPTH_TEST); }
    else { glDisable(GL_DEPTH_TEST); }
    if(WorldVariables::USE_WIREFRAME) { glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); }
    else { glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); }
}

void Mesh::StartMesh()
{
    if(meshFinished)
    {
        meshFinished = false;
        _vertices.clear();
        _vertices.shrink_to_fit();
        _indices.clear();
        _vertices.shrink_to_fit();
        //_vertexMap.clear();
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

    // Color Pointer
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(GLfloat), (GLvoid*)(3*sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

    // Normal Pointer
    glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, 7 * sizeof(GLfloat), (GLvoid*)(6*sizeof(GLfloat)));
    glEnableVertexAttribArray(2);

    _vertices.clear();
    _vertices.shrink_to_fit();
    _indices.clear();
    _vertices.shrink_to_fit();
    //_vertexMap.clear();

    meshFinished = true;
}

Mesh::~Mesh()
{
    _vertices.clear();
    _vertices.shrink_to_fit();
    _indices.clear();
    _indices.shrink_to_fit();
    //_vertexMap.clear();

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
    //int i = GetVertex(p,n,c);
    //if(i < 0)
    //{
        // Position
        _vertices.push_back(p.x);
        _vertices.push_back(p.y);
        _vertices.push_back(p.z);
        // Color
        _vertices.push_back(c.x);
        _vertices.push_back(c.y);
        _vertices.push_back(c.z);
        // Normal
        _vertices.push_back(n);
        //_vertexMap.emplace(HashVertex(p,n,c), _vertexCount);
        return _vertexCount++;
    //}
    //return static_cast<GLuint>(i);
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
    /*
    std::string hash = HashVertex(p,n,col);
    if(_vertexMap.find(hash)!=_vertexMap.end())
    {
        return static_cast<int>(_vertexMap.at(hash));
    }
    */
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
    glUniformMatrix4fv(glGetUniformLocation(VoxagenEngine::TERRAIN_SHADER.ID, "MVP"), 1, GL_FALSE, &mvp[0][0]);
    glBindVertexArray(_chunkVAO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _chunkEBO);
    glDrawElements(GL_TRIANGLES, static_cast<int>(_indexCount)*3, GL_UNSIGNED_INT, nullptr);
}

void Mesh::RenderWater(glm::mat4 mvp)
{
    if(!meshFinished)
        FinishMesh();
    glUniformMatrix4fv(glGetUniformLocation(VoxagenEngine::WATER_SHADER.ID, "MVP"), 1, GL_FALSE, &mvp[0][0]);
    glBindVertexArray(_chunkVAO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _chunkEBO);
    glDrawElements(GL_TRIANGLES, static_cast<int>(_indexCount)*3, GL_UNSIGNED_INT, nullptr);
}

bool Mesh::ShouldRender()
{
    return _indexCount > 0;
}

bool Mesh::EmptyMesh()
{
    return _indexCount == 0;
}
