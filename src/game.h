#ifndef GAME_H
#define GAME_H

#include <SDL3/SDL.h>
#include <array>
#include <fstream>
#include <list>
#include <random>
#include <string>

class Texture;
class Rectangle;
class Bow;
class SceneObject;
class InfoBar;

class Game {
public:

	// Número de actualizaciones por segundo
	static constexpr int FRAME_RATE = 30;
	// Periodo entre actualizaciones en milisegundos
	static constexpr int FRAME_PERIOD = 1000 / FRAME_RATE;

	// Tamaño real de la ventana
	static constexpr int WINDOW_WIDTH = 800;
	static constexpr int WINDOW_HEIGHT = 600;

	enum TextureName {
		BALLOON = 0,
		ARROW,
		BOW,
		DIGITS,
		BUTTERFLY,
		NUM_TEXTURES
	};

	Game();
	~Game();

	// Obtiene una textura por su nombre
	Texture* getTexture(TextureName name) const;

	// Ejecuta el bucle principal del juego
	void run();

	// Crea una flecha. Es público para que Bow pueda llamarlo
	void createArrow(const Rectangle& bowRect);

	// Comprueba si hay algún objeto colocado en ese rectángulo
	bool checkCollision(const Rectangle& rect);

	// Aumenta los puntos del jugador según los puntos por globo explotado
	void increasePoints(const int balloons);

	// Disminuye el número de mariposas y, si es posible, el número de puntos
	void decreasePoints();

private:

	struct Color {
		int r;
		int g;
		int b;
	};

	// Rutas y título
	static constexpr const char* WINDOW_TITLE = "Bow & Arrow 1.0";
	static constexpr const char* IMAGE_ROOT = "../assets/images/";
	static constexpr const char* LEVEL_ROOT = "../assets/levels/";

	// Valores por defecto usados al cargar niveles
	static constexpr Color DEFAULT_BACKGROUND = {135, 135, 10};
	static constexpr int DEFAULT_ARROWS = 10;
	static constexpr int DEFAULT_BALDELAY = 2000;
	static constexpr int DEFAULT_BALPOINTS = 10;
	static constexpr float DEFAULT_BALSPEED = 0.2f;
	static constexpr int DEFAULT_GOAL = 0;
	static constexpr int DEFAULT_BUTTERFLIES = 5;
	
	// Límites usados al validar los valores de los niveles
	static constexpr int MIN_BACKGROUND = -1;
	static constexpr int MAX_BACKGROUND = 256;
	static constexpr int MIN_ARROWS = 0;
	static constexpr int MIN_BALDELAY = 0;
	static constexpr int MIN_BALPOINTS = -1;
	static constexpr float MIN_BALSPEED = 0.0f;
	static constexpr int MIN_GOAL = -1;
	static constexpr int MIN_BUTTERFLIES = 0;

	// Constantes de renderizado
	static constexpr Uint8 OPAQUE_ALPHA = 255;

	// Proporciones del arco
	static constexpr float BOW_HEIGHT_DIV = 6.3f;
	static constexpr float BOW_WIDTH_DIV = 8.0f;

	// Globos
	static constexpr int NUM_BALLOON_COLORS = 7;
	static constexpr float MIN_GENERATOR_BALSPEED = 0.75;
	static constexpr float MAX_GENERATOR_BALSPEED = 1.25;
	static constexpr float BALLOON_MIN_X_DIV = 3.0f;
	static constexpr float BALLOON_WIDTH_DIV = 6.25f;
	static constexpr float BALLOON_HEIGHT_DIV = 4.7f;
	static constexpr float BALLOONS_PER_DELAY = 1.0f;

	// Flechas
	static constexpr float ARROW_WIDTH_DIV = 8.0f;
	static constexpr float ARROW_HEIGHT_DIV = 40.0f;
	static constexpr float ARROW_OFFSET_DIV = 13.0f;
	static constexpr float ARROW_SPEED = 0.4f;

	//Butterflies
	static constexpr float BUTTERFLY_WIDTH_DIV = 10.0f;

	// Estado de la partida
	bool exit;
	Color backgroundColor;
	int arrows;
	int baldelay;
	int balPoints;
	float balSpeed;
	int goal;
	int numButterflies;
	int levelToLoad;
	int playerPoints;

	// Guardado de niveles
	int savedLevel;
	int savedPoints;
	int savedButterflies;

	// Recursos y objetos de la escena
	SDL_Window* window = nullptr;
	SDL_Renderer* renderer = nullptr;
	std::array<Texture*, NUM_TEXTURES> textures;
	Bow* bow = nullptr;
	std::list<SceneObject*> sceneObjects;
	SceneObject* lastArrow = nullptr;
	InfoBar* infoBar = nullptr;

	// Generador y temporizadores
	std::mt19937_64 generator;
	uint64_t LastBalloonTime;
	uint64_t nextBalloon;

	// Método usado para la carga de niveles
	std::ifstream getLevelFile(const std::string& FileName);

	// Liberación de recursos
	void clearSceneObjects();
	void freeResources();

	// Método que reinicia los temporizadores de los globos
	void resetBalloonTimer();

	// Método que actualiza la barra de información
	void refreshInfoBar();

	void addObject(SceneObject* obj);
	void createBow();
	void createBalloon();
	void createButterflies();
	void render() const;
	void update();
	void handleEvents();
	void loadGame(std::ifstream file, const std::string& fileName);
	void loadNextLevel();
	
	// Guardan y cargan partidas
	void saveGame();
	void loadSavedGame();
};

inline Texture*
Game::getTexture(TextureName name) const
{
	return textures[name];
}

#endif // GAME_H
