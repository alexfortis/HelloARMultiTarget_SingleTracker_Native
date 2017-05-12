#include "asset_utils.hpp"

#include "filedata.hpp"
#include "rawimage.hpp"
#include "shader.hpp"
#include "texture.hpp"

#include <cassert>

namespace Smashing {

  GLuint load_png_asset_into_texture(const std::string &relative_path) {
    FileData png_file(relative_path);
    RawImage raw_image_data;
    raw_image_data.load_from_png(png_file.data(),
                                 png_file.data_length());
    Texture texture;
    texture.load_raw_image(raw_image_data.width(),
                           raw_image_data.height(),
                           raw_image_data.gl_color_format(),
                           raw_image_data.data());

    return texture.gl_texture();
  }
  
  GLuint build_program_from_assets(const std::string &vertex_shader_path,
                                   const std::string &fragment_shader_path) {
    FileData vertex_shader_source(vertex_shader_path);
    FileData fragment_shader_source(fragment_shader_path);
    const GLuint program =
      build_program(vertex_shader_source.data(),
                    vertex_shader_source.data_length(),
                    fragment_shader_source.data(),
                    fragment_shader_source.data_length());

    return program;
  }

}
