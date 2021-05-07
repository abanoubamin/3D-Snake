#include "Renderer.h"

Renderer::Renderer()
{
    
}

Renderer::~Renderer()
{
    Cleanup();
}


void Renderer::Initialize()
{
	menu = new square(vec3(-1.0f, -1.0f, .0f), 2.0f, square::XY);
	menu->setTexture(vec2(.0f, 1.0f), vec2(1.0f, 1.0f), vec2(1.0f, .0f), vec2(.0f, .0f));
	menu->bindTex("start1.png", 20);
	menu->updateMat();

	levels = new square(vec3(-1.0f, -1.0f, .0f), 2.0f, square::XY);

	gameover = new square(vec3(-1.0f, -1.0f, .0f), 2.0f, square::XY);
	gameover->setTexture(vec2(.0f, 1.0f), vec2(1.0f, 1.0f), vec2(1.0f, .0f), vec2(.0f, .0f));
	gameover->bindTex("over.jpg", 20);
	gameover->updateMat();

	lost = 0;
	error = 0;

	timeStart = glfwGetTime();

	srand(time(NULL));

	snakeDir = North;
	dirs[0] = North;
	dirs[1] = East;
	dirs[2] = South;
	dirs[3] = West;
	dirsPtr = 0;

	camera = new FPCamera();
	camera->Reset(25.0f, 50.0f, 25.0f, 25.0f, 0.0f, 25.0f, 1.0f, 0.0f, 0.0f); //not used
	camera->SetPerspectiveProjection(60.0f, 4.0f / 4.0f, 10.0f, 200.0f);
	camera->UpdateViewMatrix();
		
	box = new myCube(vec3(.0f, .0f, .0f), 1.0f);
	box->faces[0].bindTex("posx.jpg", 0);
	box->faces[1].bindTex("negx.jpg", 1);
	box->faces[2].bindTex("negy.jpg", 2);
	box->faces[3].bindTex("posy.jpg", 3);
	box->faces[4].bindTex("posz.jpg", 4);
	box->faces[5].bindTex("negz.jpg", 5);
	box->updateMat();

	myCube head(vec3(.0f, .0f, .0f), 1.0f);
	for (int i = 0; i < 6; i++)
	{
		head.faces[i].bindTex("head.jpg", i + 6);
	}
	head.positionX = 10.0f;
	head.positionY = 10.0f;
	head.updateMat();
	snake = new vector<myCube>();
	snake->push_back(head);

	food = new myCube(vec3(.0f, .0f, .0f), 1.0f);
	for (int i = 0; i < 6; i++)
	{
		food->faces[i].bindTex("food.jpg", i + 12);
	}
	food->positionX = rand() % 100;
	food->positionY = rand() % 100;
	food->updateMat();

	specialFood = new myCube(vec3(.0f, .0f, .0f), 1.0f);
	for (int i = 0; i < 6; i++)
	{
		specialFood->faces[i].bindTex("food2.jpg", i + 12);
	}
	specialFood->positionX = rand() % 100;
	specialFood->positionY = rand() % 100;
	specialFood->updateMat();

	for (int i = 0; i < nmaze / 3; i++)
	{
		maze[i] = new myCube(vec3(.0f, .0f, .0f), 1.0f);
		for (int j = 0; j < 6; j++)
		{
			maze[i]->faces[j].bindTex("obstacle1.jpg", j + 12);
		}
		maze[i]->positionX = rand() % 100;
		maze[i]->positionY = rand() % 100;

		printf("%f  %f\n", maze[i]->positionX, maze[i]->positionY);

		maze[i]->updateMat();
	}
	for (int i = nmaze / 3; i < 2 * nmaze / 3; i++)
	{
		maze[i] = new myCube(vec3(.0f, .0f, .0f), 1.0f);
		for (int j = 0; j < 6; j++)
		{
			maze[i]->faces[j].bindTex("obstacle2.jpg", j + 12);
		}
		maze[i]->positionX = rand() % 100;
		maze[i]->positionY = rand() % 100;

		printf("%f  %f\n", maze[i]->positionX, maze[i]->positionY);

		maze[i]->updateMat();
	}
	for (int i = 2 * nmaze / 3; i < nmaze; i++)
	{
		maze[i] = new myCube(vec3(.0f, .0f, .0f), 1.0f);
		for (int j = 0; j < 6; j++)
		{
			maze[i]->faces[j].bindTex("obstacle3.jpg", j + 12);
		}
		maze[i]->positionX = rand() % 100;
		maze[i]->positionY = rand() % 100;

		printf("%f  %f\n", maze[i]->positionX, maze[i]->positionY);

		maze[i]->updateMat();
	}

	
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);
	glClearColor(0.0f, 0.0f, 0.4f, 1.0f);
	glGenBuffers(1, &VertexBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, VertexBufferID);
	programID = LoadShaders("SimpleTransformWithColor.vertexshader", "MultiColor.fragmentshader");
	MatrixID = glGetUniformLocation(programID, "MVP");
}

void Renderer::Draw()
{
	// Enable depth test
	glEnable(GL_DEPTH_TEST);
	// Accept fragment if it closer to the camera than the former one
	glDepthFunc(GL_LESS);

	// Clear the screen
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	// Use our shader
	glUseProgram(programID);


	
	if (sound == 0 && start == 1)
	{
		PlaySound(TEXT("snake.wav"), NULL, SND_LOOP | SND_ASYNC);
		sound = 1;
	}

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(
			0,                  // attribute 0.first attribute
			3,                  // size
			GL_FLOAT,           // type
			GL_FALSE,           // normalized?
			8 * sizeof(GLfloat),   // stride: is the step between the beginning of the first vertex and the beginning of the second
			(void*)0            // array buffer offset
		);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(
			1,                  // attribute 1. No particular reason for 0, but must match the layout in the shader.
			3,                  // size
			GL_FLOAT,           // type
			GL_FALSE,           // normalized?
			8 * sizeof(GLfloat),  // stride: is the step between the beginning of the first color and the beginning of the second
			(char*) (3 * sizeof(GLfloat))        // the start offset of the first color.
			);
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(
			2,                  // attribute 1. No particular reason for 0, but must match the layout in the shader.
			2,                  // size
			GL_FLOAT,           // type
			GL_FALSE,           // normalized?
			8 * sizeof(GLfloat),  // stride: is the step between the beginning of the first color and the beginning of the second
			(char*) (6 * sizeof(GLfloat))        // the start offset of the first color.
			);

		if (m == 0)
		{
			glBufferData(GL_ARRAY_BUFFER, menu->sizeOfSquare, menu->mat, GL_STATIC_DRAW);
			glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &(mat4(1.0f))[0][0]);
			menu->texture->Bind();
			glDrawArrays(GL_TRIANGLES, 0, 6);
		}
		else if (score == -1)
		{
			if (level_counter < 100)
			{
				level_sign = 0;
				levels->setTexture(vec2(.0f, 1.0f), vec2(1.0f, 1.0f), vec2(1.0f, .0f), vec2(.0f, 0.f));
				levels->bindTex("level1.png", 40);
				levels->updateMat();
				glBufferData(GL_ARRAY_BUFFER, levels->sizeOfSquare, levels->mat, GL_STATIC_DRAW);
				glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &(mat4(1.0f))[0][0]);
				levels->texture->Bind();
				glDrawArrays(GL_TRIANGLES, 0, 6);
				level_counter++;
			}
			if (level_counter == 100)
			{
				level_counter = 0;
				level_sign = 1;
				score = 0;
			}
		}
		
		else if (lost == 0)
		{
			if (level_sign == 1)
			{
				box->draw(MatrixID, box->MVP);

				for (unsigned i = 0; i < snake->size(); i++)
				{
					snake->at(i).draw(MatrixID, snake->at(i).MVP);
				}

				for (int i = 0; i < nmaze; i++)
				{
					if (maze[i]->positionX == food->positionX && maze[i]->positionY == food->positionY)
					{
						food->positionX = rand() % 100;
						food->positionY = rand() % 100;
						food->updateMat();
					}
				}
				food->draw(MatrixID, food->MVP);

				for (int i = 0; i < nmaze; i++)
				{
					if (maze[i]->positionX == specialFood->positionX && maze[i]->positionY == specialFood->positionY)
					{
						specialFood->positionX = rand() % 100;
						specialFood->positionY = rand() % 100;
						specialFood->updateMat();
					}
				}
				if (score != 0 && score % 30 == 0)
				{
					specialFood->draw(MatrixID, specialFood->MVP);
				}
			}
			
			if (score >= 150)
			{
				level = 3;
				if (level_counter < 150)
				{
					level_sign = 0;
					levels->setTexture(vec2(.0f, 1.0f), vec2(1.0f, 1.0f), vec2(1.0f, .0f), vec2(.0f, 0.f));
					levels->bindTex("level4.png", 40);
					levels->updateMat();
					glBufferData(GL_ARRAY_BUFFER, levels->sizeOfSquare, levels->mat, GL_STATIC_DRAW);
					glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &(mat4(1.0f))[0][0]);
					levels->texture->Bind();
					glDrawArrays(GL_TRIANGLES, 0, 6);
					level_counter++;
					if (level_counter == 101)
					{
						box->faces[0].bindTex("level3_wall.jpg", 0);
						box->faces[1].bindTex("level3_wall.jpg", 1);
						box->faces[2].bindTex("level3_ground.jpg", 2);
						box->faces[3].bindTex("level3_wall.jpg", 3);
						box->faces[4].bindTex("level3_wall.jpg", 4);
						box->faces[5].bindTex("level3_wall.jpg", 5);
						box->updateMat();
						box->draw(MatrixID, box->MVP);
					}
				}
			}
			else if (score >= 100)
			{
				level = 2;
				if (level_counter < 100)
				{
					level_sign = 0;
					levels->setTexture(vec2(.0f, 1.0f), vec2(1.0f, 1.0f), vec2(1.0f, .0f), vec2(.0f, 0.f));
					levels->bindTex("level3.png", 40);
					levels->updateMat();
					glBufferData(GL_ARRAY_BUFFER, levels->sizeOfSquare, levels->mat, GL_STATIC_DRAW);
					glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &(mat4(1.0f))[0][0]);
					levels->texture->Bind();
					glDrawArrays(GL_TRIANGLES, 0, 6);
					level_counter++;
					if (level_counter == 51)
					{
						box->faces[0].bindTex("level2_wall.png", 0);
						box->faces[1].bindTex("level2_wall.png", 1);
						box->faces[2].bindTex("level2_ground.png", 2);
						box->faces[3].bindTex("level2_wall.png", 3);
						box->faces[4].bindTex("level2_wall.png", 4);
						box->faces[5].bindTex("level2_wall.png", 5);
						box->updateMat();
						box->draw(MatrixID, box->MVP);
					}
				}
			}
			else if (score >= 50)
			{
				level = 1;
				if (level_counter < 50)
				{
						level_sign = 0;
						levels->setTexture(vec2(.0f, 1.0f), vec2(1.0f, 1.0f), vec2(1.0f, .0f), vec2(.0f, 0.f));
						levels->bindTex("level2.png", 40);
						levels->updateMat();
						glBufferData(GL_ARRAY_BUFFER, levels->sizeOfSquare, levels->mat, GL_STATIC_DRAW);
						glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &(mat4(1.0f))[0][0]);
						levels->texture->Bind();
						glDrawArrays(GL_TRIANGLES, 0, 6);
						level_counter++;
						if (level_counter == 1)
						{
							box->faces[0].bindTex("wood.jpg",0);
							box->faces[1].bindTex("wood.jpg", 1);
							box->faces[2].bindTex("nwood.jpg", 2);
							box->faces[3].bindTex("wood.jpg", 3);
							box->faces[4].bindTex("wood.jpg", 4);
							box->faces[5].bindTex("wood.jpg", 5);
							box->updateMat();
							box->draw(MatrixID, box->MVP);
						}
				}
			}

			if (level == 1 && level_counter == 50)
			{
				level_sign = 1;
				for (int i = 0; i < nmaze / 3; i++)
				{
					maze[i]->draw(MatrixID, maze[i]->MVP);
				}
				
			}
			else if (level == 2 && level_counter == 100)
			{
				level_sign = 1;
				for (int i = 0; i < 2 * nmaze / 3; i++)
				{
					maze[i]->draw(MatrixID, maze[i]->MVP);
				}
			}
			else if (level == 3 && level_counter == 150)
			{
				level_sign= 1;
				for (int i = 0; i < nmaze; i++)
				{
					maze[i]->draw(MatrixID, maze[i]->MVP);
				}
			}
		}
		else
		{
			if (x == 0)
			{
				PlaySound(TEXT("game_over.wav"), NULL, SND_ASYNC);
			}
			glBufferData(GL_ARRAY_BUFFER, gameover->sizeOfSquare, gameover->mat, GL_STATIC_DRAW);
			glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &(mat4(1.0f))[0][0]);
			gameover->texture->Bind();
			glDrawArrays(GL_TRIANGLES, 0, 6);
			x++;
			lost++;
		}

	
		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
		glDisableVertexAttribArray(2);
}

void Renderer::Update()
{
	if (snake->begin()->positionX == food->positionX && snake->begin()->positionY == food->positionY)
	{
		PlaySound(TEXT("eat.wav"), NULL, SND_ASYNC);
		score += 10;
		food->positionX = rand() % 100;
		food->positionY = rand() % 100;
		myCube body(vec3(.0f, .0f, .0f), 1.0f);
		for (int i = 0; i < 6; i++)
		{
			body.faces[i].bindTex("skin.jpg", 7);
		}
		body.positionX = snake->begin()->positionX;
		body.positionY = snake->begin()->positionY;
		
		body.updateMat();
		snake->push_back(body);
		printf("Score : %d\n", score);
		sound = 0;
	}

	timefoodmove++;
	if (timefoodmove > 1000)
	{
		timefoodmove = 0;
		specialFood->positionX = rand() % 100;
		specialFood->positionY = rand() % 100;
	}
	
	if (snake->begin()->positionX == specialFood->positionX && snake->begin()->positionY == specialFood->positionY)
	{
		PlaySound(TEXT("eat.wav"), NULL, SND_ASYNC);
		score += 20;
		specialFood->positionX = rand() % 100;
		specialFood->positionY = rand() % 100;
		myCube body(vec3(.0f, .0f, .0f), 1.0f);
		for (int i = 0; i < 6; i++)
		{
			body.faces[i].bindTex("skin.jpg", 7);
		}
		body.positionX = snake->begin()->positionX;
		body.positionY = snake->begin()->positionY;

		body.updateMat();
		snake->push_back(body);
		printf("Score : %d\n", score);
		sound = 0;
	}

	double deltaTime = (glfwGetTime() - timeStart) * 1000;
	if (deltaTime < 200)
	{
		return;
	}
	else
	{
		timeStart = glfwGetTime();
	}

	followHead();

	snakeDir = dirs[dirsPtr];
	switch (snakeDir)
	{
	case North:
		snake->begin()->positionX += 1.0f;
		break;
	case South:
		snake->begin()->positionX -= 1.0f;
		break;
	case West:
		snake->begin()->positionY += 1.0f;
		break;
	case East:
		snake->begin()->positionY -= 1.0f;
		break;
	default:
		break;
	}

	vec3 eye((snake->begin()->positionX - 5.0f) ,  10.0f  , (snake->begin()->positionY - 5.0f) );
	vec3 center(0, 5.0f, 0);
	vec3 up(.0f, 1.0f, .0f);

	switch (snakeDir)
	{
	case North:
		eye.x = (snake->begin()->positionX - 15.0f) ;
		eye.z = snake->begin()->positionY;
		break;
	case South:
		eye.x = (snake->begin()->positionX + 15.0f) ;
		eye.z = snake->begin()->positionY;
		break;
	case West:
		eye.x = snake->begin()->positionX;
		eye.z = (snake->begin()->positionY - 15.0f) ;
		break;
	case East:
		eye.x = snake->begin()->positionX;
		eye.z = (snake->begin()->positionY + 15.0f) ;
		break;
	default:
		break;
	}

	if (Collision() == 1)
	{
		printf("You hit snake body");
		lost = 1;
	}
	else if (Collision() == 2)
	{
		printf("You hit the wall");
		lost = 1;
	}
	else if (Collision() == 3)
	{
		printf("You hit the maze");
		lost = 1;
	}

	switch (snakeDir)//camera direction
	{
	case North:
		center.x = (snake->begin()->positionX + 10.0f) ;
		center.z = snake->begin()->positionY;
		break;
	case South:
		center.x = (snake->begin()->positionX - 10.0f) ;
		center.z = snake->begin()->positionY;
		break;
	case West:
		center.x = snake->begin()->positionX;
		center.z = (snake->begin()->positionY + 10.0f) ;
		break;
	case East:
		center.x = snake->begin()->positionX;
		center.z = (snake->begin()->positionY - 10.0f) ;
		break;
	default:
		break;
	}
	
	/* generate secret number between 1 and 10: */
	
	camera->Reset(eye, center, up);
	camera->UpdateViewMatrix();

	box->MVP = camera->GetProjectionMatrix() * camera->GetViewMatrix();
	MVPsnake = camera->GetProjectionMatrix() * camera->GetViewMatrix();
	food->MVP = camera->GetProjectionMatrix() * camera->GetViewMatrix();
	specialFood->MVP = camera->GetProjectionMatrix() * camera->GetViewMatrix();
	
	glm::mat4 scaleBox = glm::scale(100.0f, 100.0f, 100.0f);
	box->MVP = box->MVP * scaleBox;

	rot *= -1;
	for (unsigned i = 0; i < snake->size(); i++)
	{
		if (i == 0)
			snake->at(i).MVP = MVPsnake * glm::translate(snake->at(i).positionX, .0f, snake->at(i).positionY);// *scSnake;
		else
			snake->at(i).MVP = MVPsnake * glm::translate(snake->at(i).positionX, .0f, snake->at(i).positionY)* glm::rotate((float)(deltaTime * .1), vec3(0, rot, 0));
		//printf("i:%d,x:%f,y:%f\n",i,snake->at(i).positionX, snake->at(i).positionY);
	}

	food->MVP = food->MVP *glm::translate(food->positionX, .0f, food->positionY);//* scSnake;
	specialFood->MVP = specialFood->MVP *glm::translate(specialFood->positionX, .0f, specialFood->positionY);

	if (level == 1)
	{
		for (int i = 0; i < nmaze / 3; i++)
		{
			maze[i]->MVP = camera->GetProjectionMatrix() * camera->GetViewMatrix();
			maze[i]->MVP = maze[i]->MVP *glm::translate(maze[i]->positionX, .0f, maze[i]->positionY);
			maze[i]->MVP = maze[i]->MVP *glm::scale(1.0f, 100.0f, 1.0f);
		}
	}
	else if (level == 2)
	{
		for (int i = 0; i < 2 * nmaze / 3; i++)
		{
			maze[i]->MVP = camera->GetProjectionMatrix() * camera->GetViewMatrix();
			maze[i]->MVP = maze[i]->MVP *glm::translate(maze[i]->positionX, .0f, maze[i]->positionY);
			maze[i]->MVP = maze[i]->MVP *glm::scale(1.0f, 100.0f, 1.0f);
		}
	}
	else if (level == 3)
	{
		for (int i = 0; i < nmaze; i++)
		{
			maze[i]->MVP = camera->GetProjectionMatrix() * camera->GetViewMatrix();
			maze[i]->MVP = maze[i]->MVP *glm::translate(maze[i]->positionX, .0f, maze[i]->positionY);
			maze[i]->MVP = maze[i]->MVP *glm::scale(1.0f, 100.0f, 1.0f);
		}
	}
}

void Renderer::Cleanup()
{
	glDeleteBuffers(1, &VertexBufferID);
	glDeleteVertexArrays(1, &VertexArrayID); //the one we didn't used (will be discussed later)
	glDeleteProgram(programID);
}


void Renderer::HandleKeyboardInput(int key)
{
	if (error == 0)
	{
		switch (key)
		{
			//Movement Control
		case GLFW_KEY_LEFT:
			if (dirsPtr == 3)
				dirsPtr = 0;
			else
				dirsPtr += 1;
			break;
		case GLFW_KEY_RIGHT:
			if (dirsPtr == 0)
				dirsPtr = 3;
			else
				dirsPtr -= 1;
			break;
		case GLFW_KEY_UP:
			menu->bindTex("start1.png", 20);
			menu->updateMat();
			choose = 1;
			break;
		case GLFW_KEY_DOWN:
			menu->bindTex("start2.png", 20);
			menu->updateMat();
			choose = 2;
			break;
		case GLFW_KEY_A:
			score += 10;
			printf("Score = %d\n", score);
			break;
		case GLFW_KEY_ENTER:
			if (choose == 1)
			{
				m++;
				start = 1;
			}
			else if (choose == 2)
				exit(0);
			break;
		case GLFW_KEY_SPACE:
			if (pause == 1)
			{
				start = 1;
				pause = 0;
				sound = 0;
				break;
			}
			start = 0;
			pause = 1;
			PlaySound(NULL, NULL, SND_ASYNC);
			break;
		default:
			break;
		}
		error = 1;
	}
	else
	{
		error = 0;
	}
}

void Renderer::HandleMouseClick(double xpos, double ypos)
{

}

void Renderer::followHead()
{
	for (unsigned i = snake->size() - 1; i >0 ; i--)
	{
		snake->at(i).positionX = snake->at(i-1).positionX;
		snake->at(i).positionY = snake->at(i-1).positionY;
	}
}

int Renderer::Collision()
{
	for (unsigned i = 1; i < snake->size() ; i++)
	{
		if (snake->begin()->positionX == snake->at(i).positionX && snake->begin()->positionY == snake->at(i).positionY)
		{
			return 1;
		}
	}

	if (snake->begin()->positionX > 100 || snake->begin()->positionX < 0 || snake->begin()->positionY > 100 || snake->begin()->positionY < 0)
		return 2;

	if (level == 1)
	{
		for (int i = 0; i < nmaze / 3; i++)
		{
			if (snake->begin()->positionX == maze[i]->positionX && snake->begin()->positionY == maze[i]->positionY)
				return 3;
		}
	}
	else if (level == 2)
	{
		for (int i = 0; i < 2 * nmaze / 3; i++)
		{
			if (snake->begin()->positionX == maze[i]->positionX && snake->begin()->positionY == maze[i]->positionY)
				return 3;
		}
	}
	else if (level == 3)
	{
		for (int i = 0; i < nmaze; i++)
		{
			if (snake->begin()->positionX == maze[i]->positionX && snake->begin()->positionY == maze[i]->positionY)
				return 3;
		}
	}

	return 0;
}