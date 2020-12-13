#pragma once

namespace OUtil {
    namespace MathHelper {
        template <typename T>
        T clamp(T min, T max, T value) { return (value < min)? min: (value > max)? max : value; } 
    }
}