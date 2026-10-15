#ifndef ARROW_H
#define ARROW_H

#include "rectangle.h"
#include "vector2D.h"
#include "texture.h"

class Game;  // Declaración anticipada de la clase Game

// Clase que representa una flecha en el juego
class Arrow {
	private:
		Game* game;
		Texture* texture;
		Rectangle rect;
		Vector2D<> speed;

	public:
		// Constructor que inicializa la flecha con su posición, velocidad y textura
		Arrow(Game* _game, Texture* texture, const Point2D<>& position, const Vector2D<>& speed);
		// Renderiza la flecha en la pantalla
		void render() const;
		// Actualiza la posición de la flecha y devuelve true si sigue activa, false si ha salido de la pantalla
		bool update(float deltaTime);
};

#endif // ARROW_H
