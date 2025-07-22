#include <exception>
#include <memory>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <time.h>

#ifdef __linux__
#include <sys/auxv.h>
#endif

#include "../../OpenGL.hpp"
#include "../../util/Vector.hpp"
#include "../../util/Logger.hpp"
#include "../../Game.hpp"
#include "../../EngineContext.hpp"

constexpr Vector2i kWindowDefaultSize = {800, 600};

std::weak_ptr<Game> g_game;

void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

int main() {
    if (!glfwInit()) {
		Log::err("<Desktop> Could not initialize GLFW");
		return -1;
	}
	Log::info("<Desktop> Initialized GLFW");
	//glfwWindowHint(GLFW_SAMPLES, 8);
	Vector2i monitor_size = kWindowDefaultSize;
	auto monitor = glfwGetPrimaryMonitor();
	if (monitor) {
		auto mode = glfwGetVideoMode(monitor);
		monitor_size.x = mode->width;
		monitor_size.y = mode->height;
	}
	float ui_scale = 1.0f;
	if (monitor_size.y >= 1440 && monitor_size.y < 2000)
		ui_scale = 1.5f;
	else if (monitor_size.y >= 2000)
		ui_scale = 2.0f;
	Vector2i window_size = kWindowDefaultSize * ui_scale;
	GLFWwindow *window = glfwCreateWindow(window_size.x, window_size.y, "Game", NULL, NULL);
	if (window != NULL) {
		Log::info("<Desktop> Created window");
		glfwSetWindowPos(window, std::max(0, monitor_size.x / 2 - window_size.x / 2),
			std::max(31, monitor_size.y / 2 - window_size.y / 2));
		glfwMakeContextCurrent(window);
		glfwSwapInterval(1);
#ifdef USE_GLES2
		int success = gladLoadGLES2Loader((GLADloadproc)glfwGetProcAddress);
#else
		int success = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
#endif
		if(success) {
#ifdef USE_GLES2
			Log::info("<Desktop> Running OpenGL ES version: {}", (const char *)glGetString(GL_VERSION));
#else
			Log::info("<Desktop> Running OpenGL version: {}", (const char *)glGetString(GL_VERSION));
#endif
			// Clear the screen to black
			glClearColor(0.0, 0.0, 0.0, 1.0);
			glClear(GL_COLOR_BUFFER_BIT);
			glfwSwapBuffers(window);
			glfwPollEvents();

			glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
			glfwSetKeyCallback(window, key_callback);
#ifdef __linux__
			int random_seed;
			int *seed_ptr = (int *)getauxval(AT_RANDOM);
			if (seed_ptr != NULL) {
				random_seed = *seed_ptr;
				Log::info("<Desktop> Got random seed from gexauxval(AT_RANDOM): {}", random_seed);
			} else {
				random_seed = time(0);
				Log::info("<Desktop> Got random seed from time(0): {}", random_seed);
			}
#else
			int randomSeed = time(0);
			Log::info("Got random seed from time(0): {}", randomSeed);
#endif
			std::shared_ptr<Game> game;
			try {
				game = std::make_shared<Game>(window_size, monitor_size.y, ui_scale, random_seed);

				g_game = game;
				bool first = true;
				double last_time = glfwGetTime();
				while(!glfwWindowShouldClose(window)) {
					if (!first) {
						double new_time = glfwGetTime();
						double delta = new_time - last_time;
						last_time = new_time;

						game->timeStep(delta);
					}
					first = false;
					game->render();

					glfwSwapBuffers(window);
					glfwPollEvents();
				}
			} catch (std::exception &e) {
				Log::err(e.what());
				Log::info("Terminating");
			}
		} else {
			Log::err("<Desktop> Could not load OpenGL");
		}
		if (!g_game.expired())
			Log::err("<Desktop> Termination error. Memory leak. Possibly somewhere a shared pointer to Game has been left");
		glfwDestroyWindow(window);
		Log::info("<Desktop> Destroyed window");
	} else {
		Log::err("<Desktop> Could not create window");
	}
	glfwTerminate();
	Log::info("<Desktop> Terminated GLFW");

	return 0;
}

std::shared_ptr<Game> EngineContext::game() {
	return g_game.lock();
}

void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
	auto game = g_game.lock();
	game->windowSize({width, height});
#ifdef WIN32
	game->render();
	glfwSwapBuffers(window);
#endif
	Log::dbg("<Desktop> Resized: {} x {}", width, height);
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if (action == GLFW_PRESS) {
		if (key == GLFW_KEY_F11) {
			static bool fullscreen = false;
			static int prev_width, prev_height;
			static int pos_x, pos_y;

			fullscreen = !fullscreen;
			GLFWmonitor* monitor = glfwGetPrimaryMonitor();
			if (!monitor)
				return;
			const GLFWvidmode* mode = glfwGetVideoMode(monitor);
			if (fullscreen) {
				glfwGetWindowSize(window, &prev_width, &prev_height);
				glfwGetWindowPos(window, &pos_x, &pos_y);
				glfwSetWindowMonitor(window, monitor, 0, 0, mode->width, mode->height, mode->refreshRate);
				glfwSwapInterval(1);
			}
			else {
				bool maximized = prev_width == mode->width;
				glfwSetWindowMonitor(window, NULL, pos_x, pos_y, prev_width, prev_height, GLFW_DONT_CARE);
				glfwSwapInterval(1);
				if (maximized)
					glfwMaximizeWindow(window);
			}
		} else {
			// TODO: Game key press
		}
	}
}
