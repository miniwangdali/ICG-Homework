#ifndef CUBEMAP_HPP
#define CUBEMAP_HPP

#include <iostream>
#include <vector>
#include "glsupport.h"
#include "stb_image.h"

class Cubemap {
private:
	std::vector<std::string> filenames;
	GLuint textureObject;
public:
	Cubemap(const std::string &frontFilename,
			const std::string &backFilename,
			const std::string &upFilename,
			const std::string &downFilename, 
			const std::string &rightFilename,
			const std::string &leftFilename ) {
		filenames.resize(6);
		filenames[0] = frontFilename;
		filenames[1] = backFilename;
		filenames[2] = upFilename;
		filenames[3] = downFilename;
		filenames[4] = rightFilename;
		filenames[5] = leftFilename;
		textureObject = loadGLCubemap(filenames);
	}
	GLuint loadGLCubemap(std::vector<std::string> faces) {
		GLuint textureID;
		glGenTextures(1, &textureID);
		glActiveTexture(GL_TEXTURE0);

		glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);
		for (GLuint i = 0; i < faces.size(); i++) {
			int w, h, comp;
			unsigned char* image = stbi_load(faces[i].c_str(), &w, &h, &comp, STBI_rgb_alpha);
			if (image) {
				glTexImage2D(
					GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0,
					GL_RGB, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, image
					);
				stbi_image_free(image);
			}
		}
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
		glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
		return textureID;
	}
	void bind(GLenum textureUnit) {
		glActiveTexture(textureUnit);
		glBindTexture(GL_TEXTURE_CUBE_MAP, textureObject);
	}
};

#endif // !CUBEMAP_HPP