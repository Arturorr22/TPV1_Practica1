#ifndef VECTOR2D_H
#define VECTOR2D_H

#include <iostream>
#include <cmath>

// Vector bidimensional genérico.

template<std::floating_point T = float>
class Vector2D
{
	T x, y;

public:
	Vector2D(T x, T y) : x(x), y(y) { }
	Vector2D() : Vector2D(0, 0) { }

	// Coordenadas del vector
	T getX() const { return x; }
	T getY() const { return y; }

	// Operadores
	Vector2D operator+(const Vector2D& other) const {
		return {x + other.x, y + other.y};
	}
	Vector2D operator-(const Vector2D& other) const {
		return {x - other.x, y - other.y};
	}
	T operator*(const Vector2D& other) const {
		return x * other.x + y * other.y;
	}
	Vector2D operator*(const T& scalar) const {
		return {scalar * x, scalar * y};
	}
	Vector2D& operator+=(const Vector2D& other) {
		x += other.x;
		y += other.y;
		return *this;
	}

	// Método para obtener la longitud del vector
	T length() const {
		return std::sqrt(x * x + y * y);
	}

	// Operadores de entrada/salida
	friend std::ostream& operator<<(std::ostream& out, const Vector2D& v) {
		return out << '{' << v.x << ", " << v.y << '}';
	}
};

// Definimos el alias Point2D
template<std::floating_point T = float>
using Point2D = Vector2D<T>;

#endif // VECTOR2D_H
