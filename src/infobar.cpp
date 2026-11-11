#include "infobar.h"
#include "texture.h"

// Implementación del método constructor de la clase InfoBar
InfoBar::InfoBar(Game* game, Texture* arrowTexture, Texture* digitsTexture, const int arrows, 
    const int playerPoints) 
    : GameObject(game)
    , arrowTexture(arrowTexture)
    , digitsTexture(digitsTexture)
    , arrows(arrows)
    , playerPoints(playerPoints)
{}

// Implementación del método render de la clase InfoBar
void InfoBar::render() const {
    if (arrowTexture != nullptr) {
        
        // Creamos un rectángulo arrowsRect que se usará para representar todas las flechas
        // de la InfoBar
        Rectangle arrowsRect({0, 0}, Game::WINDOW_WIDTH / SIZE_DIVISOR, Game::WINDOW_HEIGHT / SIZE_DIVISOR);
        for (int i = 0; i < arrows; ++i) {
            arrowTexture->render(arrowsRect.to_sdl());
            arrowsRect.move(Vector2D((float)(Game::WINDOW_WIDTH / SIZE_DIVISOR), 0.0f));
        }

        // Creamos un rectángulo digitsRect que se usará para representar la puntuación
        // del jugador
        Rectangle digitsRect({Game::WINDOW_WIDTH - Game::WINDOW_WIDTH / SIZE_DIVISOR, 0}, 
            Game::WINDOW_WIDTH / SIZE_DIVISOR, Game::WINDOW_HEIGHT / SIZE_DIVISOR);

        // En base a la textura de los dígitos y los puntos del jugador, representamos
        // la puntuación
        if (playerPoints == 0) {
            digitsTexture->renderFrame(digitsRect.to_sdl(), DIGIT_ROW, 0);
        }
        else {
            int aux = playerPoints;
            while (aux != 0) {
                int digit = aux % 10;
                digitsTexture->renderFrame(digitsRect.to_sdl(), DIGIT_ROW, digit);
                digitsRect.move(Vector2D(-(float)(Game::WINDOW_WIDTH / SIZE_DIVISOR), 0.0f));
                aux /= 10;
            }
        }
    }
}

// Implementación del método update de InfoBar
bool InfoBar::update(const float) {
    return true;
}

// Implementación de los métodos updateArrows y updatePlayerPoints
void InfoBar::updateArrows(const int newArrowsValue) {
    arrows = newArrowsValue;
}
void InfoBar::updatePlayerPoints(const int newPlayerPointsValue) {
    playerPoints = newPlayerPointsValue;
}
