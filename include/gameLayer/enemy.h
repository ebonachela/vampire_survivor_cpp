#pragma once
#include <gl2d/gl2d.h>

constexpr float enemyShipSize = 50.f;

struct Enemy {
	glm::vec2 position = {};
	float speed = 150.f;
	bool isAlive = true;

	void render(gl2d::Renderer2D& renderer, gl2d::Texture& texture, Enemy& enemyData, float deltaTime, glm::vec2 shipPos);
};