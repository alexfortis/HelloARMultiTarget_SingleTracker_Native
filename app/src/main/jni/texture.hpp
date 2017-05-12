/**
 * Loading Texture from PNG:
 * http://www.learnopengles.com/loading-a-png-into-memory-and-displaying-it-as-a-texture-with-opengl-es-2-using-almost-the-same-code-on-ios-android-and-emscripten/
 */

#ifndef TEXTURE_HPP
#define TEXTURE_HPP
/* Uncomment if compiler complains about GLenum
#if defined __APPLE__
#include <OpenGLES/ES2/gl.h>
#else
#include <GLES2/gl2.h>
#endif
*/
namespace Smashing {

  class Texture {
  public:
    Texture();
    /**
     * Loads in raw data into the texture assuming each component is 8-bit.
     * Then sets up the texture for trilinear mipmapping.
     */
    void load_raw_image(const GLsizei width, const GLsizei height,
                        const GLenum type, const GLvoid* pixels);
    GLuint gl_texture(void);
    ~Texture();
  private:
    GLuint gl_texture_;
  }; // end class Texture
  
} // end namespace Smashing

#endif
