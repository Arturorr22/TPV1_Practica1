#
# Proyecto CMake para las prácticas de TPV1
#

cmake_minimum_required(VERSION 3.15...3.30)

# Proyecto en C++
project(ProyectoSDL VERSION 1.0 LANGUAGES CXX)

# Selecciona la versión C++20 del estándar (sin extensiones)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Opción para activar el comprobador de memoria
option(ENABLE_ASAN "Activa el comprobador de memoria AddressSanitizer")

# Compila con la biblioteca SDL
if (CMAKE_HOST_WIN32)
	# Utiliza las bibliotecas compiladas que van incluidas
	# en el paquete con el proyecto de Visual Studio

	include_directories(
		libs/SDL3/include
		libs/SDL3_image/include
	)

	link_directories(
		libs/SDL3/lib
		libs/SDL3_image/lib
	)

	link_libraries(
		SDL3.lib
		SDL3_image.lib
	)

	# Si se utiliza el compilador de Visual Studio
	if (MSVC)
		if (ENABLE_ASAN)
			add_compile_options("$<$<CONFIG:Debug>:/fsanitize=address>")
		else()
			# Añade checkML.h automáticamente a todas las unidades de compilación
			add_compile_options("$<$<CONFIG:Debug>:/FI${CMAKE_SOURCE_DIR}/libs/misc/include/checkML.h>")
		endif()
	endif()
else()
	# En otros sistemas operativos utiliza find_package
	# para encontrar las bibliotecas instaladas aparte

	find_package(SDL3 REQUIRED)
	find_package(SDL3_image REQUIRED)

	link_libraries(SDL3::SDL3)
endif()

# Activa AddressSanitizer
if (ENABLE_ASAN AND NOT MSVC)
	add_compile_options("$<$<CONFIG:Debug>:-fsanitize=address>")
	add_link_options("$<$<CONFIG:Debug>:-fsanitize=address>")
endif()

add_executable(ProyectoSDL
	# Archivo de código
	src/main.cpp
	src/game.cpp
	src/rectangle.cpp
	src/texture.cpp
)
