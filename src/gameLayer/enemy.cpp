#include <enemy.h>

static void enemyFollowMainPlayer(float deltaTime, glm::vec2 shipPos, Enemy& enemyData) {
	glm::vec2 dir = shipPos - enemyData.position;

	if (glm::length(dir) == 0.f) {
		dir = { 1, 0 };
	}
	else {
		dir = glm::normalize(dir);
	}

	if (dir.x != 0 || dir.y != 0) {
		dir *= deltaTime * 100;
		enemyData.position += dir;
	}
}

static float getMainPlayerAngle(glm::vec2 shipPos, Enemy& enemyData, gl2d::Renderer2D& renderer) {
	glm::vec2 dir = shipPos - enemyData.position;

	if (glm::length(dir) == 0.f) {
		dir = { 1, 0 };
	}
	else {
		dir = glm::normalize(dir);
	}

	return atan2(dir.y, -dir.x);
}

void Enemy::render(gl2d::Renderer2D& renderer, gl2d::Texture& texture, Enemy& enemyData, float deltaTime, glm::vec2 shipPos) {
	float mainPlayerAngle = getMainPlayerAngle(shipPos, enemyData, renderer);
	enemyFollowMainPlayer(deltaTime, shipPos, enemyData);
	renderer.renderRectangle({ enemyData.position - glm::vec2(enemyShipSize / 2, enemyShipSize / 2), enemyShipSize, enemyShipSize }, texture, Colors_White, {}, glm::degrees(mainPlayerAngle) + 90.f);
}