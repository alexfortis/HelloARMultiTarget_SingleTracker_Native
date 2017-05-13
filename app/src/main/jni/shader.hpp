/**
 * Loading Texture from PNG:
 * http://www.learnopengles.com/loading-a-png-into-memory-and-displaying-it-as-a-texture-with-opengl-es-2-using-almost-the-same-code-on-ios-android-and-emscripten/
 */

#ifndef SHADER_HPP
#define SHADER_HPP

#if defined __APPLE__
#include <OpenGLES/ES3/gl.h>
#else
#include <GLES3/gl31.h>
#endif

namespace Smashing {

  /**
   * Utilities to help compile, link, and build shader programs
   */
  
  GLuint compile_shader(const GLenum type, const GLchar *source,
                        const GLint length);

  GLuint link_program(const GLuint vertex_shader, const GLuint fragment_shader);

  GLuint build_program(const GLchar *vertex_shader_source,
                       const GLint vertex_shader_source_length,
                       const GLchar *fragment_shader_source,
                       const GLint fragment_shader_source_length);
}

#endif
