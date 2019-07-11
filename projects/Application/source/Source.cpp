#include "Game.h"
//#include "scenes/GameScene.h"
//#include "scenes/MainMenuScene.h"
//#include "scenes/ClickableTestScene.h"
//#include "scenes/HUDTestScene.h"
//#include "scenes/ShopTestScene.h"
#include "scenes/PlaygroundScene.h"
//#include "scenes/TSPScene.h"
//#include "scenes/ListOfPlanetsScene.h"

#include "MeshLoader.h"
#include "DrawableFactory.h"
#include "Shader.h"

void objectsToLoad(DrawableFactory &factory)
{
	GLuint program = glCreateProgram();
	Shader shader;
	shader.addShader(GL_VERTEX_SHADER, "resources/shaders/shader.vert");
	shader.addShader(GL_FRAGMENT_SHADER, "resources/shaders/shader.frag");
	shader.linkProgram(program);

	GLuint hudProgram = glCreateProgram();
	Shader hud;
	hud.addShader(GL_VERTEX_SHADER, "resources/shaders/hud.vert");
	hud.addShader(GL_FRAGMENT_SHADER, "resources/shaders/hud.frag");
	hud.linkProgram(hudProgram);

	GLuint cursorProgram = glCreateProgram();
	Shader cursor;
	cursor.addShader(GL_VERTEX_SHADER, "resources/shaders/cursor.vert");
	cursor.addShader(GL_FRAGMENT_SHADER, "resources/shaders/cursor.frag");
	cursor.linkProgram(cursorProgram);

	GLuint soiProgram= glCreateProgram();
	Shader soi;
	soi.addShader(GL_VERTEX_SHADER, "resources/shaders/soi.vert");
	soi.addShader(GL_FRAGMENT_SHADER, "resources/shaders/soi.frag");
	soi.linkProgram(soiProgram);

	factory.addDrawable(objLoader, "dv",		"resources/models/dv.obj",		"resources/textures/dv.png",		program);
	factory.addDrawable(objLoader, "sv",		"resources/models/sv.obj",		"resources/textures/sv.png",		program);
	factory.addDrawable(objLoader, "fv",		"resources/models/fv.obj",		"resources/textures/fv.png",		program);
	factory.addDrawable(objLoader, "cv",		"resources/models/cv.obj",		"resources/textures/cv.jpg",		program);
	factory.addDrawable(objLoader, "planet",	"resources/models/planet.obj",	"resources/textures/planet.jpg",	program);
	factory.addDrawable(objLoader, "soi",		"resources/models/planet.obj",	nullptr,							soiProgram);

	factory.addDrawable(objLoader, "hud",		"resources/models/plane.obj", nullptr,							hudProgram);
	factory.addDrawable(objLoader, "skybox",	"resources/models/plane.obj", nullptr,							hudProgram);
	factory.addDrawable(objLoader, "map",		"resources/models/plane.obj", nullptr,							hudProgram);
	factory.addDrawable(objLoader, "cursor",	"resources/models/plane.obj", "resources/textures/cursor.png",	cursorProgram);
}

int main()
{
	Game game;

	if(game.init(1024, 768, new PlaygroundScene, objectsToLoad))
		printf("Game initialized successfully\n");
	game.run();

	return 0;
}