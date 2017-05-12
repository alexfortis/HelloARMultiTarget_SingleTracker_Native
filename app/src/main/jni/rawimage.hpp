/**
 * Loading Texture from PNG:
 * http://www.learnopengles.com/loading-a-png-into-memory-and-displaying-it-as-a-texture-with-opengl-es-2-using-almost-the-same-code-on-ios-android-and-emscripten/
 */

#ifndef RAWIMAGE_HPP
#define RAWIMAGE_HPP
/* Uncomment if compiler complains about GLenum
#if defined __APPLE__
#include <OpenGLES/ES2/gl.h>
#else
#include <GLES2/gl2.h>
#endif
*/
namespace Smashing {

  class RawImage {
  public:
    RawImage();
    void load_from_png(const void *png_data, const int png_data_size);
    int width(void);
    int height(void);
    int size(void);
    GLenum gl_color_format(void);
    void *data(void);
    ~RawImage();
  private:
    int width_;
    int height_;
    int size_;
    GLenum gl_color_format_;
    void *data_;
  }; // end class RawImage
  
} // end namespace Smashing

#endif
