#include "rawimage.hpp"

#include <cassert>
#include <cstring>
#include <string>

#include "../../3rdparty/libpng/png.h"

namespace Smashing {
  
  typedef struct {
    const png_byte *data;
    const png_size_t size;
  } DataHandle;

  typedef struct {
    const DataHandle data;
    png_size_t offset;
  } ReadDataHandle;

  typedef struct {
    const png_uint_32 width;
    const png_uint_32 height;
    const int color_type;
  } PngInfo;

  // Helpers to read PNG

  /**
   * Callback called by libpng when reading from the memory buffer
   */
  static void read_png_data_callback(png_structp png_ptr,
                                     png_byte *png_data,
                                     png_size_t read_length) {
    ReadDataHandle *handle = (ReadDataHandle *)png_get_io_ptr(png_ptr);
    png_byte * png_src = const_cast<png_byte*>(handle->data.data) + handle->offset;

    //memcpy(png_data, png_src, read_length); // memcpy(dest, src, len)
    memcpy(png_src, png_data, read_length); // png_data is newly read data
    handle->offset += read_length;
  }

  /**
   * Reads in PNG data
   * Asks libpng to perform several transformations based on the PNG type:
   * - Transparency information is converted into a full alpha channel
   * - Grayscale images are converted to full RGB
   * - Paletted images are converted to full RGB
   * - RGB images get an alpha channel added if none is present
   * - Color channels are converted to 8-bit if less than 8-bit or 16-bit
   */
  static PngInfo read_and_update_info(const png_structp png_ptr,
                                      const png_infop info_ptr) {
    png_uint_32 width, height;
    int bit_depth, color_type;

    png_read_info(png_ptr, info_ptr);
    png_get_IHDR(png_ptr, info_ptr,
                 &width, &height,
                 &bit_depth, &color_type,
                 NULL, NULL, NULL);

    // Convert transparency to full alpha
    if (png_get_valid(png_ptr, info_ptr, PNG_INFO_tRNS)) {
      png_set_tRNS_to_alpha(png_ptr);
    }

    // Convert grayscale, if needed
    if (color_type == PNG_COLOR_TYPE_GRAY && bit_depth < 8) {
      png_set_expand_gray_1_2_4_to_8(png_ptr);
    }

    // Convert paletted images, if needed
    if (color_type == PNG_COLOR_TYPE_PALETTE) {
      png_set_palette_to_rgb(png_ptr);
    }

    // Add alpha channel, if there is none
    // Rationale: GL_RGBA is faster than GL_RGB on many GPUs
    if (color_type == PNG_COLOR_TYPE_PALETTE
        || color_type == PNG_COLOR_TYPE_RGB) {
      png_set_add_alpha(png_ptr, 0xFF, PNG_FILLER_AFTER);
    }

    // Ensure 8-bit packing
    if (bit_depth < 8) {
      png_set_packing(png_ptr);
    }
    else if (bit_depth == 16) {
      png_set_scale_16(png_ptr);
    }

    png_read_update_info(png_ptr, info_ptr);

    // Read the new color type after updates have been made
    color_type = png_get_color_type(png_ptr, info_ptr);

    return (PngInfo) { width, height, color_type };
  }

  /**
   * Decodes PNG image data into raw image data
   */
  static DataHandle read_entire_png_image(const png_structp png_ptr,
                                          const png_infop info_ptr,
                                          const png_uint_32 height) {
    // calculate raw image size
    const png_size_t row_size = png_get_rowbytes(png_ptr, info_ptr);
    const int data_length = row_size * height;
    assert(row_size > 0);

    // allocate memory for raw image
    png_byte *raw_image = new png_byte[data_length];
    assert(raw_image != NULL);

    // setup libpng to read image line by line
    png_byte* row_ptrs[height];
    for (png_uint_32 i = 0; i < height; i++) {
      row_ptrs[i] = raw_image + i * row_size;
    }
    png_read_image(png_ptr, &row_ptrs[0]);

    return (DataHandle) { raw_image, data_length };
  }

  /**
   * Convert PNG color format to OpenGL color format
   */
  static GLenum get_gl_color_format(const int png_color_format) {
    assert(png_color_format == PNG_COLOR_TYPE_GRAY
           || png_color_format == PNG_COLOR_TYPE_RGB_ALPHA
           || png_color_format == PNG_COLOR_TYPE_GRAY_ALPHA);

    switch (png_color_format) {
    case PNG_COLOR_TYPE_GRAY:
      return GL_LUMINANCE;
    case PNG_COLOR_TYPE_RGB_ALPHA:
      return GL_RGBA;
    case PNG_COLOR_TYPE_GRAY_ALPHA:
      return GL_LUMINANCE_ALPHA;
    }

    return 0;
  }

  // Methods

  RawImage::RawImage() {
    width_ = height_ = size_ = 0;
    gl_color_format_ = 0;
    data_ = nullptr;
  }

  RawImage::~RawImage() {
    assert(data_ != nullptr);
    delete data_;
  }

  void RawImage::load_from_png(const void *png_data, const int png_data_size) {
    assert(png_data != NULL && png_data_size > 8);
    assert(png_check_sig((void *)png_data, 8));

    // prepare to read PNG
    png_structp png_ptr =
      png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    assert(png_ptr != NULL);
    png_infop info_ptr = png_create_info_struct(png_ptr);
    assert(info_ptr != NULL);

    // read PNG, libpng will call callback for each part of the PNG
    ReadDataHandle png_data_handle = (ReadDataHandle) {
      { (const png_byte*)png_data, png_data_size }, 0
    };
    png_set_read_fn(png_ptr, &png_data_handle, read_png_data_callback);
    if (setjmp(png_jmpbuf(png_ptr))) {
      // libpng error handling: setjmp is true if error encountered
      __builtin_trap(); // 
    }

    // parse PNG metadata and convert PNG into a format we want
    const PngInfo png_info = read_and_update_info(png_ptr, info_ptr);
    const DataHandle raw_image =
      read_entire_png_image(png_ptr, info_ptr, png_info.height);

    // clean up libpng resources
    png_read_end(png_ptr, info_ptr);
    png_destroy_read_struct(&png_ptr, &info_ptr, NULL);

    // save raw image and metadata
    width_ = png_info.width;
    height_ = png_info.height;
    size_ = raw_image.size;
    gl_color_format_ = get_gl_color_format(png_info.color_type);
    data_ = (void*)const_cast<png_byte*>(raw_image.data);
  }

  int RawImage::width(void) { return width_; }
  int RawImage::height(void) { return height_; }
  int RawImage::size(void) { return size_; }
  GLenum RawImage::gl_color_format(void) { return gl_color_format_; }
  void *RawImage::data(void) { return data_; }
  
} // end namespace Smashing
