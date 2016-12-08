#ifndef GEOMETRY_HPP
#define GEOMETRY_HPP

#include "cvec.h"
#include "matrix4.h"
#include "quat.h"
#include "glsupport.h"
#include "geometrymaker.h"
#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"


struct Vertex {
	Cvec3f position, normal, binormal, tangent;
	Cvec2f texture;
	Vertex() {};
	Vertex(float posX, float posY, float posZ, float normalX, float normalY, float normalZ) : position(posX, posY, posZ), normal(normalX, normalY, normalZ) {};
	Vertex& operator = (const GenericVertex& v) {
		position = v.pos;
		normal = v.normal;
		texture = v.tex;
		binormal = v.binormal;
		tangent = v.tangent;
		return *this;
	}
};

struct Transform {
	Quat rotation;
	Cvec3 scale;
	Cvec3 translation;
	Transform() : scale(1.0, 1.0, 1.0), translation(0.0, 0.0, 0.0), rotation() {};
	Matrix4 createMatrix() {
		Matrix4 origin;
		origin = Matrix4::makeScale(scale) * origin;
		origin = quatToMatrix(rotation) * origin;
		origin = Matrix4::makeTranslation(translation) * origin;
		return origin;
	}
};

class Geometry {
private:
	std::vector<Vertex> vertices;
	std::vector<unsigned short> indices;
	GLuint vertexBufferObject;
	GLuint indexBufferObject;

protected:
	void loadObjFile(const std::string &fileName, std::vector<Vertex> &outVertices, std::vector<unsigned short>& outIndices) {
		tinyobj::attrib_t attrib;
		std::vector<tinyobj::shape_t> shapes;
		std::vector<tinyobj::material_t> materials;
		std::string err;
		bool ret = tinyobj::LoadObj(&attrib, &shapes, &materials, &err, fileName.c_str(), NULL, true);
		if (ret) {
			for (size_t i = 0; i < shapes.size(); i++) {
				for (size_t j = 0; j < shapes[i].mesh.indices.size(); j++) {
					unsigned int vertexOffset = shapes[i].mesh.indices[j].vertex_index * 3;
					unsigned int normalOffset = shapes[i].mesh.indices[j].normal_index * 3;
					unsigned int texOffset = shapes[i].mesh.indices[j].texcoord_index * 2;
					Vertex v;
					v.position[0] = attrib.vertices[vertexOffset];
					v.position[1] = attrib.vertices[vertexOffset + 1];
					v.position[2] = attrib.vertices[vertexOffset + 2];
					v.normal[0] = attrib.normals[normalOffset];
					v.normal[1] = attrib.normals[normalOffset + 1];
					v.normal[2] = attrib.normals[normalOffset + 2];
					v.texture[0] = attrib.texcoords[texOffset];
					v.texture[1] = 1.0 - attrib.texcoords[texOffset + 1];
					outVertices.push_back(v);
					outIndices.push_back(outVertices.size() - 1);
				}
			}
		}
		else {
			std::cout << err << std::endl;
			assert(false);
		}
	}
	void calculateFaceTangent(const Cvec3f &v1, const Cvec3f &v2, const Cvec3f &v3,
		const Cvec2f &texCoord1, const Cvec2f &texCoord2, const Cvec2f &texCoord3,
		Cvec3f &tangent, Cvec3f &binormal) {
		Cvec3f side0 = v1 - v2;
		Cvec3f side1 = v3 - v1;
		Cvec3f normal = cross(side1, side0);
		float deltaV0 = texCoord1[1] - texCoord2[1];
		float deltaV1 = texCoord3[1] - texCoord1[1];
		tangent = side0 * deltaV1 - side1 * deltaV0;
		normalize(tangent);
		float deltaU0 = texCoord1[0] - texCoord2[0];
		float deltaU1 = texCoord3[0] - texCoord1[0];
		binormal = side0 * deltaU1 - side1 * deltaU0;
		normalize(binormal);
		Cvec3f tangentCross = cross(tangent, binormal);
		if (dot(tangentCross, normal) < 0.0f) {
			tangent = tangent * (-1);
		}
	}
public:
	Geometry() {

	}
	void load(const std::string &fileName) {
		loadObjFile(fileName, vertices, indices);
		for (size_t i = 0; i < vertices.size(); i = i + 3) {
			Cvec3f tangent;
			Cvec3f binormal;
			calculateFaceTangent(vertices[i].position, vertices[i + 1].position, vertices[i + 2].position,
				vertices[i].texture, vertices[i + 1].texture, vertices[i + 2].texture, tangent, binormal);
			vertices[i].binormal = binormal;
			vertices[i + 1].binormal = binormal;
			vertices[i + 2].binormal = binormal;

			vertices[i].tangent = tangent;
			vertices[i + 1].tangent = tangent;
			vertices[i + 2].tangent = tangent;
		}
		// create vertex position VBOs
		glGenBuffers(1, &vertexBufferObject);
		glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
		glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * vertices.size(), vertices.data(), GL_STATIC_DRAW);
		// create vertex index VBOs
		glGenBuffers(1, &indexBufferObject);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferObject);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned short) * indices.size(), indices.data(), GL_STATIC_DRAW);

	}
	void cube(float length) {
		int ibLen, vbLen;
		getCubeVbIbLen(vbLen, ibLen);
		vertices.resize(vbLen);
		indices.resize(ibLen);
		makeCube(length, vertices.begin(), indices.begin());
		
		// create vertex position VBOs
		glGenBuffers(1, &vertexBufferObject);
		glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
		glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * vertices.size(), vertices.data(), GL_STATIC_DRAW);
		// create vertex index VBOs
		glGenBuffers(1, &indexBufferObject);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferObject);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned short) * indices.size(), indices.data(), GL_STATIC_DRAW);
	}
	void sphere(float radius, int slices, int stacks) {
		int ibLen, vbLen;
		getSphereVbIbLen(slices, stacks, vbLen, ibLen);
		vertices.resize(vbLen);
		indices.resize(ibLen);
		makeSphere(radius, slices, stacks, vertices.begin(), indices.begin());

		// create vertex position VBOs
		glGenBuffers(1, &vertexBufferObject);
		glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
		glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * vertices.size(), vertices.data(), GL_STATIC_DRAW);
		// create vertex index VBOs
		glGenBuffers(1, &indexBufferObject);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferObject);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned short) * indices.size(), indices.data(), GL_STATIC_DRAW);
	}
	GLuint getVBO() {
		return vertexBufferObject;
	}
	GLuint getIBO() {
		return indexBufferObject;
	}
	int getIndicesSize() {
		return indices.size();
	}
};

#endif // !GEOMETRY_HPP