#pragma once

#include <SDL/SDL.h>

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Mesh.h"
#include "GLSLProgram.h"

enum class GameState { PLAY, EXIT };

class MainGame
{
public:
	MainGame();
	~MainGame();

	void run();

	void initSystems();

	void initShaders();

	void gameLoop();

	void processInput();

	void drawGame();

private:

	SDL_Window * m_window;

	int m_screenWidth, m_screenHeight;

	GameState m_gameState;

	Mesh m_mesh;

	GLSLProgram m_colorProgram;
};