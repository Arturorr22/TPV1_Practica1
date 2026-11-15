#include "game.h"
#include <limits>
#include "bow.h"
#include "balloon.h"
#include "arrow.h"
#include "infobar.h"
#include "butterfly.h"
#include "error.h"

#include <SDL3_image/SDL_image.h>

#include <string>
#include <fstream>
#include <sstream>

using namespace std;

// Estructura para especificar las texturas que hay que
// cargar y el tamaño de su matriz de frames
struct TextureSpec
{
	const char* name;
	int nrows = 1;
	int ncols = 1;
};

constexpr array<TextureSpec, Game::NUM_TEXTURES> textureList{
	TextureSpec{"balloons.png", 7, 6},
	{"arrow.png"},
	{"bow.png", 1, 2},
	{"digits.png", 1, 10},
	{"butterfly.png", 4, 10},
};


Game::Game()
	: exit(false)
	, window(nullptr)
	, renderer(nullptr)
	, textures{}
	, bow(nullptr)
	, lastArrow(nullptr)
	, infoBar(nullptr)
{
	try {
		initSDL();
		loadTextures();
		loadLevel(FIRST_LEVEL);
		createInitialObjects();
	}
	catch (...) {

		// Si el constructor lanza una excepción, el destructor no se ejecuta,
		// así que liberamos aquí lo que se hubiera reservado hasta el momento
		freeResources();
		throw; // relanza la excepción
	}
}

// Carga SDL y crea la ventana con su renderizador
void
Game::initSDL()
{
	if (!SDL_Init(SDL_INIT_VIDEO))
		throw SDLError("SDL_Init");

	if (!SDL_CreateWindowAndRenderer(WINDOW_TITLE, WINDOW_WIDTH, WINDOW_HEIGHT, 0, &window, &renderer))
		throw SDLError("creacion de la ventana y el renderizador");
}

// Carga las texturas al inicio
void
Game::loadTextures()
{
	string imgRoot = IMAGE_ROOT;

	for (size_t i = 0; i < textures.size(); i++) {
		auto [name, nrows, ncols] = textureList[i];
		textures[i] = new Texture(renderer, (imgRoot + name).c_str(), nrows, ncols);
	}
}

// Crea los objetos iniciales de la partida y prepara los generadores aleatorios
void
Game::createInitialObjects()
{
	// Creamos el arco
	bow = new Bow(this, Rectangle({ 0, (float)(WINDOW_HEIGHT / 2 - WINDOW_HEIGHT / BOW_HEIGHT_DIV) },
		WINDOW_WIDTH / BOW_WIDTH_DIV, WINDOW_HEIGHT / BOW_HEIGHT_DIV), textures[BOW], arrows, false);

	// Añadimos el arco a la lista de SceneObjects
	addObject(bow);

	// Creamos la infoBar
	infoBar = new InfoBar(this, textures[ARROW], textures[DIGITS], arrows, playerPoints);

	// Configuramos la semilla del generador para la generación de globos
	generator.seed(std::random_device()());

	// Creamos las mariposas
	createButterflies();

	// Calculamos el tiempo que ha de pasar hasta que aparezca el primer globo
	nextBalloon = exponential_distribution<double>(BALLOONS_PER_DELAY / baldelay)(generator);

	// Almacenamos el tiempo actual al iniciar el programa en la variable LastBalloonTime
	LastBalloonTime = SDL_GetTicks();
}

Game::~Game() {
	freeResources();
}

// Libera los recursos reservados. Se puede llamar aunque la inicialización haya
// fallado a medias, porque comprueba cada recurso antes de liberarlo
void
Game::freeResources()
{
	// Destruimos todos los objetos de la escena
	for (SceneObject* obj : sceneObjects)
		delete obj;
	sceneObjects.clear();

	// Borramos la infoBar
	delete infoBar;
	infoBar = nullptr;

	// Destruimos las texturas
	for (Texture*& texture : textures) {
		delete texture;
		texture = nullptr;
	}

	// Destruimos el renderer, la ventana y cerramos SDL
	if (renderer != nullptr) SDL_DestroyRenderer(renderer);
	if (window != nullptr) SDL_DestroyWindow(window);
	renderer = nullptr;
	window = nullptr;
	SDL_Quit();
}

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

void
Game::createArrow(const Rectangle& bowRect) {

	// Creamos una flecha y la añadimos a la lista de objetos de la escena
	// Antes de incorporarla a la lista, hacemos que el puntero lastArrow apunte a ella
	lastArrow = new Arrow(this, Rectangle({ 0,
		bowRect.getTopLeftCorner().getY() + (float)(WINDOW_HEIGHT / ARROW_OFFSET_DIV) },
		WINDOW_WIDTH / ARROW_WIDTH_DIV, WINDOW_HEIGHT / ARROW_HEIGHT_DIV),
		textures[ARROW], Vector2D(ARROW_SPEED, 0.0f));
	sceneObjects.push_back(lastArrow);

	// Actualizamos el valor de arrows al actual
	--arrows;
	infoBar->updateArrows(arrows);
}

bool
Game::checkCollision(const Rectangle& rect) {

	// Recorremos todos los objetos de la escena comprobando si se dan colisiones
	auto it = sceneObjects.begin();
	while (it != sceneObjects.end() && !(*it)->hit(rect)) {
		++it;
	}
	return it != sceneObjects.end();
}

void
Game::increasePoints() {
	playerPoints += balPoints;
	infoBar->updatePlayerPoints(playerPoints);
}

void
Game::decreasePoints() {
	if (playerPoints >= balPoints) {
		playerPoints -= balPoints;
		infoBar->updatePlayerPoints(playerPoints);
	}
}

namespace {

	// Lee el siguiente entero de la línea y comprueba que esté entre minValue y maxValue.
	// Si no es válido, lanza un FileFormatError con el fichero y la línea del error
	int readValue(istream& line, const string& keyword, int minValue, int maxValue,
		const string& fileName, int lineNumber) {
		int value;
		if (!(line >> value))
			throw FileFormatError(fileName, lineNumber,
				"falta un numero valido para '" + keyword + "'");
		if (value < minValue || value > maxValue)
			throw FileFormatError(fileName, lineNumber, "'" + keyword + "' debe estar entre "
				+ to_string(minValue) + " y " + to_string(maxValue));
		return value;
	}
}

// Genera un número aleatorio entre min y max con el generador del juego
float
Game::randomFloat(float min, float max)
{
	return uniform_real_distribution<float>(min, max)(generator);
}

// Devuelve la ruta del fichero de un nivel
string
Game::levelPath(int level) const
{
	return string(LEVEL_ROOT) + to_string(level) + ".txt";
}

// Comprueba si existe el fichero de un nivel, sin lanzar ninguna excepción
bool
Game::levelExists(int level) const
{
	return ifstream(levelPath(level)).is_open();
}

// Lee un nivel, línea a línea, y modifica los atributos de Game. Lanza FileNotFoundError
// si no existe el fichero y FileFormatError si alguna línea no es válida
void
Game::loadLevel(int level)
{
	string path = levelPath(level);
	ifstream file(path);
	if (!file.is_open())
		throw FileNotFoundError(path);

	// Asignamos unos valores por defecto a los atributos de Game por si el archivo
	// no los modifica
	backgroundColor = DEFAULT_BACKGROUND;
	arrows = DEFAULT_ARROWS;
	baldelay = DEFAULT_BALDELAY;
	balPoints = DEFAULT_BALPOINTS;
	goal = DEFAULT_GOAL;
	numButterflies = DEFAULT_BUTTERFLIES;

	// Analizamos el archivo línea a línea, contándolas para poder informar de los errores
	string text;
	for (int lineNumber = 1; getline(file, text); ++lineNumber) {
		istringstream line(text);
		string keyword;
		line >> keyword;
		readLevelOption(keyword, line, path, lineNumber);
	}

	// Fijamos los puntos del jugador a 0 y el siguiente nivel a cargar
	playerPoints = 0;
	levelToLoad = level + 1;
}

// Analiza una línea del archivo de un nivel: keyword es su primera palabra y line
// contiene el resto. Las líneas vacías, los comentarios (#) y las palabras clave
// desconocidas se ignoran sin avisar
void
Game::readLevelOption(const string& keyword, istream& line, const string& fileName, int lineNumber)
{
	// Lee el valor entero que sigue a la palabra clave
	auto read = [&](int minValue, int maxValue) {
		return readValue(line, keyword, minValue, maxValue, fileName, lineNumber);
		};

	if (keyword == "bgcolor") {
		backgroundColor.r = read(MIN_BACKGROUND, MAX_BACKGROUND);
		backgroundColor.g = read(MIN_BACKGROUND, MAX_BACKGROUND);
		backgroundColor.b = read(MIN_BACKGROUND, MAX_BACKGROUND);
	}
	else if (keyword == "arrows") arrows = read(MIN_ARROWS, MAX_ARROWS);
	else if (keyword == "baldelay") baldelay = read(MIN_BALDELAY, MAX_BALDELAY);
	else if (keyword == "balpoints") balPoints = read(MIN_BALPOINTS, MAX_BALPOINTS);
	else if (keyword == "goal") goal = read(MIN_GOAL, MAX_GOAL);
	else if (keyword == "butterflies") numButterflies = read(MIN_BUTTERFLIES, MAX_BUTTERFLIES);
}

// Implementación del método addObject de la clase Game
void
Game::addObject(SceneObject* obj)
{
	if (obj != nullptr) {
		sceneObjects.push_back(obj);
	}
}

void
Game::createBalloon()
{
	// Calculamos, de manera pseudoaleatoria, la posición del borde inferior
	// en la que aparecerá el globo, junto con su color y su velocidad en el
	// eje y
	int color = uniform_int_distribution<int>(0, NUM_BALLOON_COLORS - 1)(generator);
	float speedY = uniform_real_distribution<float>(MIN_BALLOON_SPEED,
		MAX_BALLOON_SPEED)(generator);
	float posX = uniform_real_distribution<float>(WINDOW_WIDTH / BALLOON_MIN_X_DIV,
		WINDOW_WIDTH - WINDOW_WIDTH / BALLOON_WIDTH_DIV)(generator);

	// Creamos el globo y lo añadimos a sceneObjects
	sceneObjects.push_back(new Balloon(this, Rectangle({ posX, WINDOW_HEIGHT },
		WINDOW_WIDTH / BALLOON_WIDTH_DIV, WINDOW_HEIGHT / BALLOON_HEIGHT_DIV),
		textures[BALLOON], Vector2D<>(0, speedY), color));

	// Recalculamos el valor hasta el siguiente globo
	nextBalloon = exponential_distribution<double>(BALLOONS_PER_DELAY / baldelay)(generator);

	// Actualizamos el valor de LastBalloonTime
	LastBalloonTime = SDL_GetTicks();
}

void
Game::createButterflies()
{
	// Calculamos el tamaño, igual para todas: el alto sale de la proporción del frame
	float width = WINDOW_WIDTH / BUTTERFLY_WIDTH_DIV;
	float height = width / textures[BUTTERFLY]->getAspectRatio();

	for (int i = 0; i < numButterflies; ++i) {

		// Posición aleatoria con la mariposa completa dentro de la ventana
		float posX = uniform_real_distribution<float>(0.0f, WINDOW_WIDTH - width)(generator);
		float posY = uniform_real_distribution<float>(0.0f, WINDOW_HEIGHT - height)(generator);

		// La dirección inicial la elige la propia mariposa
		sceneObjects.push_back(new Butterfly(this, Rectangle({ posX, posY }, width, height),
			textures[BUTTERFLY]));
	}
}

void
Game::render() const
{
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

void
Game::update()
{
	// Actualizamos todos los objetos de la escena
	auto it = sceneObjects.begin();
	while (it != sceneObjects.end()) {
		if ((*it)->update(FRAME_PERIOD)) ++it;
		else {

			// Comprobamos si se trata de la última flecha de la que disponía el
			// jugador
			if (arrows == 0 && *it == lastArrow) exit = true;
			delete* it;
			it = sceneObjects.erase(it);
		}
	}

	// Comprobamos si ya ha pasado suficiente tiempo desde la generación del último globo
	if (SDL_GetTicks() - LastBalloonTime > nextBalloon) createBalloon();

	// Si el jugador supera la meta, cargamos el siguiente nivel
	if (goal != 0 && playerPoints >= goal) loadNextLevel();
}

void
Game::handleEvents()
{
	SDL_Event event;

	// Solo quit se maneja directamente, todo lo demás se delega al arco (Bow)
	while (SDL_PollEvent(&event)) {
		if (event.type == SDL_EVENT_QUIT)
			exit = true;
		else if (event.type == SDL_EVENT_KEY_DOWN || event.type == SDL_EVENT_KEY_UP) {
			bow->handleEvent(event);
		}
	}
}

void
Game::loadNextLevel()
{
	// Si no existe el siguiente nivel, el jugador ha superado todos y gana la partida
	if (!levelExists(levelToLoad)) {
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, WINDOW_TITLE, VICTORY_MESSAGE, window);
		exit = true;
		return;
	}

	// Borramos todos los objetos de la escena antes de pasar al siguiente nivel
	// (exceptuando el arco)
	auto it = ++sceneObjects.begin();
	while (it != sceneObjects.end()) {
		delete* it;
		it = sceneObjects.erase(it);
	}

	// Cargamos el nivel y creamos sus mariposas
	loadLevel(levelToLoad);
	createButterflies();

	// Actualizamos el número de flechas del arco
	bow->updateArrows(arrows);

	// Actualizamos la infoBar
	infoBar->updateArrows(arrows);
	infoBar->updatePlayerPoints(playerPoints);
}
