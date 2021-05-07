#pragma once
#ifndef  MYCUBE_H
#define  MYCUBE_H

#include <glm/gtx/transform.hpp>
#include "square.h"

class myCube
{
public:
	float positionX;//for translation
	float positionY;//for translation
	square faces[6];
	mat4 MVP;

	myCube(vec3, float);

	void updateMat();
	void draw(GLuint MatrixID, mat4 MVP);//glUniformMatrix4fv(mvpMatrixID, 1, GL_FALSE, &MVP[0][0]);
};

#endif // ! MYCUBE_H
