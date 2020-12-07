#include "Vec2D.hpp"
#include "MathHelper.hpp"

namespace OUtil {

    Vec2D Vec2D::relativeTo(Vec2D parentPosition) const { return *this - parentPosition; }
    Vec2D Vec2D::operator-(const Vec2D& other) const { return {x - other.x, y - other.y}; } 
    Vec2D Vec2D::operator+(const Vec2D& other) const { return {x + other.x, y + other.y}; } 
    Vec2D Vec2D::operator+(const int& scalar) const { return {x * scalar, y * scalar}; } 
    Vec2D Vec2D::operator/(const int& scalar) const { return {x / scalar, y / scalar}; } 
    bool Vec2D::operator==(const Vec2D& other) const { return x == other.x && y == other.y;}

    Vec2D Vec2D::getVersor() const { 
        return { 
            MathHelper::clamp(-1, 1, x), 
            MathHelper::clamp(-1, 1, y)
        };     
    }

    const Vec2D Vec2D::ORIGIN = {0,0};

}