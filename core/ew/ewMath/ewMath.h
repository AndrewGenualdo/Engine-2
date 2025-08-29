#pragma once
#include <random>
#include <glm/geometric.hpp>

namespace ew {
	constexpr float PI = 3.14159265359f;
	constexpr float TAU = 6.283185307179586f;
	constexpr float DEG2RAD = (PI / 180.0f);
	constexpr float RAD2DEG = (180.0f / PI);
	inline float Radians(float degrees) {
		return degrees * DEG2RAD;
	}
	inline float RandomRange(float min, float max) {
		float t = (float)rand() / RAND_MAX;
		return min + (max - min) * t;
	}
	inline float lerp(float a, float b, float t) {
		return a + t * (b - a);
	}
	inline glm::vec2 lerp(glm::vec2 a, glm::vec2 b, float t) {
		return glm::vec2(lerp(a.x, b.x, t), lerp(a.y, b.y, t));
	}
	inline float normalize(float value) {
		if(value == 0) return 0;
		return value / glm::abs(value);
	}

	inline glm::vec2 closestPointOnLine(const glm::vec2& A, const glm::vec2& B, const glm::vec2& P) {
		glm::vec2 AB = B - A;
		glm::vec2 AP = P - A;

		float t = glm::dot(AP, AB) / glm::dot(AB, AB);

		// If you want the closest point *on the segment*, clamp t to [0, 1]:
		t = glm::clamp(t, 0.0f, 1.0f);

		return A + t * AB;
	}

	// Helper: check intersection of two finite line segments
	inline bool linesIntersect(glm::vec2 a1, glm::vec2 a2, glm::vec2 b1, glm::vec2 b2) {
		auto cross = [](glm::vec2 u, glm::vec2 v) {
			return u.x * v.y - u.y * v.x;
		};

		glm::vec2 r = a2 - a1;
		glm::vec2 s = b2 - b1;
		float denom = cross(r, s);

		if (denom == 0) return false; // Parallel or collinear

		float t = cross(b1 - a1, s) / denom;
		float u = cross(b1 - a1, r) / denom;

		return (t > 0 && t < 1 && u > 0 && u < 1); // Strictly inside, no endpoints
	}

	inline bool lineIntersectsSquare(glm::vec2 p1, glm::vec2 p2,
							  glm::vec2 squarePos, glm::vec2 squareDim)
	{
		// Shrink the square slightly to avoid corner grazing
		constexpr float epsilon = -0.0001f;
		glm::vec2 min = squarePos + glm::vec2(epsilon);
		glm::vec2 max = squarePos + squareDim - glm::vec2(epsilon);

		// Quick inside check
		auto pointInside = [&](glm::vec2 p) {
			return (p.x > min.x && p.x < max.x && p.y > min.y && p.y < max.y);
		};
		if (pointInside(p1) || pointInside(p2))
			return true;

		// Define rectangle edges
		glm::vec2 topLeft     = min;
		glm::vec2 topRight    = { max.x, min.y };
		glm::vec2 bottomLeft  = { min.x, max.y };
		glm::vec2 bottomRight = max;

		// Check if the line segment crosses any edge strictly inside
		if (linesIntersect(p1, p2, topLeft, topRight)) return true;
		if (linesIntersect(p1, p2, topRight, bottomRight)) return true;
		if (linesIntersect(p1, p2, bottomRight, bottomLeft)) return true;
		if (linesIntersect(p1, p2, bottomLeft, topLeft)) return true;

		return false;
	}
}
