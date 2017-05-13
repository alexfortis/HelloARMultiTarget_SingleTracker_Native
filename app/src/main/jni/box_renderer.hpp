#ifndef BOX_RENDERER_HPP
#define BOX_RENDERER_HPP

#include "renderer.hpp"
#include "sprite.h"
#include <GLES3/gl31.h>

#include "easyar/matrix.hpp"

namespace Smashing {

  class Box_Renderer : public EasyAR::samples::Renderer {
  public:

    Box_Renderer();
    Box_Renderer(const std::string & vsp, const std::string & fsp);
    virtual ~Box_Renderer();
    virtual void init();
    virtual void render(const EasyAR::Matrix44F &projectionMatrix,
                        const EasyAR::Matrix44F &cameraview,
                        EasyAR::Vec2F size,
                        sprite *sprites, size_t nsprites);
    void rand_color();
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

    GLubyte colors_2[8][4];
  };
}

#endif
