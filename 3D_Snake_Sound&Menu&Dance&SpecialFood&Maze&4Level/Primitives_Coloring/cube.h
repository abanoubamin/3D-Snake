#include <glm/glm.hpp>
#include <gl/glew.h>
#include "square.h"
#include "texture.h"

using namespace::glm;
class cube
{
public:
	cube(vec3,float);
	square faces[6];
	void updateMat();
	void draw(GLuint MatrixID, mat4 MVP);
private:

};