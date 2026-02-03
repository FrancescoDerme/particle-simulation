#include "platform_utils.hpp"

#include <SFML/Window/VideoMode.hpp>

#if defined(__linux__)
// X11 includes are to be kept inside this .cpp file
// because they define macros that conflict with SFML
#include <X11/Xatom.h>
#include <X11/Xlib.h>

// Function to toggle fullscreen using X11 Atom messages
void toggleFullscreen(sf::RenderWindow& window) {
    // Get the native handle
    auto handle = window.getNativeHandle();

    // Open a temporary connection to the X server
    Display* dpy = XOpenDisplay(NULL);
    if (!dpy) return;

    // Get the Atoms for the Window Manager State
    Atom wmState = XInternAtom(dpy, "_NET_WM_STATE", False);
    Atom fullScreen = XInternAtom(dpy, "_NET_WM_STATE_FULLSCREEN", False);

    // Prepare the ClientMessage event
    XEvent xev;
    xev.type = ClientMessage;
    xev.xclient.window = handle;
    xev.xclient.message_type = wmState;
    xev.xclient.format = 32;

    // 0 = Remove, 1 = Add, 2 = Toggle
    xev.xclient.data.l[0] = 2;
    xev.xclient.data.l[1] = fullScreen;
    xev.xclient.data.l[2] = 0;  // No second property to toggle
    xev.xclient.data.l[3] = 1;  // Normal source indication

    // Send the event to the Root Window (asking WM to handle it)
    XSendEvent(dpy, DefaultRootWindow(dpy), False,
               SubstructureRedirectMask | SubstructureNotifyMask, &xev);

    // Flush and close
    XFlush(dpy);
    XCloseDisplay(dpy);
}

#else
void toggleFullscreen(sf::RenderWindow& window) {
    static bool isFullscreen = false;
    static sf::Vector2u lastSize = {WIDTH, HEIGHT};
    static sf::Vector2i lastPos;

    if (isFullscreen) {
        // Revert to Windowed
        window.create(sf::VideoMode(lastSize), "Cloth Simulation",
                      sf::Style::Default, sf::State::Windowed);
        window.setPosition(lastPos);
        isFullscreen = false;
    }
    else {
        // Save current state before switching
        lastSize = window.getSize();
        lastPos = window.getPosition();

        // Switch to Fullscreen
        window.create(sf::VideoMode::getDesktopMode(), "Cloth Simulation",
                      sf::Style::Default, sf::State::Fullscreen);
        isFullscreen = true;
    }
}
#endif

fs::file_time_type getLatestFolderTime(const fs::path& path) {
    fs::file_time_type maxTime = fs::file_time_type::min();

    for (const auto& entry : fs::recursive_directory_iterator(path)) {
        if (entry.is_regular_file()) {
            if (entry.path().extension() != ".cpp" &&
                entry.path().extension() != ".h")
                continue;

            auto currentTime = entry.last_write_time();
            if (currentTime > maxTime) {
                maxTime = currentTime;
            }
        }
    }

    return maxTime;
}
