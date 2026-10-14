#include "arrow.h"
#include "game.h"

// Crea un objeto Arrow con la posición, velocidad y textura especificadas
Arrow::Arrow(Game* _game, Texture* texture, const Point2D<>& position, const Vector2D<>& speed) : game(_game), texture(texture), rect(position, texture->getFrameWidth(), texture->getFrameHeight()), speed(speed) {}

// Implementa la lógica de renderizado de la flecha utilizando la textura y el rectángulo
void Arrow::render() const {
	texture->render(rect.to_sdl());
}

// Implementa la lógica de actualización de la flecha
bool Arrow::update(float deltaTime) {
	// Actualiza la posición de la flecha según su velocidad y el tiempo transcurrido
	Vector2D<> displacement = speed * deltaTime;
	rect.move(displacement);

	// Comprueba si la flecha ha salido de la pantalla mirando si la esquina superior 
	// izquierda del rectángulo está fuera de los límites de la ventana
	// || rect.getTopLeftCorner().getY() > Game::WINDOW_HEIGHT Me lo dejo para comprobar si no funciona
	if (rect.getTopLeftCorner().getX() > Game::WINDOW_WIDTH) {
		return false;
	}
	return true;
}
