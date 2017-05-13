#ifndef PNG_RENDERER_HPP
#define PNG_RENDERER_HPP

#include "renderer.hpp"
#include "sprite.h"

namespace Smashing {

  class PNG_Renderer() : public EasyAR::samples::Renderer {
  public:
    virtual void init();
    virtual void render(const Matrix44F &projectionMatrix,
                        const Matrix44F &cameraview,
                        Vec2F size,
                        sprite *sprites, size_t nsprites);
  private:
    int pos_coord;
    int pos_tex_coord;
    int pos_tex_unit;
    int pos_trans;
    int pos_proj;
    unsigned int vbo_coord;
    unsigned int vbo_tex_coord;    
  }
  
}

#endif
