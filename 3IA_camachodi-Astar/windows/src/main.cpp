#include <SFML/Graphics.hpp>
#include <imgui.h>
#include <imgui-SFML.h>
#include <internals.h>

void init() {

	window = new sf::RenderWindow();

	//MAP

	map_.InitElementsData();
	map_.LoadElementsData();

	//WINDOW

	window->create(sf::VideoMode(1200, 1000), "PATHFINDING", sf::Style::Titlebar | sf::Style::Close | sf::Style::Resize);
	//window->setKeyRepeatEnabled(false);
	

	//IMGUI

	ImGui::SFML::Init(*window);
}

void EventProcessing()
{

	while (window->pollEvent(event_)) {

		ImGui::SFML::ProcessEvent(event_);

		if (event_.type == sf::Event::KeyPressed)
		{

			if (event_.key.code == sf::Keyboard::Escape)
			{

				window->close();

			}

		}

		if (event_.type == sf::Event::Closed)
		{

			window->close();

		}

	}

}

void DrawImgui() {

	ImGui::SFML::Update(*window, imgui_clock.restart());

	ImGui::Begin("FPS CONTROLLER");

	ImGui::Text("updateFramesPerSecond:");

	ImGui::SliderInt("1", &updateFramesPerSecond, 1, 144);

	ImGui::Text("eventsFramesPerSecond:");
	ImGui::SliderInt("2", &eventsFramesPerSecond, 1, 144);

	ImGui::Text("fixedUpdateFramesPerSecond:");
	ImGui::SliderInt("3", &fixedUpdateFramesPerSecond, 1, 144);

	ImGui::Text("drawFramesPerSecond:");
	ImGui::SliderInt("4", &drawFramesPerSecond, 1, 144);

	ImGui::NewLine();

	if (ImGui::Button("Replace Agent"))
	{

		if (map_.goalChangeDone_)
		{

			map_.agentChangeDone_ = false;

		}

	}

	ImGui::NewLine();

	if (ImGui::Button("Replace Goal"))
	{
		if (map_.agentChangeDone_)
		{

			map_.goalChangeDone_ = false;

		}
	}

	ImGui::NewLine();

	if (ImGui::Button("Start Seek"))
	{
		if (map_.seekDone_)
		{

			map_.seekDone_ = false;
			map_.StartSeek();

		}
	}

	ImGui::SameLine();

	ImGui::End();

}

void Update() {

	map_.ChangeTile(window);
	map_.ReplaceAgent(window);
	map_.ReplaceGoal(window);

}

void FixedUpdate() {

	//AQUI VA EL MOVIMIENTO

}

void Draw() {

	window->clear();

	//AQUI SE DIBUJAN TODOS LOS TILES
	map_.DrawElements(window);

	DrawImgui();

	ImGui::SFML::Render(*window);

	window->display();

}

void loop() {

	while (window->isOpen()) {

		if (updateClock.getElapsedTime().asSeconds() >= (1.0f / updateFramesPerSecond)) {

			Update();
			updateClock.restart();

		}

		if (eventsClock.getElapsedTime().asSeconds() >= (1.0f / eventsFramesPerSecond)) {

			EventProcessing();
			eventsClock.restart();

		}

		if (fixedUpdateClock.getElapsedTime().asSeconds() >= (1.0f / fixedUpdateFramesPerSecond)) {

			FixedUpdate();
			fixedUpdateClock.restart();
		}

		if (drawClock.getElapsedTime().asSeconds() >= (1.0f / drawFramesPerSecond)) {

			Draw();
			drawClock.restart();

		}
	}

}

void shutDown() {

	ImGui::SFML::Shutdown();

}

int main() {

	init();
	loop();
	shutDown();

	return 0;
}