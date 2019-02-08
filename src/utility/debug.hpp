#ifndef DEBUG_HPP
#define DEBUG_HPP

#include "utility/shader.hpp"
#include "glm/mat4x4.hpp"
#include "glm/vec3.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "worldvariables.hpp"

class DebugObject
{
private:
    Shader Debug_Shader = Shader("../src/shaders/LightShader.vert","../src/shaders/LightShader.frag");
    GLint LightMatrixID;
    GLuint lightVAO, lightVBO, lightEBO;

public:
    DebugObject()
    {
        GLfloat cube_vertices[] = {
            -0.5f,-0.5f,-0.5f, //BLB 0
            -0.5f,-0.5f, 0.5f, //BLF 1
             0.5f,-0.5f,-0.5f, //BRB 2
             0.5f,-0.5f, 0.5f, //BRF 3
            -0.5f, 0.5f,-0.5f, //TLB 4
            -0.5f, 0.5f, 0.5f, //TLF 5
             0.5f, 0.5f,-0.5f, //TRB 6
             0.5f, 0.5f, 0.5f  //TRF 7
        };
        GLuint cube_indices[] = {
            4,7,6,  //Top
            4,5,7,
            1,2,3,  //Bottom
            1,0,2,
            7,2,6,  //Right
            7,3,2,
            4,1,5,  //Left
            4,0,1,
            6,0,4,  //Back
            6,2,0,
            5,3,7,  //Front
            5,1,3
        };
        LightMatrixID = glGetUniformLocation(Debug_Shader.ID, "MVP");

        glGenBuffers(1, &lightVBO);
        glBindBuffer(GL_ARRAY_BUFFER, lightVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(cube_vertices), cube_vertices, GL_STATIC_DRAW);

        glGenBuffers(1, &lightEBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, lightEBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cube_indices), cube_indices, GL_STATIC_DRAW);

        glGenVertexArrays(1, &lightVAO);
        glBindVertexArray(lightVAO);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
        glEnableVertexAttribArray(0);
    }

    void DrawDebugChunk(int x,int y,int z, glm::mat4 &proj, glm::mat4 &view, glm::vec3 col)
    {
        int s = WorldVariables::CHUNK_SIZE;
        Debug_Shader.use();
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glm::mat4 model = glm::translate(glm::mat4(1), glm::vec3((x*s)-0.5,(y*s)-0.5,(z*s)-0.5));
        model = glm::translate(model, glm::vec3(s/2,s/2,s/2));
        model = glm::scale(model, glm::vec3(s,s,s));
        glm::mat4 mvp = proj * view * model;
        glUniformMatrix4fv(LightMatrixID, 1, GL_FALSE, &mvp[0][0]);
        Debug_Shader.setVec3("COL", col);
        glBindVertexArray(lightVAO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, lightEBO);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, nullptr);
    }

    void DrawDebugCube(int x,int y,int z, glm::mat4 &proj, glm::mat4 &view)
    {
        int s = 1;
        Debug_Shader.use();
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glm::mat4 model = glm::translate(glm::mat4(1), glm::vec3(x,y,z));
        model = glm::scale(model, glm::vec3(s,s,s));
        glm::mat4 mvp = proj * view * model;
        glUniformMatrix4fv(LightMatrixID, 1, GL_FALSE, &mvp[0][0]);
        glBindVertexArray(lightVAO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, lightEBO);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, nullptr);
    }

    ~DebugObject()
    {

    }
};

#endif // DEBUG_HPP
