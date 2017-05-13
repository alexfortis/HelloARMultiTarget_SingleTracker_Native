/**
* Copyright (c) 2015-2016 VisionStar Information Technology (Shanghai) Co., Ltd. All Rights Reserved.
* EasyAR is the registered trademark or trademark of VisionStar Information Technology (Shanghai) Co., Ltd in China
* and other countries for the augmented reality technology developed by VisionStar Information Technology (Shanghai) Co., Ltd.
*/

#ifndef __EASYAR_SAMPLE_UTILITY_SIMPLERENDERER_H__
#define __EASYAR_SAMPLE_UTILITY_SIMPLERENDERER_H__

#include "sprite.h"
#include "asset_utils.hpp"

#include "easyar/matrix.hpp"

#include <string>

namespace EasyAR{
namespace samples{

class Renderer
{
public:
    Renderer();
    Renderer(const std::string &vertex_shader_path,
             const std::string &fragment_shader_path);
    virtual void init();
    virtual void render(const Matrix44F& projectionMatrix,
                const Matrix44F& cameraview,
                Vec2F size,
                sprite *sprites, size_t nsprites) = 0;
    virtual ~Renderer();
protected:
    std::string vertex_shader_path;
    std::string fragment_shader_path;
    unsigned int program;
};

}
}
#endif
