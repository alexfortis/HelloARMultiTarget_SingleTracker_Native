#include "shader.hpp"

#include <cassert>

namespace Smashing {

  GLuint compile_shader(const GLenum type, const GLchar *source,
                        const GLint length) {
    assert(source != NULL);
    GLuint shader_obj_id = glCreateShader(type);
    GLint compile_status;
    assert(shader_obj_id != 0);
    
    glShaderSource(shader_obj_id, 1, (const GLchar **)&source, &length);
    glCompileShader(shader_obj_id);
    glGetShaderiv(shader_obj_id, GL_COMPILE_STATUS, &compile_status);
    assert(compile_status != 0);

    return shader_obj_id;
  }

  GLuint link_program(const GLuint vertex_shader, const GLuint fragment_shader) {
    GLuint program_obj_id = glCreateProgram();
    GLint link_status;
    assert(program_obj_id != 0);

    glAttachShader(program_obj_id, vertex_shader);
    glAttachShader(program_obj_id, fragment_shader);
    glLinkProgram(program_obj_id);
    glGetProgramiv(program_obj_id, GL_LINK_STATUS, &link_status);
    assert(link_status != 0);

    return program_obj_id;
  }

  GLuint build_program(const GLchar *vertex_shader_source,
                       const GLint vertex_shader_source_length,
                       const GLchar *fragment_shader_source,
                       const GLint fragment_shader_source_length) {
    GLuint vertex_shader =
      compile_shader(GL_VERTEX_SHADER,
                     vertex_shader_source,
                     vertex_shader_source_length);
    GLuint fragment_shader =
      compile_shader(GL_FRAGMENT_SHADER,
                     fragment_shader_source,
                     fragment_shader_source_length);
    return link_program(vertex_shader, fragment_shader);
  }

  
}
