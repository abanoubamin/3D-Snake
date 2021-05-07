#include <glm/glm.hpp>
#include <gl/glew.h>
#include "square.h"
#include "texture.h"
#include "cube.h"
#include <glm/gtc/matrix_transform.hpp> 
#include <glm/gtx/transform.hpp>

using namespace::glm;

cube::cube(vec3 point,float leng)
{
	faces[0] = square(point,leng,square::XY);
	faces[1] = square(point, leng, square::XY);
	faces[2] = square(point, leng, square::XZ);
	faces[3] = square(point, leng, square::XZ);
	faces[4] = square(point, leng, square::YZ);
	faces[5] = square(point, leng, square::YZ);

	faces[0].mMat = mat4(1.0f);
	faces[1].mMat = translate(.0f, .0f, 1.0f);
	faces[2].mMat = mat4(1.0f);
	faces[3].mMat = translate(0.0f, 1.0f, .0f);
	faces[4].mMat = mat4(1.0f);
	faces[5].mMat = translate(1.0f, .0f, .0f);
}

void cube::updateMat()
{
	for (int i = 0; i < 6; i++)
	{
		faces[i].updateMat();
	}
}

void cube::draw(GLuint MatrixID,mat4 MVP)
{
	for (int i = 0; i < 6; i++)
	{
		glBufferData(GL_ARRAY_BUFFER, faces[i].sizeOfSquare, faces[i].mat, GL_STATIC_DRAW);
		faces[i].texture->Bind();
		glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &(MVP*(faces[i].mMat))[0][0]);
		glDrawArrays(GL_TRIANGLES, 0, 6);
	}
}