#ifndef __INTERNALS_H__
#define __INTERNALS_H__ 1

#include "common_def.h"
#include <SFML/Graphics.hpp>
#include "map.h"

sf::RenderWindow* window;

sf::Clock imgui_clock;

sf::Clock updateClock;
sf::Clock eventsClock;
sf::Clock drawClock;
sf::Clock fixedUpdateClock;

sf::Event event_;

Map map_;

int updateFramesPerSecond = 60.0f;
int eventsFramesPerSecond = 60.0f;
int fixedUpdateFramesPerSecond = 60.0f;
int drawFramesPerSecond = 60.0f;

int x = 0;
int y = 0;


#endif