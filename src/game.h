// Dependencias circulares
// Meter toda la gestion de exit en update
// Crear LISTA de SceneObjects
// Ordenar las propiedades de Game en base al momento en el que se usan
// Comprobar si hay variables que podría quitar
// Correo Rubén
// Preguntar por prácticas de #include's
// Revisar soltar teclas en handleEvents

#ifndef GAME_H
#define GAME_H

#include <SDL3/SDL.h>
#include <array>
#include <list>
#include <istream>
#include <random>
#include <fstream>

// Declaraciones anticipadas
class Texture;
class Arrow;
class Bow;
class SceneObject;

// Clase principal del juego.
class Game {
	public:

		// Número de actualizaciones por segundo
		static constexpr int FRAME_RATE = 30;

		// Periodo entre actualizaciones en milisegundos
		static constexpr int FRAME_PERIOD = 1000 / FRAME_RATE;
		
		// Tamaño real de la ventana
		static constexpr int WINDOW_WIDTH = 800;
		static constexpr int WINDOW_HEIGHT = 600;

		enum TextureName
		{
			BALLOON = 0,
			ARROW,
			BOW,
			NUM_TEXTURES
		};
		
	private:
		
		// Variable booleana para controlar la finalización del juego
		bool exit;

		// Struct para representar colores en formato RGB
		struct Color { int r, g, b; };
		
		// Color de fondo del nivel 
		Color backgroundColor;
		
		// Número de flechas
		int arrows;

		// Tiempo de espera en milisegundos entre la generación de un globo y su 
		// siguiente
		int baldelay;
		
		// Variables para almacenar la meta de puntos y los puntos por globo explotado 
		int balPoints;
		int goal;
		
		// Rutas y título
		static constexpr const char* WINDOW_TITLE = "Bow & Arrow 1.0";
		static constexpr const char* IMAGE_ROOT = "../assets/images/";
		static constexpr const char* LEVEL_ROOT = "../assets/levels/";
		
		SDL_Window* window;
		SDL_Renderer* renderer;
		std::array<Texture*, NUM_TEXTURES> textures;
		
		// Variable entera que representa el nivel a cargar
		int levelToLoad;
		
		// Variable para almacenar los puntos actuales del jugador
		int playerPoints = 0;
		
		// Constantes usadas en la carga de niveles
		static constexpr Color DEFAULT_BACKGROUND = {0, 0, 0};
		static constexpr int DEFAULT_ARROWS = 10;
		static constexpr int DEFAULT_BALDELAY = 750;
		static constexpr int DEFAULT_BALPOINTS = 10;
		static constexpr int DEFAULT_GOAL = 100;
		static constexpr int MIN_BACKGROUND = 0;
		static constexpr int MIN_ARROWS = 1;
		static constexpr int MIN_BALDELAY = 1;
		static constexpr int MIN_BALPOINTS = 1;
		static constexpr int MIN_GOAL = 1;
		
		// Método LoadGame de la clase Game que permite cargar un nivel
		void LoadGame(std::ifstream& file);
		
		// Atributo propio que guarda la referencia al arco (Bow)
		Bow* bow;
		
		// Arco
		static constexpr float BOW_HEIGHT_DIV = 6.3f;
		static constexpr float BOW_WIDTH_DIV = 8.0f;

		// Lista de punteros a SceneObject que contiene las referencias a todos
		// los objetos del juego
		std::list<SceneObject*> sceneObjects;
		
		// Método para añadir la referencia de un objeto de la escena a la lista
		// sceneObjects
		void addObject(SceneObject* obj);

		// Globos
		// filas de balloons.png
		static constexpr int NUM_BALLOON_COLORS = 7;           
		static constexpr float MIN_BALLOON_SPEED = -0.05f;
		static constexpr float MAX_BALLOON_SPEED = -0.02f;

		// posX mínima = WINDOW_WIDTH / 3
		static constexpr float BALLOON_MIN_X_DIV = 3.0f;       
		static constexpr float BALLOON_WIDTH_DIV = 6.25f;
		static constexpr float BALLOON_HEIGHT_DIV = 4.7f;

		// tasa = BALLOONS_PER_DELAY / baldelay
		static constexpr float BALLOONS_PER_DELAY = 1.0f;      
		
		// Render
		static constexpr Uint8 OPAQUE_ALPHA = 255;
		
		// Métodos responsables de la creación de globos y flechas
		void createBalloon();
		
		// Flecha
		static constexpr float ARROW_WIDTH_DIV = 5.95f;
		static constexpr float ARROW_HEIGHT_DIV = 21.95f;
		static constexpr float ARROW_OFFSET_DIV = 6.3f;
		static constexpr float ARROW_SPEED = 0.1f;

		/*
		// InfoBar (mitad del tamaño de la flecha)
		static constexpr float INFOBAR_SCALE = 2.0f;
		*/
	
		void render() const;
		void update();
		void handleEvents();
		
		// Método que lee de un archivo de entrada los datos de baldelay, arrows, etc...
		void loadNextLevel();

		// Generador de números pseudoaleatorios
		std::mt19937_64 generator;

		// Variables para gestionar el tiempo
		uint64_t LastBalloonTime;
		uint64_t nextBalloon;


	public:
	
	Game();
	~Game();
	
		// Obtiene una textura por su nombre
		Texture* getTexture(TextureName name) const;

		// Ejecuta el bucle principal del juego
		void run();
		
		// Método para la creación de flechas
		// Es público para que así bow pueda llamarlo
		void createArrow(const Rectangle& bowRect);

		// Comprueba si hay algún objeto colocado en ese rectángulo
		// bool checkCollision(const Rectangle& rect);
		
		// Método que aumenta el total de puntos del jugador respecto a los puntos que suelta 
		// cada globo
		void increasePoints();
};

inline Texture*
Game::getTexture(TextureName name) const
{
	return textures[name];
}

#endif // GAME_H
