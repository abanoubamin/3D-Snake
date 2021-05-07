#include <gl/glew.h>
#include <glm/glm.hpp>
#include "texture.h"

using namespace glm;

class square
{
private:
	vec3 p1;
	vec3 p2;
	vec3 p3;
	vec3 p4;
	vec3 c1;
	vec3 c2;
	vec3 c3;
	vec3 c4;
	vec2 t1;
	vec2 t2;
	vec2 t3;
	vec2 t4;

public :
	enum plane {XY, XZ, YZ};
	Texture * texture;
	GLfloat  mat[48];//(3 positions,3 color ,2 texture) * 4
	mat4 mMat;
	static const int sizeOfSquare = 192;//6 points * 8 elements * sizeof(float)

	square();
	square(vec3, float, plane);

	void setColors(vec3 , vec3 , vec3 , vec3);
	void setTexture(vec2, vec2, vec2, vec2);
	void bindTex(char *, int);
	void updateMat();
};
