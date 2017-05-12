/**
 * Loading Texture from PNG:
 * http://www.learnopengles.com/loading-a-png-into-memory-and-displaying-it-as-a-texture-with-opengl-es-2-using-almost-the-same-code-on-ios-android-and-emscripten/
 */

#ifndef FILEDATA_HPP
#define FILEDATA_HPP

#include <string>

namespace Smashing {

  class FileData {
  public:
    FileData(const std::string &path);
    const void *data(void);
    long data_length(void);
    ~FileData();
  private:
    long data_length_;
    const void *data_;
    void *file_handle_;
  }; // end class FileData
  
} // end namespace Smashing

#endif
