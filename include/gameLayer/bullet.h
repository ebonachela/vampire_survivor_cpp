#pragma once
#include <gl2d/gl2d.h>

constexpr float bulletSize = 15.f;
constexpr float bulletSpeed = 1000.f;

struct Bullet {
	glm::vec2 position = {0, 0};
	glm::vec2 direction = {1, 0};

	void render(gl2d::Renderer2D& renderer, gl2d::Texture& texture, float deltaTime, Bullet& bullet);
};