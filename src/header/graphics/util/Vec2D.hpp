#pragma once


namespace OUtil {
    struct Vec2D {
        int x, y;

        public:
        const static Vec2D ORIGIN;

        Vec2D relativeTo(Vec2D parentPosition) const;
        Vec2D operator-(const Vec2D& other) const;
        Vec2D operator+(const Vec2D& other) const;
        Vec2D operator+(const int& scalar) const;
        Vec2D operator/(const int& scalar) const;
        bool operator==(const Vec2D& other) const;
    
        Vec2D getVersor() const;
    };
}