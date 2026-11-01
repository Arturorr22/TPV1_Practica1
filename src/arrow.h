#ifndef ARROW_H
#define ARROW_H

#include "rectangle.h"
#include "vector2D.h"
#include "texture.h"

class Game;  // Declaración anticipada de la clase Game

// Clase que representa una flecha en el juego
class Arrow {
	private:
		Game* game; // Puntero a la instancia del juego
		Rectangle rect; // Rectángulo que define la posición y dimensiones de la flecha
		Vector2D<> speed; // Velocidad de la flecha
		Texture* texture; // Puntero a la textura de la flecha

		// Proporciones del rectángulo de la punta respecto al rectángulo de la flecha
		static constexpr float TIP_X_FACTOR = 2.0f / 3.0f;
		static constexpr float TIP_Y_FACTOR = 1.0f / 3.0f;
		static constexpr float TIP_WIDTH_FACTOR = 1.0f / 4.0f;
		static constexpr float TIP_HEIGHT_FACTOR = 1.0f / 2.0f;

	public:

		// Constructor que inicializa la flecha con el juego, el rectángulo de destino, la velocidad y la textura
		Arrow(Game* game, const Rectangle destRect, const Vector2D<>& speed, Texture* texture);
		
		// Renderiza la flecha en la pantalla
		void render() const;
		
		// Actualiza la posición de la flecha y devuelve true si sigue activa, false si ha salido de la pantalla
		bool update(const float delta); //La velocidad es en pixeles*milisegundos
};

#endif // ARROW_H
