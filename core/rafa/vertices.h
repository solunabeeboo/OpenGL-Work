#pragma once
#include "header.h"

namespace vert
{
	//Stores data for a single vertex
	struct Vertex 
	{
		glm::vec3 pos;
		glm::vec3 normal;
		glm::vec3 uv;
	};

	//Data structure for storing vertices and indices. Will be passed to Mesh class.
	struct MeshData 
	{
		std::vector<Vertex> vertices;
		std::vector<unsigned int> indices;
	};

	class Mesh
	{
	public:
		//Creates a new OpenGL VAO, VBO, and EBO, filling the VBO with vertices, EBO with indices
		Mesh(const MeshData& meshData);
		//Draws a mesh. Just binds the VAO and does a draw call.
		void draw(bool drawAsPoints); //If drawAsPoints is true, use GL_POINTS instead of GL_TRIANGLES for the draw call
	private:
		unsigned int m_vao, m_vbo, m_ebo;
		unsigned int m_numVertices, m_numIndices;
	};

	//These functions create a new instance of MeshData and populate its vertices and indices.
	MeshData createSphere(float radius, int segments);
	MeshData createCylinder(float height, float radius, int segments);
	MeshData createPlane(float width, float height, int segments);

	////Example usage
	//myLib::MeshData sphereMeshData = myLib::createSphere(0.5f, 64);
	//myLib::Mesh sphereMesh(sphereMeshData);
	//sphereMesh.draw(false);

	////TIPS!
	//glPolygonMode(GL_FRONT, GL_LINE) // Enable wireframe rendering
	//	glPolygonMode(GL_FRONT, GL_FILL) //Disable wireframe rendering
	//	glEnable(GL_CULL_FACE) //Enables face culling
	//	glCullFace(GL_BACK) //Set to backface culling, specifically
	//	glDrawArrays(GL_POINTS, 0, numVertices); //Drawing as points instead of triangles
}