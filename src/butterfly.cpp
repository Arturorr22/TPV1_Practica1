#include "butterfly.h"
#include "game.h"
#include <cmath>
#include "texture.h"
#include <format>

// Implementación del constructor de la clase Butterfly
Butterfly::Butterfly(Game* game, const Rectangle& rect, Texture* texture, 
	const Vector2D<>& speed, const float timeSinceDirectionChange, 
	const int animationFrame, const bool dead, std::mt19937_64& generator)
	: MobileObject(game, rect, texture, speed)
	, timeSinceDirectionChange(timeSinceDirectionChange)
	, animationFrame(animationFrame)
	, dead(dead)
	, generator(generator)

	// timeSinceDirectionChange(0.0f)
	// , animationFrame(0)
	// , dead(false)
{
	if (timeSinceDirectionChange == 0.0f) randomizeDirection();
}

// Implementación del método hit de la clase Butterfly
bool Butterfly::hit(const Rectangle& obj) {

	// Una mariposa muerta o que no toca el rectángulo no recibe el impacto
	if (dead || !SceneObject::hit(obj))
		return false;

	// La mariposa muere y su cadáver pasa a caer a velocidad constante
	game->decreasePoints();
	dead = true;
	speed = Vector2D<>(0.0f, FALL_SPEED);
	return true;
}

// Implementación del método randomizeDirection de la clase Butterfly
void Butterfly::randomizeDirection() {

	// Sorteamos un ángulo y lo convertimos en velocidad con el módulo fijo
	const float angle = std::uniform_real_distribution<float>(0.0f, TWO_PI)(generator);
	speed = Vector2D<>(FLIGHT_SPEED * std::cos(angle), FLIGHT_SPEED * std::sin(angle));
}

// Implementación del método bounceOffBorders de la clase Butterfly
void Butterfly::bounceOffBorders() {
	const Point2D<> corner = rect.getTopLeftCorner();
	float speedX = speed.getX();
	float speedY = speed.getY();

	// Solo invertimos la componente que apunta hacia fuera de la ventana
	if ((corner.getX() < 0 && speedX < 0) ||
		(corner.getX() + rect.getWidth() > Game::WINDOW_WIDTH && speedX > 0))
		speedX = -speedX;

	if ((corner.getY() < 0 && speedY < 0) ||
		(corner.getY() + rect.getHeight() > Game::WINDOW_HEIGHT && speedY > 0))
		speedY = -speedY;

	speed = Vector2D<>(speedX, speedY);
}

// Implementación del método render de la clase Butterfly
void Butterfly::render() const {
	if (texture != nullptr) {
		texture->renderFrame(rect.to_sdl(), 0, animationFrame);
	}
}

// Implementación del método update de la clase Butterfly
bool Butterfly::update(const float delta) {

	// El cadáver cae hasta salir por el borde inferior de la ventana
	if (dead) {
		rect.move(speed * delta);
		return rect.getTopLeftCorner().getY() < Game::WINDOW_HEIGHT;
	}

	// Cada cierto tiempo, la mariposa viva cambia de dirección al azar
	timeSinceDirectionChange += delta;
	if (timeSinceDirectionChange > DIRECTION_CHANGE_TIME) {
		randomizeDirection();
		timeSinceDirectionChange = 0.0f;
	}

	// Se mueve, rebota si intenta salirse de la ventana y avanza su animación
	rect.move(speed * delta);
	bounceOffBorders();
	animationFrame = (animationFrame + 1) % FLIGHT_FRAMES;
	return true;
}

// Implementación del método saveObject de la clase Balloon
void Butterfly::saveObject(std::ostream& savedFile) const {

    // Escribimos una b minúscula para indicar que se trata de un globo
    savedFile << "f ";

    // Llamamos al método saveObject de MobileObject
    MobileObject::saveObject(savedFile);

    // Añadimos los atributos propios del globo
    savedFile << std::format("{} {} {}", timeSinceDirectionChange, 
		animationFrame, dead ? 1 : 0) << "\n";
}

// Implementación del método loadObject de la clase Balloon
Butterfly* Butterfly::loadObject(std::istream& savedFile, Game* game, std::mt19937_64& generator) {

    float x, y, w, h, sX, sY, tSDC;
    savedFile >> x >> y >> w >> h >> sX >> sY >> tSDC;
	int aF, d;
    savedFile >> aF >> d;

    return new Butterfly(game, Rectangle({x, y}, w, h), game->getTexture(Game::BUTTERFLY), 
        Vector2D(sX, sY), tSDC, aF, d == 1 ? true : false, generator);
}