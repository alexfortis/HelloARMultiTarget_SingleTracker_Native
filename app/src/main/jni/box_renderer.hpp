#ifndef BOX_RENDERER_HPP
#define BOX_RENDERER_HPP

#include "renderer.hpp"
#include "sprite.h"

namespace Smashing {

  class Box_Renderer() : public EasyAR::samples::Renderer {
  public:
    virtual void init();
    virtual void render(const Matrix44F &projectionMatrix,
                        const Matrix44F &cameraview,
                        Vec2F size,
                        sprite *sprites, size_t nsprites);
  private:
    int pos_coord_box;
    int pos_color_box;
    int pos_color_box_2;
    int pos_trans_box;
    int pos_proj_box;
    unsigned int vbo_coord_box;
    unsigned int vbo_color_box;
    unsigned int vbo_color_box_2;
    unsigned int vbo_faces_box;    
  }
  
}

#endif
