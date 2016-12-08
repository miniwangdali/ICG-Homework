#ifndef OBJECT3D_HPP
#define OBJECT3D_HPP

#include "geometry.hpp"

class Object3D {
private:
	Transform transform;
	Geometry *geometry;
	Object3D *parent;

	// attributes
	GLuint positionAttribute;
public:
	Object3D() : parent(nullptr), transform(), positionAttribute(NULL) {
		geometry = new Geometry();
	};
	virtual void Render() {
		Draw();
	};
	void Draw() {

		glBindBuffer(GL_ARRAY_BUFFER, geometry->getVBO());
		glEnableVertexAttribArray(positionAttribute);
		glVertexAttribPointer(positionAttribute, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, geometry->getIBO());
		glDrawElements(GL_TRIANGLES, geometry->getIndicesSize(), GL_UNSIGNED_SHORT, 0);

		glDisableVertexAttribArray(positionAttribute);

	}
	Matrix4 getTransformMatrix() {
		if (parent == nullptr) return transform.createMatrix();
		else {
			return parent->getTransformMatrix() * transform.createMatrix();
		}
	}
	Geometry* getGeometry() {
		return geometry;
	}
	Transform* getTransform() {
		return &transform;
	}
};
#endif // !OBJECT3D_HPP