#include "butterfly.h"

Butterfly::Butterfly(Game* game, const Rectangle& rect, Texture* texture,
	const Vector2D<>& speed)
	: MobileObject(game, rect, texture, speed)
	, numberOfButterflies(0)
	, animationFrame(0)
{
}

bool Butterfly::hit(const Rectangle& obj) {
	if (!SceneObject::hit(obj))
		return false;
	--numberOfButterflies;
	return true;
}

void Butterfly::ChangeDirection() {
	speed = Vector2D<>(-speed.getX(), -speed.getY());
}

void Butterfly::RandomizeDirection() {
	if(  > DIRECTION_CHANGE_TIME)
	speed = Vector2D<>(rand() % 200 - 100, rand() % 200 - 100);
}

void Butterfly::render() const {
	if (texture != nullptr) {
		texture->renderFrame(rect.to_sdl(), 0, animationFrame);
	}
}

bool Butterfly::update(const float delta) {
	if (MobileObject::update(delta)) {
		++animationFrame;
		if (animationFrame == FLIGHT_FRAMES) {
			animationFrame = 0;
		}
		return true;
	}
	return false;
}
