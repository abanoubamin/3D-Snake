#include <gl\glew.h>
#include <glm\glm.hpp>
#include "square.h"

square::square()
{

}

square::square(vec3 point1, float len, plane pl)
{

	p1 = point1;

	switch(pl)
	{
		case XY:
			p2.x = p1.x + len;
			p3.x = p1.x + len;
			p4.x = p1.x;

			p2.y = p1.y;
			p3.y = p1.y + len;
			p4.y = p1.y + len;

			p2.z = .0f;
			p3.z = .0f;
			p4.z = .0f;
			break;

		case XZ:
			p2.x = p1.x + len;
			p3.x = p1.x + len;
			p4.x = p1.x;

			p2.y = .0f;
			p3.y = .0f;
			p4.y = .0f;

			p2.z = p1.z;
			p3.z = p1.z + len;
			p4.z = p1.z + len;
			break;

		case YZ:
			p2.x = .0f;
			p3.x = .0f;
			p4.x = .0f;

			p2.y = p1.y + len;
			p3.y = p1.y + len;
			p4.y = p1.y;

			p2.z = p1.z;
			p3.z = p1.z + len;
			p4.z = p1.z + len;
			break;

		default:
			break;
	}

	vec3 defaultColor(.0f, .0f, .0f);
	setColors(defaultColor, defaultColor, defaultColor, defaultColor);

	vec2 defaultTexture(.0f, .0f);
	setTexture(defaultTexture, defaultTexture, defaultTexture, defaultTexture);
}


void square::setColors(vec3 color1, vec3 color2, vec3 color3, vec3 color4)
{
	c1 = color1;
	c2 = color2;
	c3 = color3;
	c4 = color4;
}

void square::setTexture(vec2 text1, vec2 text2, vec2 text3, vec2 text4)
{
	t1 = text1;
	t2 = text2;
	t3 = text3;
	t4 = text4;
}

void square::bindTex(char * path, int id)
{
	texture = new Texture(path, id);
}

void square::updateMat()
{
	GLfloat VertsMat[] =
	{
		//first triangle
		p1.x,p1.y,p1.z,c1.x,c1.y,c1.z,t1.x,t1.y,
		p2.x,p2.y,p2.z,c2.x,c2.y,c2.z,t2.x,t2.y,
		p3.x,p3.y,p3.z,c3.x,c3.y,c3.z,t3.x,t3.y,
		//second triangle
		p1.x,p1.y,p1.z,c1.x,c1.y,c1.z,t1.x,t1.y,
		p3.x,p3.y,p3.z,c3.x,c3.y,c3.z,t3.x,t3.y,
		p4.x,p4.y,p4.z,c4.x,c4.y,c4.z,t4.x,t4.y
	};
	for (int i = 0; i < 48; i++)
	{
		mat[i] = VertsMat[i];
	}
}