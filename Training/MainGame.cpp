#include "MainGame.h"
#include "Errors.h"


MainGame::MainGame()
{
	m_window = nullptr;
	m_screenWidth = 1024;
	m_screenHeight = 762;
	m_gameState = GameState::PLAY;

}


MainGame::~MainGame()
{
}
void MainGame::run() {

	initSystems();
	m_mesh.init();
	gameLoop();
}

void MainGame::initSystems() {

	SDL_Init(SDL_INIT_EVERYTHING);

	m_window = SDL_CreateWindow("Game Engine", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, m_screenWidth, m_screenHeight, SDL_WINDOW_OPENGL);

	SDL_GLContext GLContext = SDL_GL_CreateContext(m_window);

	GLenum initiliazeGlew = glewInit();

	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	glEnable(GL_DEPTH_TEST);

	glDepthFunc(GL_LESS);

	initShaders();
}
void MainGame::initShaders() {

	m_colorProgram.compileShaders("Shaders/colorShading.vert", "Shaders/colorShading.frag");
	m_colorProgram.linkShaders();
}

void MainGame::gameLoop() {
	while (m_gameState != GameState::EXIT) {
		processInput();
		drawGame();
	}
}

void MainGame::processInput() {

	SDL_Event evnt;
	while (SDL_PollEvent(&evnt)) {
		switch (evnt.type) {
		case SDL_QUIT:
			m_gameState = GameState::EXIT;
		}
	}
}

void MainGame::drawGame() {

	glClearDepth(1.0f);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	m_colorProgram.use();

	GLuint location = m_colorProgram.getUniformLocation("MVP");

	// Projection matrix : 45° Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
	glm::mat4 Projection = glm::perspective(glm::radians(45.0f), 4.0f / 3.0f, 0.1f, 100.0f);
	// Camera matrix
	glm::mat4 View = glm::lookAt(
		glm::vec3(4, 3, -3), // Camera is at (4,3,-3), in World Space
		glm::vec3(0, 0, 0), // and looks at the origin
		glm::vec3(0, 1, 0)  // Head is up (set to 0,-1,0 to look upside-down)
	);
	// Model matrix : an identity matrix (model will be at the origin)
	glm::mat4 Model = glm::mat4(1.0f);
	// Our ModelViewProjection : multiplication of our 3 matrices
	glm::mat4 MVP = Projection * View * Model; // Remember, matrix multiplication is the other way around

	glUniformMatrix4fv(location, 1, GL_FALSE, &MVP[0][0]);

	m_mesh.draw();

	m_colorProgram.unuse();

	SDL_GL_SwapWindow(m_window);
}