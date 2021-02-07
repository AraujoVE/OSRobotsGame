#pragma once

namespace DampEngine
{
    enum class RenderingAPI {
        None = 0, OpenGL
    };

    class Renderer {
        public:
            inline static RenderingAPI GetCurrentAPI() { return s_RenderingAPI; }
            inline static void SetApi(RenderingAPI api) { s_RenderingAPI = api; }

        private:
        static RenderingAPI s_RenderingAPI;
    };

    RenderingAPI Renderer::s_RenderingAPI = RenderingAPI::OpenGL;

    
} // namespace DampEngine
