/**
 * Loading Texture from PNG:
 * http://www.learnopengles.com/loading-a-png-into-memory-and-displaying-it-as-a-texture-with-opengl-es-2-using-almost-the-same-code-on-ios-android-and-emscripten/
 */

#ifndef ASSET_UTILS_HPP
#define ASSET_UTILS_HPP

#include <string>

#if defined __APPLE__
#include <OpenGLES/ES2/gl.h>
#else
#include <GLES2/gl2.h>
#endif


namespace Smashing {

  GLuint load_png_asset_into_texture(const std::string &relative_path);
  
  GLuint build_program_from_assets(const std::string &vertex_shader_path,
                                   const std::string &fragment_shader_path);
}

#endif
