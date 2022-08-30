#include <SFML/Graphics.hpp>
#include <imgui.h>
#include <imgui-SFML.h>
#include <board.h>
#include <internals.h>

void init() {
	
	board = new Board();
	window = new sf::RenderWindow();

	//BOARD

	board->initImgData("../../data/gfx/maps/map_03_60x44_cost.png");
	board->loadBoard();
	board->agentSpriteLoad();
	board->falseBoardImg.loadFromFile("../../data/gfx/maps/map_03_960x704_layout ABGS.png");
	board->boardTexture.setTexture(board->falseBoardImg, true);

	//WINDOW

	window->create(sf::VideoMode(board->boardWidth * 20, board->boardHeight * 16), "AI test", sf::Style::Titlebar | sf::Style::Close | sf::Style::Resize);
	
	//IMGUI

	ImGui::SFML::Init(*window);
}

void EventProcessing() {
	while (window->pollEvent(events)) {

		ImGui::SFML::ProcessEvent(events);

		// GUSTAVO: Check that this is a KeyPressed event.
		if (events.key.code == sf::Keyboard::Escape || events.type == sf::Event::Closed)
			window->close();
	}
}

void DrawImgui() {

	ImGui::SFML::Update(*window, imgui_clock.restart());

	ImGui::Begin("FPS CONTROLLER");

	ImGui::Text("updateFramesPerSecond:");
	// GUSTAVO: Using ints.
	ImGui::SliderInt("1", &updateFramesPerSecond, 1.0f, 144.0f);

	ImGui::Text("eventsFramesPerSecond:");
	ImGui::SliderInt("2", &eventsFramesPerSecond, 1.0f, 144.0f);

	ImGui::Text("fixedUpdateFramesPerSecond:");
	ImGui::SliderInt("3", &fixedUpdateFramesPerSecond, 1.0f, 144.0f);

	ImGui::Text("drawFramesPerSecond:");
	ImGui::SliderInt("4", &drawFramesPerSecond, 1.0f, 144.0f);

	if (ImGui::Button("random"))
	{
		for (int i = 0; i < kBoardMaxAgents; i++)
		{
			board->agentss[i].mType = kMoveType_RANDOM;
		}
	}

	ImGui::SameLine();

	if (ImGui::Button("patrol"))
	{
		for (int i = 0; i < kBoardMaxAgents; i++)
		{
			board->agentss[i].mType = kMoveType_PATROL;
		}
	}

	ImGui::SameLine();

	if (ImGui::Button("pacman"))
	{
		for (int i = 0; i < kBoardMaxAgents; i++)
		{
			board->agentss[i].mType = kMoveType_PACMAN;
		}
	}

	// GUSTAVO: X and Y are swapped.
	if (ImGui::SliderInt("target X", &x, 0, board->boardHeight))
	{
		for (int i = 0; i < kBoardMaxAgents; i++)
		{
			board->agentss[i].targetx = x;
		}
	}

	if (ImGui::SliderInt("target Y", &y, 0, board->boardWidth))
	{
		for (int i = 0; i < kBoardMaxAgents; i++)
		{
			board->agentss[i].targety = y;
		}
	}

	// GUSTAVO: Individual agents must be able to change their behavior.

	ImGui::End();

}

void Update() {

}

void FixedUpdate() {

	board->movementType();

}

void Draw() {

	window->clear();

	window->draw(board->boardTexture);
	board->drawBoard(window);
	board->drawAgents(window);

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