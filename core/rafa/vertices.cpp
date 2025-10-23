#include "vertices.h"
using namespace vert;


//Creates a new OpenGL VAO, VBO, and EBO, filling the VBO with vertices, EBO with indices
Mesh::Mesh(const MeshData& meshData)
{

}

//Draws a mesh. Just binds the VAO and does a draw call.
//If drawAsPoints is true, use GL_POINTS instead of GL_TRIANGLES for the draw call
void Mesh::draw(bool drawAsPoints)
{

}

//These functions create a new instance of MeshData and populate its vertices and indices.
MeshData vert::createSphere(float radius, int segments)
{
	MeshData null;
	return null;
}
MeshData vert::createCylinder(float height, float radius, int segments)
{
	MeshData null;
	return null;
}
MeshData vert::createPlane(float width, float height, int segments)
{
	MeshData null;
	return null;
}