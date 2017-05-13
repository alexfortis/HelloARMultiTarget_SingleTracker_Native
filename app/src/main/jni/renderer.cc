/**
* Copyright (c) 2015-2016 VisionStar Information Technology (Shanghai) Co., Ltd. All Rights Reserved.
* EasyAR is the registered trademark or trademark of VisionStar Information Technology (Shanghai) Co., Ltd in China
* and other countries for the augmented reality technology developed by VisionStar Information Technology (Shanghai) Co., Ltd.
*/

#include "renderer.hpp"

#if defined __APPLE__
#include <OpenGLES/ES2/gl.h>
#else
#include <GLES2/gl2.h>
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
#include "shader.hpp"
#include "sprite.h"

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
    // ??? semantic binding?
    pos_coord = glGetAttribLocation(program, "coord");
    pos_tex_coord = glGetAttribLocation(program, "tex_coord");
    pos_tex_unit = glGetUniformLocation(program, "tex_unit");
    pos_trans = glGetUniformLocation(program, "trans");
    pos_proj = glGetUniformLocation(program, "proj");
    // Create VBOs
    glGenBuffers(1, &vbo_coord);
    glGenBuffers(1, &vbo_tex_coord);

    LOGI("Renderer init completed\n");
}

void Renderer::render(const Matrix44F& projectionMatrix,
                      const Matrix44F& cameraview,
                      Vec2F size, sprite *sprites, size_t nsprites) {
    LOGI("render start\n");

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    // setup shaders
    glUseProgram(program);

    float height = size[0] / 1000;
    for (size_t i = 0; i < nsprites; i++) {
      LOGI("render sprite %zu start\n", i);
      // Pass texture to shader
      glActiveTexture(GL_TEXTURE0);
      GLuint texture = Smashing::load_png_asset_into_texture(sprites[i].imgPath);
      LOGI("loaded png into texture\n");
      glBindTexture(GL_TEXTURE_2D, texture);
      glUniform1i(pos_tex_unit, 0);
      LOGI("pass texture to shader complete\n");
      // Pass vertex coords attributes to shader
      glBindBuffer(GL_ARRAY_BUFFER, vbo_coord);
      glEnableVertexAttribArray(pos_coord);
      glVertexAttribPointer(pos_coord, 3, GL_FLOAT, GL_FALSE, 0, 0);
      LOGI("pass vertex coords attributes to shader complete\n");
      // Pass vertex coords values to shader
      glBindBuffer(GL_ARRAY_BUFFER, vbo_coord);
      const GLfloat coords[4][3] = {
        /* +z */
        {size[0] / 2, size[1] / 2, height / 2},
        {size[0] / 2, -size[1] / 2, height / 2},
        {-size[0] / 2, -size[1] / 2, height / 2},
        {-size[0] / 2, size[1] / 2, height / 2}
      };
      glBufferData(GL_ARRAY_BUFFER, sizeof(coords), coords, GL_DYNAMIC_DRAW);
      LOGI("pass vertex coords values to shader complete\n");
      // Pass texture coords attributes to shader
      glBindBuffer(GL_ARRAY_BUFFER, vbo_tex_coord);
      glEnableVertexAttribArray(pos_tex_coord);
      glVertexAttribPointer(pos_tex_coord, 2, GL_FLOAT, GL_FALSE, 0, 0);
      LOGI("pass texture coords attributes to shader complete\n");
      // Pass texture coords values to shader
      glBindBuffer(GL_ARRAY_BUFFER, vbo_tex_coord);
      const GLfloat tex_coords[4][2] = {
        //{1.0f, 1.0f}, {1.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 1.0f} // upside down
        //{1.0f, 1.0f}, {0.0f, 1.0f}, {0.0f, 0.0f}, {1.0f, 0.0f} // turned 270
        {1.0f, 0.0f} ,{1.0f, 1.0f}, {0.0f, 1.0f}, {0.0f, 0.0f}
      };
      glBufferData(GL_ARRAY_BUFFER, sizeof(tex_coords), tex_coords, GL_DYNAMIC_DRAW);
      LOGI("pass texture coords values to shader complete\n");
      // Pass projection view matrices to get eye-view from world-model-view
      glUniformMatrix4fv(pos_trans, 1, 0, cameraview.data);
      glUniformMatrix4fv(pos_proj, 1, 0, projectionMatrix.data);
      LOGI("pass matrices to shader complete\n");
      // Render sprite
      glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
      LOGI("render sprite %zu complete\n", i);
      // Release buffer binding
      glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    LOGI("render done\n");
}

}
}
