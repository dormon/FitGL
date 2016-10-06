#include<SDLWindow/SDLWindow.h>

#include<iostream>
#include<cassert>

SDLWindow::SDLWindow(uint32_t width, uint32_t height, bool resizable, bool fullscreen, int multisample) {
    assert(this != nullptr);
    Uint32 flags = SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN;
    if (resizable) flags |= SDL_WINDOW_RESIZABLE;
    if (fullscreen) flags |= SDL_WINDOW_FULLSCREEN_DESKTOP;
    if (multisample > 1) {
        SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
        SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, multisample);
    }
	SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
    this->m_window = SDL_CreateWindow("FitGL", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, flags);
}

SDLWindow::~SDLWindow() {
    assert(this != nullptr);
    SDL_DestroyWindow(this->m_window);
}

SDL_GLContext SDLWindow::createContext(
        std::string const&name,
        uint32_t version,
        Profile profile,
        Flag flags,
        bool vsync) {
    assert(this != nullptr);
    //SDL_GL_SetAttribute(SDL_GL_SHARE_WITH_CURRENT_CONTEXT, 1);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, version / 100);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, (version % 100) / 10);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, profile);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, flags);
    SharedSDLContext ctx = std::shared_ptr<SDL_GLContext>(
            new SDL_GLContext,
            [&](SDL_GLContext * ctx) {
                if (*ctx)SDL_GL_DeleteContext(*ctx);
                delete ctx; });
    *ctx = SDL_GL_CreateContext(this->m_window);
    if (*ctx == nullptr)
        return nullptr;
    this->m_contexts[name] = ctx;
    if (vsync) {
        // late swap tearing
        if (SDL_GL_SetSwapInterval(-1) == -1) {
            // true vsync
            SDL_GL_SetSwapInterval(1);
        }
    } else {
        // vsync off
        SDL_GL_SetSwapInterval(0);
    }
    return *ctx;
}

void SDLWindow::setContext(
        std::string const&name,
        SDLWindow const&other,
        std::string const&otherName) {
    assert(this != nullptr);
    assert(other.m_contexts.count(otherName) != 0);
    this->m_contexts[name] = other.m_contexts.find(otherName)->second;
}

void SDLWindow::makeCurrent(std::string const&name)const {
    assert(this != nullptr);
    assert(this->m_contexts.count(name) != 0);
    SDL_GL_MakeCurrent(this->m_window, *this->m_contexts.find(name)->second);
}

void SDLWindow::makeCurrent(SDL_GLContext & context) {
    assert(context != nullptr);
    SDL_GL_MakeCurrent(this->m_window, context);
}

void SDLWindow::swap()const {
    assert(this != nullptr);
    SDL_GL_SwapWindow(this->m_window);
}

SDLWindow::WindowId SDLWindow::getId()const {
    assert(this != nullptr);
    return SDL_GetWindowID(this->m_window);
}

void SDLWindow::setEventCallback(
        EventType const&eventType,
        EventCallback const&callback) {
    assert(this != nullptr);
    this->m_eventCallbacks[eventType] = callback;
}

bool SDLWindow::hasEventCallback(
        EventType const&eventType)const {
    assert(this != nullptr);
    auto ii = this->m_eventCallbacks.find(eventType);
    return ii != this->m_eventCallbacks.end() && ii->second != nullptr;
}

void SDLWindow::callEventCallback(
        EventType const&eventType,
        SDL_Event const&event) {
    assert(this != nullptr);
    assert(this->m_eventCallbacks.count(eventType) != 0);
    assert(this->m_eventCallbacks[eventType] != nullptr);
    (this->m_eventCallbacks[eventType])(event);
}

void SDLWindow::setSize(uint32_t width, uint32_t heght) {
    assert(this != nullptr);
    SDL_SetWindowSize(this->m_window, width, heght);
}

uint32_t SDLWindow::getWidth()const {
    assert(this != nullptr);
    int size[2];
    SDL_GetWindowSize(this->m_window, size + 0, size + 1);
    return size[0];
}

uint32_t SDLWindow::getHeight()const {
    assert(this != nullptr);
    int size[2];
    SDL_GetWindowSize(this->m_window, size + 0, size + 1);
    return size[1];
}

void SDLWindow::move(int x, int y) {
    SDL_SetWindowPosition(m_window, x, y);
}

void SDLWindow::setFullscreen(Fullscreen const&type) {
    SDL_SetWindowFullscreen(this->m_window, type);
}

void SDLWindow::setTitle(std::string const & title) {
    SDL_SetWindowTitle(m_window, title.c_str());
}
