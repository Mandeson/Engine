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
constexpr const char *kWindowTitle = "Game";
constexpr const char *kRunningOpenGLVersionText = "<Desktop> Running OpenGL {}";

void (*OpenGL::glBindFramebufferPtr)(GLenum, GLuint);
void (*OpenGL::glDeleteFramebuffersPtr)(GLsizei, const GLuint *);
void (*OpenGL::glGenFramebuffersPtr)(GLsizei, GLuint *);
GLenum (*OpenGL::glCheckFramebufferStatusPtr)(GLenum);
void (*OpenGL::glFramebufferTexture2DPtr)(GLenum, GLenum, GLenum, GLuint, GLint);

std::weak_ptr<Game> g_game;
bool g_is_gles = false;

void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

static void setupGLES() {
	glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
}

static void loadDefaultOpenGLFramebufferPtrs() {
	OpenGL::glBindFramebufferPtr = glBindFramebuffer;
	OpenGL::glDeleteFramebuffersPtr = glDeleteFramebuffers;
	OpenGL::glGenFramebuffersPtr = glGenFramebuffers;
	OpenGL::glCheckFramebufferStatusPtr = glCheckFramebufferStatus;
	OpenGL::glFramebufferTexture2DPtr = glFramebufferTexture2D;
}

static void loadExtensionOpenGLFramebufferPtrs() {
	OpenGL::glBindFramebufferPtr = glBindFramebufferEXT;
	OpenGL::glDeleteFramebuffersPtr = glDeleteFramebuffersEXT;
	OpenGL::glGenFramebuffersPtr = glGenFramebuffersEXT;
	OpenGL::glCheckFramebufferStatusPtr = glCheckFramebufferStatusEXT;
	OpenGL::glFramebufferTexture2DPtr = glFramebufferTexture2DEXT;
}

static GLFWwindow *createWindow(Vector2i window_size, Vector2i monitor_size) {
	GLFWwindow *window = glfwCreateWindow(window_size.x, window_size.y, kWindowTitle, NULL, NULL);
	if (window != NULL) {
		glfwSetWindowPos(window, std::max(0, monitor_size.x / 2 - window_size.x / 2),
			std::max(31, monitor_size.y / 2 - window_size.y / 2));
		glfwMakeContextCurrent(window);
		glfwSwapInterval(1);
	}
	return window;
}

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
#ifdef FORCE_GLES
	setupGLES();
#endif
	GLFWwindow *window = createWindow(window_size, monitor_size);
	if (window != NULL) {
		Log::info("<Desktop> Created window");
		bool error = false;
#ifdef FORCE_GLES
		if (gladLoadGLES2Loader((GLADloadproc)glfwGetProcAddress)) {
			if (GLAD_GL_ES_VERSION_2_0) {
				g_is_gles = true;
				loadDefaultOpenGLFramebufferPtrs();
				Log::info(kRunningOpenGLVersionText, "ES 2.0");
			} else {
				error = true;
				Log::info("<Desktop> OpenGL version too old. Engine requires OpenGL ES 2.0");
			}
		} else {
			error = true;
		}
#else
		if (gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
			if (GLAD_GL_VERSION_3_0) {
				loadDefaultOpenGLFramebufferPtrs();
				Log::info(kRunningOpenGLVersionText, "3.0");
			} else if (GLAD_GL_VERSION_2_1 && GLAD_GL_EXT_framebuffer_object) {
				loadExtensionOpenGLFramebufferPtrs();
				Log::info(kRunningOpenGLVersionText, "2.1 (GL_EXT_framebuffer_object)");
			} else {
				glfwDestroyWindow(window);
				setupGLES();
				window = createWindow(window_size, monitor_size);
				if (window != NULL && gladLoadGLES2Loader((GLADloadproc)glfwGetProcAddress) && GLAD_GL_ES_VERSION_2_0) {
					g_is_gles = true;
					loadDefaultOpenGLFramebufferPtrs();
					Log::info(kRunningOpenGLVersionText, "ES 2.0");
				} else {
					error = true;
					Log::info("<Desktop> OpenGL version too old. Engine requires either OpenGL 3.0, "
							"OpenGL 2.1 + GL_EXT_framebuffer_object or OpenGL ES 2.0");
				}
			}
		} else {
			error = true;
		}
#endif
		if(!error) {
			Log::info("<Desktop> Available OpenGL version: {}", (const char *)glGetString(GL_VERSION));
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
			int random_seed = time(0);
			Log::info("<Desktop> Got random seed from time(0): {}", random_seed);
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

bool OpenGL::isGLES() {
	return g_is_gles;
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
