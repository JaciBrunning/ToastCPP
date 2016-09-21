#pragma once

#include "toast/library.hpp"
#include "toast/math/math.hpp"

#include <string>

namespace Toast {
	namespace Math {
		struct Vec2D {
			double x = 0, y = 0;

			API Vec2D() {}
			API Vec2D(double _x, double _y) : x(_x), y(_y) {}
			
			API inline double magnitude()	const { return sqrt(x*x + y*y); }
			API inline double heading()		const { return atan2(y, x); }

			API Vec2D unit() const {
				double mag = magnitude();
				return Vec2D(x / mag, y / mag);
			}

			API inline double dot(Vec2D &other)				const { return x * other.x + y * other.y; }
			API inline double project_scalar(Vec2D &other)	const { return dot(other) / other.magnitude(); }
			API inline Vec2D project(Vec2D &other) const;
			API inline Vec2D relative_to(Vec2D &other) const;
			API inline Vec2D rotate(double angle) const {
				return Vec2D(x * cos(angle) - y * sin(angle), x * sin(angle) + y * cos(angle));
			}

			API inline Vec2D rotate_to(double angle) const {
				return rotate(angle - heading());
			}

			API static inline Vec2D from_angle(double angle, double magnitude) {
				return Vec2D(magnitude * cos(angle), magnitude * sin(angle));
			}

			API inline operator std::string() const {
				char buf[20];
				sprintf(&buf[0], "<%.3f, %.3f>", x, y);
				return std::string(buf);
			}
		};

		// Note that this is component-wise. i.e. <x1, y1> * <x2, y2> = <x1x2, y1y2>
		inline Vec2D operator*(const Vec2D &vec1, const Vec2D &vec2) {
			return Vec2D(vec1.x * vec2.x, vec1.y * vec2.y);
		}
		inline Vec2D operator*(const Vec2D &vec, double scalar) {
			return Vec2D(vec.x * scalar, vec.y * scalar);
		}
		inline Vec2D operator*(double scalar, const Vec2D &vec) { return vec * scalar; }

		// Note that this is component-wise. i.e. <x1, y1> / <x2, y2> = <x1/x2, y1/y2>
		inline Vec2D operator/(const Vec2D &vec1, const Vec2D &vec2) {
			return Vec2D(vec1.x / vec2.x, vec1.y / vec2.y);
		}
		inline Vec2D operator/(const Vec2D &vec, double scalar) {
			return Vec2D(vec.x / scalar, vec.y / scalar);
		}
		inline Vec2D operator/(double scalar, const Vec2D &vec) {
			return Vec2D(scalar / vec.x, scalar / vec.y);
		}

		inline Vec2D operator+(const Vec2D &vec1, const Vec2D &vec2) {
			return Vec2D(vec1.x + vec2.x, vec1.y + vec2.y);
		}
		inline Vec2D operator-(const Vec2D &vec1, const Vec2D &vec2) {
			return Vec2D(vec1.x - vec2.x, vec1.y - vec2.y);
		}
		inline Vec2D operator-(const Vec2D &vec) {
			return Vec2D(-vec.x, -vec.y);
		}

		// Declared above
		Vec2D Vec2D::project(Vec2D &other) const {
			return other * project_scalar(other);
		}

		Vec2D Vec2D::relative_to(Vec2D &other) const {
			return (*this) - other;
		}
	}
}
