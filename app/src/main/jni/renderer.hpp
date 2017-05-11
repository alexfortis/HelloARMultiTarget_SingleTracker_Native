/**
* Copyright (c) 2015-2016 VisionStar Information Technology (Shanghai) Co., Ltd. All Rights Reserved.
* EasyAR is the registered trademark or trademark of VisionStar Information Technology (Shanghai) Co., Ltd in China
* and other countries for the augmented reality technology developed by VisionStar Information Technology (Shanghai) Co., Ltd.
*/

#ifndef __EASYAR_SAMPLE_UTILITY_SIMPLERENDERER_H__
#define __EASYAR_SAMPLE_UTILITY_SIMPLERENDERER_H__

#include "easyar/matrix.hpp"

namespace EasyAR{
namespace samples{

class Renderer
{
public:
    void init();
    void render(const Matrix44F& projectionMatrix, const Matrix44F& cameraview, Vec2F size);
private:
    unsigned int program_square;
    int pos_coord_square;
    int pos_color_square;
    int pos_trans_square;
    int pos_proj_square;
    unsigned int vbo_coord_square;
    unsigned int vbo_color_square;
    unsigned int vbo_color_square_2;
    unsigned int vbo_faces_square;
};

}
}
#endif
