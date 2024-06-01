#include <bullet.h>
#include "imgui.h"

void Bullet::render(gl2d::Renderer2D& renderer, gl2d::Texture& texture, float deltaTime, Bullet& bullet)
{
	bullet.position += bullet.direction * deltaTime * bulletSpeed;
	renderer.renderRectangle({ bullet.position, bulletSize, bulletSize }, texture);
}
