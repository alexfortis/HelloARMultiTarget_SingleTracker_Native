#include "texture.hpp"

#include <cassert>

namespace Smashing {

  // Methods

  Texture::Texture() {
    gl_texture_ = 0;
  }


  Texture::~Texture() {

  }

  void Texture::load_raw_image(const GLsizei width, const GLsizei height,
                               const GLenum type, const GLvoid* pixels) {
    glGenTextures(1, &gl_texture_);
    assert(gl_texture_ != 0);

    glBindTexture(GL_TEXTURE_2D, gl_texture_);

    glTexParamteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParamteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D,
                 0, type, width, height,
                 0, type, GL_UNSIGNED_BYTE, pixels);
    glGenerateMipmap(GL_TEXTURE_2D);

    glBindTexture(GL_TEXTURE_2D, 0);
  }

  GLuint gl_texture(void) { return gl_texture_; }
  
}
