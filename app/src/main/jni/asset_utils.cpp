#include "asset_utils.hpp"

#include "filedata.hpp"
#include "rawimage.hpp"
#include "shader.hpp"
#include "texture.hpp"

#if defined __APPLE__
#include <OpenGLES/ES2/gl.h>
#else
#include <GLES2/gl2.h>
#endif

#ifdef ANDROID
#include <android/log.h>
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, "AssetUtil", __VA_ARGS__)
#define LOGW(...) __android_log_print(ANDROID_LOG_WARN, "AssetUtil", __VA_ARGS__)
#else
#define LOGI(...) printf(__VA_ARGS__)
#define LOGW(...) fprintf(stderr, __VA_ARGS__)
#endif

#include <cassert>

namespace Smashing {

  GLuint load_png_asset_into_texture(const std::string &relative_path) {
    FileData png_file(relative_path);
    LOGI("Load FileData complete\n");
    RawImage raw_image_data;
    raw_image_data.load_from_png(png_file.data(),
                                 png_file.data_length());
    LOGI("Load png complete\n");
    Texture texture;
    texture.load_raw_image(raw_image_data.width(),
                           raw_image_data.height(),
                           raw_image_data.gl_color_format(),
                           raw_image_data.data());
    LOGI("Load texture complete\n");
    return texture.gl_texture();
  }
  
  GLuint build_program_from_assets(const std::string &vertex_shader_path,
                                   const std::string &fragment_shader_path) {
    LOGI("Build program started\n");
    FileData vertex_shader_source(vertex_shader_path);
    LOGI("Got vertex shader from AssetManager\n");
    FileData fragment_shader_source(fragment_shader_path);
    LOGI("Got fragment shader from AssetManager\n");
    const GLuint program =
      build_program((const GLchar*) vertex_shader_source.data(),
                    vertex_shader_source.data_length(),
                    (const GLchar*) fragment_shader_source.data(),
                    fragment_shader_source.data_length());
    LOGI("Build program complete\n");
    return program;
  }

}
