#include <vector>
#include <time.h>
#include <windows.h>
//#include <mmsystem.h>
#include <gl/glew.h>
#include <gl/glfw3.h>
#include "shader.hpp"
#include "texture.h"
#include "FPCamera.h"
#include "myCube.h"

#pragma once

using namespace::std;

class Renderer
{
	int error;
	double timeStart;
	int m = 0; //for start menu
	int sound = 0;
	int score = -1;
	int level_counter = 0;

	square * menu;
	square * levels;
	square * gameover;
	FPCamera* camera;
	myCube * box;
	vector<myCube> * snake;
	myCube * food;
	int timefoodmove = 0;
	myCube * specialFood;
	const static int nmaze = 30;
	myCube * maze[nmaze];
	int level = 0;
	
	GLuint VertexArrayID;
	GLuint VertexBufferID;
	GLuint programID;

	// Transformation
	GLuint MatrixID;
	glm::mat4 MVPsnake;
	enum Direction {North, East, West, South};
	Direction snakeDir;
	Direction dirs[4];
	int dirsPtr;
	
public:
    Renderer();
    ~Renderer();
    
    void Initialize();
    void Draw();
	void Update();
	void Cleanup();

	void HandleKeyboardInput(int);
	void HandleMouseMove(double, double);
	void HandleMouseClick(double, double);
	void followHead();
	int Collision();

	int lost;
	int start = 0;//condition in main loop to prevent generating snake
	int choose = 1;//default choose 1(start)
	int pause = 0;//for pause
	int x = 0;//for gameOver sound
	int rot = 1;//for rotation(dance)
	int level_sign = 1;
};
