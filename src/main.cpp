#include <GL/gl.h>
#include <SDL.h>
#include <SDL_opengl.h>
#include "imgui/imgui.h"
#include "imgui/backends/imgui_impl_sdl.h"
#include "imgui/backends/imgui_impl_opengl3.h"


int main(int argc, char **argv) {

    // Set framerate limit.
    int frameLimitMilliseconds = 1000 / 60;

    // Set reqired GL and shader version.
    struct {
        int glVersionMajor = 3;
        int glVersionMinor = 2;
        const char *glslShaderVersion = "#version 150";
    } glRequirements;


    // Initialise SDL.
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        printf("[ERROR] %s\n", SDL_GetError());
        return -1;
    }

    // Set SDL OpenGL framebuffer attributes.
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);

    // Set SDL OpenGL context attributes.
    SDL_GL_SetAttribute(
        SDL_GL_CONTEXT_PROFILE_MASK,
        SDL_GL_CONTEXT_PROFILE_CORE
    );

    // Set required version of SDL OpenGL.
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, glRequirements.glVersionMajor);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, glRequirements.glVersionMinor);

    // Set SDL window flags.
    SDL_WindowFlags window_flags = (SDL_WindowFlags) (
        SDL_WINDOW_OPENGL
        | SDL_WINDOW_RESIZABLE
        | SDL_WINDOW_ALLOW_HIGHDPI
        );

    // Create SDL window.
    SDL_Window *window = SDL_CreateWindow (
        "SDL Imgui sample",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        500,
        500,
        window_flags
        );
    SDL_SetWindowMinimumSize(window, 300, 300);

    // Create OpenGL context of SDL window.
    SDL_GLContext gl_context = SDL_GL_CreateContext(window);
    SDL_GL_MakeCurrent(window, gl_context);
    SDL_GL_SetSwapInterval(-1);     // -1 is adaptive vsync.

    // Setup Dear ImGui context and io.
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    // Note this relative path assumes binary is being executed from the 'build' directory.
    io.Fonts->AddFontFromFileTTF("./DroidSans.ttf", 16);

    // Set scale of UI.
    ImGui::GetStyle().ScaleAllSizes(1.5f);

    // Setup Dear ImGui style.
    ImGui::StyleColorsDark();

    // Setup Platform/Renderer bindings.
    // window is the SDL_Window*
    // gl_context is the SDL_GLContext
    ImGui_ImplSDL2_InitForOpenGL(window, gl_context);
    ImGui_ImplOpenGL3_Init(glRequirements.glslShaderVersion);

    // Main (SDL) event loop.
    bool done = false;
    while(!done) {
        // Handle events
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            // Forward events to Imgui first.
            ImGui_ImplSDL2_ProcessEvent(&event);
            // Check for quit / close.
            if (event.type == SDL_QUIT)
                done = true;
            if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_CLOSE && event.window.windowID == SDL_GetWindowID(window))
                done = true;
        }

        int frameLimiterTimeStart = SDL_GetTicks();

        // Clear window.
        glViewport(0, 0, (int)io.DisplaySize.x, (int)io.DisplaySize.y);
        ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
        glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        /*
            Render other stuff here...
        */

        // Begin the Imgui frame.
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplSDL2_NewFrame(window);
        ImGui::NewFrame();

        static float speed;
        static bool someBoolean;

        ImGui::Begin("Imgui window", 0, ImGuiWindowFlags_AlwaysAutoResize);
        ImGui::Checkbox("Boolean property", &someBoolean);
        if(ImGui::Button("Reset Speed")) {
            // This code is executed when the user clicks the button.
            speed = 0;
        }
        ImGui::SliderFloat("Speed", &speed, 0.0f, 10.0f);
        ImGui::End();

        // Render Imgui.
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        // Swap in new frame.
        SDL_GL_SwapWindow(window);

        // Framerate limiter.
        int frameLimiterTimeTaken = SDL_GetTicks() - frameLimiterTimeStart;
        if (frameLimiterTimeTaken < 0) continue;
        int sleepTime = frameLimitMilliseconds - frameLimiterTimeTaken;
        if (sleepTime > 0)
        {
            SDL_Delay(sleepTime);
        }
    }

    // Cleanup Imgui.
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();

    // Other cleanup here...
    if (window) {
        SDL_GL_DeleteContext(gl_context);
        SDL_DestroyWindow(window);
    }
    SDL_Quit();

    return 0;
}
