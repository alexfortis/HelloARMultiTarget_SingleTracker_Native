/**
* Copyright (c) 2015-2016 VisionStar Information Technology (Shanghai) Co., Ltd. All Rights Reserved.
* EasyAR is the registered trademark or trademark of VisionStar Information Technology (Shanghai) Co., Ltd in China
* and other countries for the augmented reality technology developed by VisionStar Information Technology (Shanghai) Co., Ltd.
*/

#include "renderer.hpp"

#if defined __APPLE__
#include <OpenGLES/ES3/gl.h>
#else
#include <GLES3/gl31.h>
#endif

#ifdef ANDROID
#include <android/log.h>
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, "Renderer", __VA_ARGS__)
#define LOGW(...) __android_log_print(ANDROID_LOG_WARN, "Renderer", __VA_ARGS__)
#else
#define LOGI(...) printf(__VA_ARGS__)
#define LOGW(...) fprintf(stderr, __VA_ARGS__)
#endif

#include "asset_utils.hpp"

namespace EasyAR{
namespace samples{

Renderer::Renderer() { }

Renderer::Renderer(const std::string &vertex_shader_path,
                     const std::string &fragment_shader_path) {
    this->vertex_shader_path = vertex_shader_path;
    this->fragment_shader_path = fragment_shader_path;
}

Renderer::~Renderer() { }

void Renderer::init()
{
    LOGI("Renderer init started\n");

    // Setup shaders
    program =
      Smashing::build_program_from_assets(vertex_shader_path, fragment_shader_path);
    glUseProgram(program);

    LOGI("Renderer init completed\n");
}

}
}
