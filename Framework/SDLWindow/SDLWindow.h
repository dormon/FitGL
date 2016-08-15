#pragma once

#include<iostream>
#include<map>
#include<memory>

#include<SDL2/SDL.h>

#include<SDLWindow/SDLEventProc.h>

class SDLWindow {
public:
    using WindowId = uint32_t;
    using EventType = uint32_t;

    enum Profile {
        CORE = SDL_GL_CONTEXT_PROFILE_CORE,
        COMPATIBILITY = SDL_GL_CONTEXT_PROFILE_COMPATIBILITY,
        ES = SDL_GL_CONTEXT_PROFILE_ES,
    };

    enum Flag {
        NONE = 0,
        DEBUG = SDL_GL_CONTEXT_DEBUG_FLAG,
        FORWARD_COMPATIBLE = SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG,
        ROBUST_ACCESS = SDL_GL_CONTEXT_ROBUST_ACCESS_FLAG,
        RESET_ISOLATION = SDL_GL_CONTEXT_RESET_ISOLATION_FLAG,
    };

    SDLWindow(uint32_t width = 1024, uint32_t height = 768,
            bool resizable = true, bool fullscreen = false, int multisample = 1);
    ~SDLWindow();
    SDL_GLContext createContext(
            std::string const&name = "context",
            uint32_t version = 450u,
            Profile profile = CORE,
            Flag flags = NONE,
            bool vsync = false);
    void setContext(
            std::string const&name,
            SDLWindow const&other,
            std::string const&otherName);
    void makeCurrent(std::string const&name)const;
    void makeCurrent(SDL_GLContext &context);
    void swap()const;
    WindowId getId()const;

    void setEventCallback(
            EventType const&eventType,
            EventCallback const&callback = nullptr);
    bool hasEventCallback(
            EventType const&eventType)const;
    void callEventCallback(
            EventType const&eventType,
            SDL_Event const&eventData);
    void setSize(uint32_t width, uint32_t heght);
    uint32_t getWidth()const;
    uint32_t getHeight()const;
    void move(int x, int y);

    enum Fullscreen {
        WINDOW = 0,
        FULLSCREEN = SDL_WINDOW_FULLSCREEN,
        FULLSCREEN_DESKTOP = SDL_WINDOW_FULLSCREEN_DESKTOP,
    };
    void setFullscreen(Fullscreen const&type);
    void setTitle(std::string const &title);
protected:
    using SharedSDLContext = std::shared_ptr<SDL_GLContext>;
    SDL_Window* m_window = nullptr;
    std::map<std::string, SharedSDLContext> m_contexts;
    std::map<EventType, EventCallback>m_eventCallbacks;
};

