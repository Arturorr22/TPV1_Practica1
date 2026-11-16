#include <SDL3/SDL.h>

#include <stdexcept>
#include <string>

// Superclase de las excepciones del juego. Reutiliza el constructor y el método
// what() de std::logic_error; su tipo permite capturarlas por separado.
class GameError : public std::logic_error {
public:
	using std::logic_error::logic_error;
};

// Error de inicialización o uso de SDL. Guarda el contexto y el mensaje específico
// que proporciona SDL_GetError().
class SDLError : public GameError {
public:

	SDLError(const std::string& context)
		: GameError("Error de SDL en " + context + ": " + SDL_GetError()) {
	}
};

// Error al intentar abrir un fichero que no se encuentra. El mensaje incluye su nombre.
class FileNotFoundError : public GameError {
public:

	FileNotFoundError(const std::string& fileName)
		: GameError("No se ha encontrado el fichero: " + fileName) {
	}
};

// Error al leer un archivo de datos del juego. El mensaje incluye el nombre del
// archivo, la línea del error y su descripción.
class FileFormatError : public GameError {
private:
	// Compone el mensaje que se almacena en GameError.
	static std::string buildMessage(const std::string& fileName, int lineNumber,
		const std::string& message) {
		return "Error de formato en " + fileName + " (linea " + std::to_string(lineNumber)
			+ "): " + message;
	}

public:

	FileFormatError(const std::string& fileName, int lineNumber, const std::string& message)
		: GameError(buildMessage(fileName, lineNumber, message)) {
	}
};
