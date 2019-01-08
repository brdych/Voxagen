#include "voxelrenderer.hpp"

VoxelRenderer::VoxelRenderer()
{

}

GLuint VoxelRenderer::AddVertex(glm::vec3 p, glm::vec3 n, glm::vec3 c)
{
    VertexInfo vi;
    //Point
    vi.px = p.x;
    vi.py = p.y;
    vi.pz = p.z;
    //Normal
    vi.nx = n.x;
    vi.ny = n.y;
    vi.nz = n.z;
    //RGB
    vi.r = c.x;
    vi.g = c.y;
    vi.b = c.z;
    vertices[vertex_count] = vi;
    vertex_count++;
    return vertex_count-1;
}

void VoxelRenderer::Addtriangle(GLuint v1, GLuint v2, GLuint v3)
{
    indices[index_count] = v1;
    indices[index_count+1] = v2;
    indices[index_count+2] = v3;
    index_count += 3;
}

void VoxelRenderer::StartMesh()
{

}

void VoxelRenderer::FinishMesh()
{

}

void VoxelRenderer::Render()
{

}
