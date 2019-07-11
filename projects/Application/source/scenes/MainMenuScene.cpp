#include "scenes/MainMenuScene.h"
#include "cameras/RotateCamera.h"

void MainMenuScene::onCreate(const DrawableFactory &drawableFactory)
{
	m_totalTime = 0;

	m_textRenderer.init();
	m_textRenderer.loadFont("resources/fonts/guardians.ttf");
	
	m_camera = std::make_unique<RotateCamera>(glm::vec3(1.f,-0.5f,-1.25f),6.f,10.f,45.f,true);
}

Scene * MainMenuScene::run(const DrawableFactory &drawableFactory, const Input &input, float elapsedTime)
{
	m_totalTime += elapsedTime * 0.5f;

	if (input.Keys[KEY_ESC])
		m_wantToPop = true;

	m_textRenderer.drawText(Text("Hello", glm::vec3(-0.5,0, -1), false, false, glm::vec4(1, std::cos(m_totalTime)* 0.5f  + 0.5f, 0, 1)));
	m_textRenderer.drawText(Text("World", glm::vec3(-1, -1, -1.5), false, false, glm::vec4(0, std::sin(m_totalTime)*0.5f + 0.5f,1, 1)));
	
	m_camera->handleInput(input, elapsedTime);

	//m_textRenderer.drawText(Text("h", glm::vec3(-0.5f, 0, -1), glm::vec4(1, 0, 0, 1)));

	m_textRenderer.render(*m_camera);

	return nullptr;
}