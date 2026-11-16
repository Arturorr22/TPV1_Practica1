#include "game.h"
#include "texture.h"
#include "bow.h"
#include "infobar.h"
#include "butterfly.h"
#include "arrow.h"
#include "balloon.h"
#include "error.h"

#include <fstream>
#include <sstream>
#include <string>

using namespace std;

// Estructura para especificar las texturas que hay que
// cargar y el tamaño de su matriz de frames
struct TextureSpec {
	const char* name;
	int nrows = 1;
	int ncols = 1;
};

constexpr array<TextureSpec, Game::NUM_TEXTURES> textureList {
	TextureSpec{"balloons.png", 7, 6},
	{"arrow.png"},
	{"bow.png", 1, 2},
	{"digits.png", 1, 10},
	{"butterfly.png", 4, 10},
};

// Implementación del constructor de la clase Game
Game::Game()
	: exit(false)
{
	// Carga SDL y crea la ventana con su renderizador
	if (!SDL_Init(SDL_INIT_VIDEO))
		throw SDLError("SDL_Init");

	if (!SDL_CreateWindowAndRenderer(WINDOW_TITLE, WINDOW_WIDTH, WINDOW_HEIGHT, 0, &window, &renderer))
		throw SDLError("SDL_CreateWindowAndRenderer");

	// Carga las texturas al inicio
	try {
		textures.fill(nullptr);
		string imgRoot = IMAGE_ROOT;
		
		for (size_t i = 0; i < textures.size(); i++) {
			auto [name, nrows, ncols] = textureList[i];
			textures[i] = new Texture(renderer, (imgRoot + name).c_str(), nrows, ncols);
		}
		
		// Leemos el nivel 1
		levelToLoad = 1;
		const string levelFileName = string(LEVEL_ROOT) + to_string(levelToLoad) + ".txt";
		ifstream levelFile(levelFileName);
		if (!levelFile.is_open())
			throw FileNotFoundError(levelFileName);
		loadGame(levelFile, levelFileName);
		++levelToLoad;
	}
	catch (...) {
		
		// Liberamos los recursos creados antes del fallo
		for (std::size_t i = 0; i < textures.size(); ++i)
			delete textures[i];
		if (renderer != nullptr) SDL_DestroyRenderer(renderer);
		if (window != nullptr) SDL_DestroyWindow(window);
		SDL_Quit();
		
		// Relanza la excepción
		throw;
	}
	
	// Fijamos los puntos del jugador a 0
	playerPoints = 0;

	// Creamos el arco
	bow = new Bow(this, Rectangle({ 0, (float)(WINDOW_HEIGHT / 2 - WINDOW_HEIGHT / BOW_HEIGHT_DIV) },
		WINDOW_WIDTH / BOW_WIDTH_DIV, WINDOW_HEIGHT / BOW_HEIGHT_DIV), textures[BOW], arrows, false);

	// Añadimos el arco a la lista de SceneObjects
	addObject(bow);

	// Creamos la infoBar con sus respectivos datos
	infoBar = new InfoBar(this, textures[ARROW], textures[DIGITS], bow->getArrows(), playerPoints);

	// Configuramos la semilla del generador para la generación de globos
	generator.seed(std::random_device()());

	// Creamos las mariposas
	createButterflies();

	// Calculamos el tiempo que ha de pasar hasta que aparezca el primer globo
	nextBalloon = exponential_distribution<double>(BALLOONS_PER_DELAY / baldelay)(generator);

	// Almacenamos el tiempo actual al iniciar el programa en la variable LastBalloonTime
	LastBalloonTime = SDL_GetTicks();
}

// Implementación del destructor de la clase Game
Game::~Game() {
	freeResources();
}

// Libera los recursos reservados. Se puede llamar aunque la inicialización haya
// fallado a medias, porque comprueba cada recurso antes de liberarlo
void
Game::freeResources()
{
	// Destruimos todos los objetos de la escena
	auto it = sceneObjects.begin();
	while (it != sceneObjects.end()) {
		delete *it;
		it = sceneObjects.erase(it);
	}

	// Borramos la infoBar
	delete infoBar;
	infoBar = nullptr;

	// Destruimos las texturas
	for (int i = 0; i < textures.size(); ++i) {
		delete textures[i];
		textures[i] = nullptr;
	}

	// Destruimos el renderer, la ventana y cerramos SDL
	if (renderer != nullptr) SDL_DestroyRenderer(renderer);
	if (window != nullptr) SDL_DestroyWindow(window);
	renderer = nullptr;
	window = nullptr;
	SDL_Quit();
}

// Implementación del método run de la clase Game
void
Game::run() {
	while (!exit) {

		// Calculamos frameStart y despues frameEnd
		int frameStart = SDL_GetTicks();

		// Manejamos los posibles eventos
		handleEvents();

		// Actualizamos el estado de todos los objetos del juego
		update();

		// Renderizamos todos los objetos del juego
		render();

		// Usamos frameEnd y frameStart para lograr que el juego se ciña a
		// FRAME_PERIOD, que se corresponde con el tiempo entre frames
		int frameEnd = SDL_GetTicks();
		if (frameEnd - frameStart < FRAME_PERIOD) {
			SDL_Delay(FRAME_PERIOD - (frameEnd - frameStart));
		}
	}
}

// Implementación del método createArrow de la clase Game
void
Game::createArrow(const Rectangle& bowRect) {

	// Creamos una flecha y la añadimos a la lista de objetos de la escena
	// Antes de incorporarla a la lista, hacemos que el puntero lastArrow apunte a ella
	lastArrow = new Arrow(this, Rectangle({ 0,
		bowRect.getTopLeftCorner().getY() + (float)(WINDOW_HEIGHT / ARROW_OFFSET_DIV) },
		WINDOW_WIDTH / ARROW_WIDTH_DIV, WINDOW_HEIGHT / ARROW_HEIGHT_DIV),
		textures[ARROW], Vector2D(ARROW_SPEED, 0.0f), 0);
	sceneObjects.push_back(lastArrow);

	// Actualizamos el valor del número de flechas que maneja infoBar
	infoBar->updateArrows(bow->getArrows());
}

// Implementación del método checkCollision de la clase Game
bool
Game::checkCollision(const Rectangle& rect) {

	// Recorremos todos los objetos de la escena comprobando si se dan colisiones
	auto it = sceneObjects.begin();
	while (it != sceneObjects.end() && !(*it)->hit(rect)) {
		++it;
	}
	return it != sceneObjects.end();
}

// Implementación del método increasePoints de la clase Game
void
Game::increasePoints(const int balloons) {

	// Aumentamos el número de puntos del jugador y actualizamos el valor almacenado
	// en infoBar
	playerPoints += balPoints + (balPoints / 2) * balloons;
	infoBar->updatePlayerPoints(playerPoints);
}

// Implementación del método decreasePoints de la clase Game
void 
Game::decreasePoints() {

	--numButterflies;

	// Comprueba si se pueden restar puntos, y en ese caso los resta y actualiza el valor
	// almacenado en infoBar
	if (playerPoints >= balPoints) {
		playerPoints -= balPoints;
		infoBar->updatePlayerPoints(playerPoints);
	}
	
}

// Lee un entero del fichero y comprueba que no sea menor o igual que minValue
// o mayor o igual que maxValue, siempre y cuando se haya introducido un valor para
// este último argumento
// Si no es válido, lanza una excepción con el nombre de la palabra clave
float readValue(std::ifstream& file, const string& name, const int line, const int minValue, 
	const int maxValue = 0) {
	float value;
	if (!(file >> value) || value <= minValue || (maxValue != 0 && value >= maxValue))
		throw FileFormatError(name, line, "Valor incorrecto.");
	return value;
}

// Implementación del método LoadGame
void
Game::loadGame(std::ifstream& file, const std::string& fileName)
{
	// Asignamos unos valores por defecto a los atributos de Game por si el archivo
	// no los modifica
	backgroundColor = DEFAULT_BACKGROUND;
	arrows = DEFAULT_ARROWS;
	baldelay = DEFAULT_BALDELAY;
	balPoints = DEFAULT_BALPOINTS;
	balSpeed = DEFAULT_BALSPEED;
	goal = DEFAULT_GOAL;
	numButterflies = DEFAULT_BUTTERFLIES;

	// Leemos el archivo y modificamos los atributos de Game
	string keyword;
	int lineNumber = 0;
	while (file >> keyword) {

		// Leemos la primera palabra de cada línea e intentamos almacenar el valor
		// escrito después
		if (keyword == "bgcolor") {
			backgroundColor.r = readValue(file, fileName, lineNumber, MIN_BACKGROUND, MAX_BACKGROUND);
			backgroundColor.g = readValue(file, fileName, lineNumber, MIN_BACKGROUND, MAX_BACKGROUND);
			backgroundColor.b = readValue(file, fileName, lineNumber, MIN_BACKGROUND, MAX_BACKGROUND);
		}
		else if (keyword == "arrows") arrows = readValue(file, fileName, lineNumber, MIN_ARROWS);
		else if (keyword == "baldelay") baldelay = readValue(file, fileName, lineNumber, MIN_BALDELAY);
		else if (keyword == "balpoints") balPoints = readValue(file, fileName, lineNumber, MIN_BALPOINTS);
		else if (keyword == "balspeed") balSpeed = readValue(file, fileName, lineNumber, MIN_BALSPEED);
		else if (keyword == "goal") goal = readValue(file, fileName, lineNumber, MIN_GOAL);
		else if (keyword == "butterflies") numButterflies = readValue(file, fileName, lineNumber, MIN_BUTTERFLIES);
		else file.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		++lineNumber;
	}
}

// Implementación del método addObject de la clase Game
void
Game::addObject(SceneObject* obj) {
	if (obj != nullptr) {
		sceneObjects.push_back(obj);
	}
}

// Implementación del método createBalloon de la clase Game
void
Game::createBalloon() {

	// Calculamos, de manera pseudoaleatoria, la posición del borde inferior
	// en la que aparecerá el globo y su color
	int color = uniform_int_distribution<int>(0, NUM_BALLOON_COLORS - 1)(generator);
	float speedY = balSpeed * uniform_real_distribution<float>(MIN_GENERATOR_BALSPEED, 
		MAX_GENERATOR_BALSPEED)(generator);
	float posX = uniform_real_distribution<float>(WINDOW_WIDTH / BALLOON_MIN_X_DIV,
		WINDOW_WIDTH - WINDOW_WIDTH / BALLOON_WIDTH_DIV)(generator);

	// Creamos el globo y lo añadimos a sceneObjects
	sceneObjects.push_back(new Balloon(this, Rectangle({posX, WINDOW_HEIGHT}, 
		WINDOW_WIDTH / BALLOON_WIDTH_DIV, WINDOW_HEIGHT / BALLOON_HEIGHT_DIV), 
		textures[BALLOON], Vector2D<>(0, -speedY), color, false, 0));

	// Recalculamos el valor hasta el siguiente globo
	nextBalloon = exponential_distribution<double>(BALLOONS_PER_DELAY / baldelay)(generator);

	// Actualizamos el valor de LastBalloonTime
	LastBalloonTime = SDL_GetTicks();
}

// Implementación del método createButterflies de la clase Game
void
Game::createButterflies() {

	// Calculamos el tamaño, igual para todas: el alto sale de la proporción del frame
	float width = WINDOW_WIDTH / BUTTERFLY_WIDTH_DIV;
	float height = width / textures[BUTTERFLY]->getAspectRatio();

	for (int i = 0; i < numButterflies; ++i) {

		// Posición aleatoria con la mariposa completa dentro de la ventana
		float posX = uniform_real_distribution<float>(0.0f, WINDOW_WIDTH - width)(generator);
		float posY = uniform_real_distribution<float>(0.0f, WINDOW_HEIGHT - height)(generator);

		// La dirección inicial la elige la propia mariposa
		sceneObjects.push_back(new Butterfly(this, Rectangle({posX, posY}, width, height),
			textures[BUTTERFLY], Vector2D(), 0.0f, 0, false, generator));
	}
}

// Implementación del método render de la clase Game
void
Game::render() const {

	// Dibujamos el fondo con el color almacenado
	SDL_SetRenderDrawColor(renderer, backgroundColor.r, backgroundColor.g,
		backgroundColor.b, OPAQUE_ALPHA);

	SDL_RenderClear(renderer);

	// Renderizamos todos los objetos de la escena
	for (auto it = sceneObjects.begin(); it != sceneObjects.end(); ++it) {
		(*it)->render();
	}
	infoBar->render();

	SDL_RenderPresent(renderer);
}

// Implementación del método update de la clase Game
void
Game::update() {

	// Actualizamos todos los objetos de la escena borrando aquellos cuyo update indique
	// su eliminación
	auto it = sceneObjects.begin();
	while (it != sceneObjects.end()) {
		if ((*it)->update(FRAME_PERIOD)) ++it;
		else {

			// Comprobamos si se trata de la última flecha de la que disponía el
			// jugador
			if (bow->getArrows() == 0 && *it == lastArrow) exit = true;
			delete *it;
			it = sceneObjects.erase(it);
		}
	}

	// Comprobamos si ya ha pasado suficiente tiempo desde la generación del último globo
	if (SDL_GetTicks() - LastBalloonTime > nextBalloon) createBalloon();

	// Si el jugador acaba con todas las mariposas, pierde
	if (numButterflies == 0) {
		exit = true;
		cout << "Has perdido\n";
	}

	// Si el jugador supera la meta, cargamos el siguiente nivel
	if (goal != 0 && playerPoints >= goal) loadNextLevel();
}


// Implementación del método handleEvents de la clase Game
void
Game::handleEvents() {
	SDL_Event event;

	// Quit y las combinaciones CTRL + S y CTRL + O se manejan directamente
	// El resto se delega al arco
	while (SDL_PollEvent(&event)) {
		if (event.type == SDL_EVENT_QUIT)
			exit = true;
		else if (event.type == SDL_EVENT_KEY_DOWN || event.type == SDL_EVENT_KEY_UP) {
			if (event.type == SDL_EVENT_KEY_DOWN && (event.key.mod & SDL_KMOD_CTRL)) {
				if (event.key.key == SDLK_S) saveGame();
				else if (event.key.key == SDLK_O) loadSavedGame();
			}
			else { 
				bow->handleEvent(event); 
			}
		}
	}
}

// Implementación del método loadNextLevel de la clase Game
void
Game::loadNextLevel()
{
	const string levelFileName = string(LEVEL_ROOT) + to_string(levelToLoad) + ".txt";
	ifstream levelFile(levelFileName);
	if (!levelFile.is_open()) {
		cout << "Has ganado. Felicidades!\n";
		exit = true;
	}
	else {
		loadGame(levelFile, levelFileName);
		++levelToLoad;
	
		// Borramos todos los objetos de la escena antes de pasar al siguiente nivel
		// (exceptuando el arco)
		auto it = sceneObjects.begin();
		while (it != sceneObjects.end()) {
			delete *it;
			it = sceneObjects.erase(it);
		}
		lastArrow = nullptr;
	
		// Fijamos los puntos del jugador a 0
		playerPoints = 0;
	
		// Creamos el arco
		bow = new Bow(this, Rectangle({ 0, (float)(WINDOW_HEIGHT / 2 - WINDOW_HEIGHT / BOW_HEIGHT_DIV) },
			WINDOW_WIDTH / BOW_WIDTH_DIV, WINDOW_HEIGHT / BOW_HEIGHT_DIV), textures[BOW], arrows, false);
	
		// Añadimos el arco a la lista de SceneObjects
		addObject(bow);
	
		// Creamos las mariposas del siguiente nivel
		createButterflies();
	
		// Actualizamos la infoBar
		infoBar->updateArrows(bow->getArrows());
		infoBar->updatePlayerPoints(playerPoints);
	}
}

// Implementación del método saveGame de la clase Game
void
Game::saveGame() const {

	// Creamos el archivo de guardado
	ofstream savedFile("saved.txt");
	
	// Guardamos el nivel en el que se encuentra el jugador y los puntos que tiene
	savedFile << levelToLoad - 1 << '\n' << playerPoints << '\n' << numButterflies << "\n";

	// Guardamos cada objeto
	for (auto it = sceneObjects.begin(); it != sceneObjects.end(); ++it)
		(*it)->saveObject(savedFile);

	// Cerramos el archivo de escritura
	savedFile.close();
}

// Implementación del método loadSavedGame de la clase Game
void
Game::loadSavedGame() {

	ifstream savedFile("saved.txt");
	if (!savedFile.is_open())
		throw FileNotFoundError("saved.txt");

	// Leemos los valores de savedLevel y savedPoints
	if (!(savedFile >> savedLevel)) {
		throw FileFormatError("saved.txt", 1, "Nivel de guardado en formato inválido.");
	}
	if (!(savedFile >> savedPoints)) {
		throw FileFormatError("saved.txt", 2, "Puntos del jugador en formato inválido.");
	}
	if (!(savedFile >> savedButterflies)) {
		throw FileFormatError("saved.txt", 3, "Número de mariposas en formato inválido.");
	}

	// Leemos los datos del nivel correspondiente al guardado
	const string levelFileName = string(LEVEL_ROOT) + to_string(savedLevel) + ".txt";
	ifstream levelFile(levelFileName);
	if (!levelFile.is_open()) {
		throw FileNotFoundError(levelFileName);
	}
	loadGame(levelFile, levelFileName);
	levelFile.close();

	// Actualizamos los datos en base a la carga de la partida
	levelToLoad = savedLevel + 1;
	playerPoints = savedPoints;
	numButterflies = savedButterflies;

	// Eliminamos todos los objetos de la escena
	auto it = sceneObjects.begin();
	while (it != sceneObjects.end()) {
		delete *it;
		it = sceneObjects.erase(it);
	}
	bow = nullptr;
	lastArrow = nullptr;

	// Leemos todos los objetos
	char c;
	int lineNumber = 4;
	while (savedFile >> c) {
		switch (c) {
		case 'B':
			if (bow != nullptr) {
				throw FileFormatError("saved.txt", lineNumber, "Existencia de dos arcos.");
			}
			bow = Bow::loadObject(savedFile, this, lineNumber);
			addObject(bow);
			break;
		case 'b':
			addObject(Balloon::loadObject(savedFile, this, lineNumber));
			break;
		case 'A':
			lastArrow = Arrow::loadObject(savedFile, this, lineNumber);
			addObject(lastArrow);
			break;
		case 'f':
			addObject(Butterfly::loadObject(savedFile, this, generator, lineNumber));
			break;
		default:
			throw FileFormatError("saved.txt", lineNumber, "Incoherencia en formato.");
		}
		++lineNumber;
	}

	if (bow == nullptr) {
		throw FileFormatError("saved.txt", lineNumber, "Arco no encontrado.");
	}

	// Actualizamos el estado del juego
	infoBar->updateArrows(bow->getArrows());
	infoBar->updatePlayerPoints(playerPoints);
	LastBalloonTime = SDL_GetTicks();
	nextBalloon = exponential_distribution<double>(BALLOONS_PER_DELAY / baldelay)(generator);
	savedFile.close();
}
