#define GLM_ENABLE_EXPERIMENTAL
#include "gameLayer.h"
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include "platformInput.h"
#include "imgui.h"
#include <iostream>
#include <sstream>
#include "imfilebrowser.h"
#include <gl2d/gl2d.h>
#include <platformTools.h>
#include <enemy.h>
#include <bullet.h>
#include <vector>

struct ShipData {
	glm::vec2 pos = { 100, 100 };
	bool canMove = true;
	float angle = 0.f;
};

ShipData shipData;
ShipData enemyData;
gl2d::Renderer2D renderer;
gl2d::Texture spaceShipTexture;
gl2d::Texture enemyTexture;
gl2d::Texture backgroundTexture;
gl2d::Texture bulletTexture;

std::vector<Bullet> bullets;
std::vector<Enemy> enemies;

float spawnEnemyTimerSecconds = 0;

int w = 0; 
int h = 0;

bool initGame()
{
	//initializing stuff for the renderer
	gl2d::init();
	renderer.create();

	spaceShipTexture.loadFromFile(RESOURCES_PATH "spaceShip/ships/green.png", true);
	enemyTexture.loadFromFile(RESOURCES_PATH "spaceShip/ships/Blue.png", true);
	backgroundTexture.loadFromFile(RESOURCES_PATH "space/Black Hole1.png", true);
	bulletTexture.loadFromFile(RESOURCES_PATH "spaceShip/ships/green.png", true);

	for (int i = 0; i < 5; i++) {
		Enemy e1;
		int randX = rand() % (1280 - 0 + 1) + 0;
		int randY = rand() % (960 - 0 + 1) + 0;
		e1.position = glm::vec2(randX, randY);

		enemies.push_back(e1);
	}

	
	return true;
}

void init() {
	w = platform::getFrameBufferSizeX(); //window w
	h = platform::getFrameBufferSizeY(); //window h

	glViewport(0, 0, w, h);
	glClear(GL_COLOR_BUFFER_BIT); //clear screen

	renderer.updateWindowMetrics(w, h);
}

static void handleMovement(float deltaTime) {
	if (shipData.canMove == false) {
		return;
	}

	glm::vec2 direction = {};

	if (platform::isButtonHeld(platform::Button::W)) {
		direction.y = -1;
	}

	if (platform::isButtonHeld(platform::Button::A)) {
		direction.x = -1;
	}

	if (platform::isButtonHeld(platform::Button::D)) {
		direction.x = 1;
	}

	if (platform::isButtonHeld(platform::Button::S)) {
		direction.y = 1;
	}

	if (direction.x != 0 || direction.y != 0) {
		direction = glm::normalize(direction);
		direction *= deltaTime * 200;
		shipData.pos += direction;
	}
}

float getMouseAngle() {
	glm::vec2 mousePos = platform::getRelMousePosition();
	glm::vec2 screenCenter(w / 2.f, h / 2.f);
	glm::vec2 mouseDir = mousePos - screenCenter;

	if (glm::length(mouseDir) == 0.f) {
		mouseDir = { 1, 0 };
	}
	else {
		mouseDir = glm::normalize(mouseDir);
	}

	return atan2(mouseDir.y, -mouseDir.x);
}

bool gameLogic(float deltaTime)
{
	init();
	handleMovement(deltaTime);
	renderer.renderRectangle({ 0, 0, 1000, 1000 }, backgroundTexture);

	if (shipData.canMove) {
		shipData.angle = getMouseAngle();
	}
	float shipSize = 50.f;

	renderer.currentCamera.follow(shipData.pos, deltaTime * 200, 0, 200, w, h);

	renderer.renderRectangle({shipData.pos - glm::vec2(shipSize / 2, shipSize / 2), shipSize, shipSize}, spaceShipTexture, Colors_White, {}, glm::degrees(shipData.angle) + 90.f);
	
	if (platform::isLMousePressed() && shipData.canMove) {
		glm::vec2 mousePos = platform::getRelMousePosition();
		glm::vec2 screenCenter(w / 2.f, h / 2.f);
		glm::vec2 mouseDir = mousePos - screenCenter;

		if (glm::length(mouseDir) == 0.f) {
			mouseDir = { 1, 0 };
		}
		else {
			mouseDir = glm::normalize(mouseDir);
		}

		Bullet b;
		b.position = shipData.pos;
		b.direction = mouseDir;
		bullets.push_back(b);
	}

	for (int i = 0; i < bullets.size(); i++) {
		bullets[i].render(renderer, bulletTexture, deltaTime, bullets[i]);

		if (abs(glm::length(shipData.pos - bullets[i].position)) > 1000) {
			bullets.erase(bullets.begin() + i);
			i--;
			continue;
		}

		for (int j = 0; j < enemies.size(); j++) {
			Enemy& enemy = enemies[j];

			if (abs(glm::length(enemy.position - bullets[i].position)) <= 50) {
				enemy.isAlive = false;
				bullets.erase(bullets.begin() + i);
				enemies.erase(enemies.begin() + j);
				i--;
				j--;
				continue;
			}
		}

	}

	for (int i = 0; i < enemies.size(); i++) {
		Enemy& enemy = enemies[i];

		if (glm::length(shipData.pos - enemy.position) <= 50.f) {
			shipData.canMove = false;
			gl2d::Font f(RESOURCES_PATH "roboto_black.ttf");
			renderer.renderText({ shipData.pos.x, shipData.pos.y - 100 }, "Fim do Jogo", f, Colors_White);
		}

		if (!shipData.canMove) {
			continue;
		}

		enemy.render(renderer, enemyTexture, enemy, deltaTime, shipData.pos);
	}

	spawnEnemyTimerSecconds -= deltaTime;

	if (spawnEnemyTimerSecconds < 0 && shipData.canMove)
	{
		spawnEnemyTimerSecconds = 0.5;

		Enemy e1;
		int randX = rand() % (1280 - 0 + 1) + 0;
		int randY = rand() % (960 - 0 + 1) + 0;
		e1.position = glm::vec2(randX, randY);

		enemies.push_back(e1);
	}

	ImGui::Begin("debug");
	ImGui::Text("Bullets: %d", bullets.size());
	ImGui::Text("Enemies: %d", enemies.size());
	ImGui::End();

	renderer.flush();

	return true;
}

// This function might not be be called if the program is forced closed
void closeGame()
{



}
