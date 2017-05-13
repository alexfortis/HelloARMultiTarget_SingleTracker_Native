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
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, "EasyAR", __VA_ARGS__)
#else
#define LOGI(...) printf(__VA_ARGS__)
#endif

const char* box_vert="uniform mat4 trans;\n"
        "uniform mat4 proj;\n"
        "attribute vec4 coord;\n"
        "attribute vec4 color;\n"
        "varying vec4 vcolor;\n"
        "\n"
        "void main(void)\n"
        "{\n"
        "    vcolor = color;\n"
        "    gl_Position = proj*trans*coord;\n"
        "}\n"
        "\n"
;

const char* box_frag="#ifdef GL_ES\n"
        "precision highp float;\n"
        "#endif\n"
        "varying vec4 vcolor;\n"
        "\n"
        "void main(void)\n"
        "{\n"
        "    gl_FragColor = vcolor;\n"
        "}\n"
        "\n"
;

namespace EasyAR{
namespace samples{

static bool printed = false;

void Renderer::init()
{
    program_box = glCreateProgram();
    GLuint vertShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertShader, 1, &box_vert, 0);
    glCompileShader(vertShader);
    GLuint fragShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragShader, 1, &box_frag, 0);
    glCompileShader(fragShader);
    glAttachShader(program_box, vertShader);
    glAttachShader(program_box, fragShader);
    glLinkProgram(program_box);
    glUseProgram(program_box);
    pos_coord_box = glGetAttribLocation(program_box, "coord");
    pos_color_box = glGetAttribLocation(program_box, "color");
    pos_trans_box = glGetUniformLocation(program_box, "trans");
    pos_proj_box = glGetUniformLocation(program_box, "proj");

    glGenBuffers(1, &vbo_coord_box);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_coord_box);
    const GLfloat cube_vertices[8][3] = {
        /* +z */{1.0f / 2, 1.0f / 2, 0.01f / 2}, {1.0f / 2, -1.0f / 2, 0.01f / 2}, {-1.0f / 2, -1.0f / 2, 0.01f / 2}, {-1.0f / 2, 1.0f / 2, 0.01f / 2},
        /* -z */{1.0f / 2, 1.0f / 2, -0.01f / 2}, {1.0f / 2, -1.0f / 2, -0.01f / 2}, {-1.0f / 2, -1.0f / 2, -0.01f / 2}, {-1.0f / 2, 1.0f / 2, -0.01f / 2}};
    const GLfloat pyramid_vertices[8][3] = {
        /* +z */{1.0f , 1.0f , 0.01f}, {1.0f, -1.0f, 0.01f}, {-1.0f, -1.0f, 0.01f}, {-1.0f, 1.0f, 0.01f},
        /* -z */{1.0f / 2, 1.0f / 2, -0.01f / 2}, {1.0f / 2, -1.0f / 2, -0.01f / 2}, {-1.0f / 2, -1.0f / 2, -0.01f / 2}, {-1.0f / 2, 1.0f / 2, -0.01f / 2}};
    glBufferData(GL_ARRAY_BUFFER, sizeof(pyramid_vertices), pyramid_vertices, GL_DYNAMIC_DRAW);

    glGenBuffers(1, &vbo_color_box);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_color_box);
    const GLubyte cube_vertex_colors[8][4] = {
        {255, 0, 0, 128}, {0, 255, 0, 128}, {0, 0, 255, 128}, {0, 0, 0, 128},
        {0, 255, 255, 128}, {255, 0, 255, 128}, {255, 255, 0, 128}, {255, 255, 255, 128}};
    const GLubyte pyramid_vertex_colors[8][4] = {
        {255, 0, 0, 128}, {0, 255, 0, 128}, {0, 0, 255, 128}, {0, 0, 0, 128},
        {0, 255, 255, 128}, {255, 0, 255, 128}, {255, 255, 0, 128}, {255, 255, 255, 128}};
    glBufferData(GL_ARRAY_BUFFER, sizeof(pyramid_vertex_colors), pyramid_vertex_colors, GL_STATIC_DRAW);

    glGenBuffers(1, &vbo_color_box_2);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_color_box_2);
    const GLubyte cube_vertex_colors_2[8][4] = {
        {255, 0, 0, 255}, {255, 255, 0, 255}, {0, 255, 0, 255}, {255, 0, 255, 255},
        {255, 0, 255, 255}, {255, 255, 255, 255}, {0, 255, 255, 255}, {255, 0, 255, 255}};
    const GLubyte pyramid_vertex_colors_2[8][4] = {
        {255, 0, 0, 255}, {255, 255, 0, 255}, {0, 255, 0, 255}, {255, 0, 255, 255},
        {255, 0, 255, 255}, {255, 255, 255, 255}, {0, 255, 255, 255}, {255, 0, 255, 255}};
    glBufferData(GL_ARRAY_BUFFER, sizeof(pyramid_vertex_colors_2), pyramid_vertex_colors_2, GL_STATIC_DRAW);

    glGenBuffers(1, &vbo_faces_box);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo_faces_box);
    const GLushort cube_faces[6][4] = {
        /* +z */{3, 2, 1, 0}, /* -y */{2, 3, 7, 6}, /* +y */{0, 1, 5, 4},
        /* -x */{3, 0, 4, 7}, /* +x */{1, 2, 6, 5}, /* -z */{4, 5, 6, 7}};
    const GLushort pyramid_faces[6][4] = {
        /* +z */{3, 2, 1, 0}, /* -y */{2, 3, 7, 6}, /* +y */{0, 1, 5, 4},
        /* -x */{3, 0, 4, 7}, /* +x */{1, 2, 6, 5}, /* -z */{4, 5, 6, 7}};
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(pyramid_faces), cube_faces, GL_STATIC_DRAW);
}

void Renderer::render(const Matrix44F& projectionMatrix, const Matrix44F& cameraview, Vec2F size, sprite *sprites, size_t n_sprites)
{
    if(!printed) {
        printed = true;
        LOGI("size[0]=%.2f, size[1]=%.2f\n", size[0], size[1]);
    }
    Vec2F sprite_size;
    sprite_size[0] = size[0]/5;
    sprite_size[1] = size[1]/5;
    for(size_t i = 0; i < n_sprites; i++) {
        sprite ant = sprites[i];

        glBindBuffer(GL_ARRAY_BUFFER, vbo_coord_box);
        float height = size[0] / 1000;
        const GLfloat cube_vertices[8][3] = {
            /* +z */{size[0] / 2, size[1] / 2, height / 2}, {size[0] / 2, -size[1] / 2, height / 2}, {-size[0] / 2, -size[1] / 2, height / 2}, {-size[0] / 2, size[1] / 2, height / 2},
            /* -z */{size[0] / 2, size[1] / 2, 0}, {size[0] / 2, -size[1] / 2, 0}, {-size[0] / 2, -size[1] / 2, 0}, {-size[0] / 2, size[1] / 2, 0}};
        const GLfloat pyramid_vertices[8][3] = {
            /* +z */{sprite_size[0]+ant.x, sprite_size[1]+ant.y, height}, {sprite_size[0]+ant.x, -sprite_size[1]+ant.y, height}, {-sprite_size[0]+ant.x, -sprite_size[1]+ant.y, height}, {-sprite_size[0]+ant.x, sprite_size[1]+ant.y, height},
            /* -z */{(sprite_size[0]+1) / 2+ant.x, sprite_size[1] / 2+ant.y, 0}, {(sprite_size[0]) / 2 + ant.x, -sprite_size[1] / 2 + ant.y, 0}, {(-sprite_size[0]) / 2 + ant.x, -sprite_size[1] / 2 + ant.y, 0}, {(-sprite_size[0]) / 2 + ant.x, sprite_size[1] / 2 + ant.y, 0}};
        glBufferData(GL_ARRAY_BUFFER, sizeof(cube_vertices), cube_vertices, GL_DYNAMIC_DRAW);

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glEnable(GL_DEPTH_TEST);
        glUseProgram(program_box);
        glBindBuffer(GL_ARRAY_BUFFER, vbo_coord_box);
        glEnableVertexAttribArray(pos_coord_box);
        glVertexAttribPointer(pos_coord_box, 3, GL_FLOAT, GL_FALSE, 0, 0);
        glBindBuffer(GL_ARRAY_BUFFER, vbo_color_box);
        glEnableVertexAttribArray(pos_color_box);
        glVertexAttribPointer(pos_color_box, 4, GL_UNSIGNED_BYTE, GL_TRUE, 0, 0);
        glUniformMatrix4fv(pos_trans_box, 1, 0, cameraview.data);
        glUniformMatrix4fv(pos_proj_box, 1, 0, projectionMatrix.data);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo_faces_box);
        for(int i = 0; i < 6; i++) {
            glDrawElements(GL_TRIANGLE_FAN, 4, GL_UNSIGNED_SHORT, (void*)(i * 4 * sizeof(GLushort)));
        }

        glBindBuffer(GL_ARRAY_BUFFER, vbo_coord_box);
        const GLfloat cube_vertices_2[8][3] = {
            /* +z */{sprite_size[0] / 4 + ant.x, sprite_size[1] / 4 + ant.y, sprite_size[0] / 4},{sprite_size[0] / 4 + ant.x, -sprite_size[1] / 4 + ant.y, sprite_size[0] / 4},{-sprite_size[0] / 4 + ant.x, -sprite_size[1] / 4 + ant.y, sprite_size[0] / 4},{-sprite_size[0] / 4 + ant.x, sprite_size[1] / 4 + ant.y, sprite_size[0] / 4},
            /* -z */{sprite_size[0] / 4 + ant.x, sprite_size[1] / 4 + ant.y, 0},{sprite_size[0] / 4 + ant.x, -sprite_size[1] / 4 + ant.y, 0},{-sprite_size[0] / 4 + ant.x, -sprite_size[1] / 4 + ant.y, 0},{-sprite_size[0] / 4 + ant.x, sprite_size[1] / 4 + ant.y, 0}};
        const GLfloat pyramid_vertices_2[8][3] = {
            /* +z */{sprite_size[0] / 2+ant.x, sprite_size[1] / 2+ant.y, sprite_size[0] / 2},{sprite_size[0] / 2 + ant.x, -sprite_size[1] / 2+ant.y, sprite_size[0] / 2},{-sprite_size[0] / 2 + ant.x, -sprite_size[1] / 2+ant.y, sprite_size[0] / 2},{-sprite_size[0] / 2 + ant.x, sprite_size[1] / 2+ant.y, sprite_size[0] / 2},
            /* -z */{sprite_size[0] / 4+ant.x, sprite_size[1] / 4+ant.y, 0},{sprite_size[0] / 4 + ant.x, -sprite_size[1] / 4+ant.y, 0},{-sprite_size[0] / 4 + ant.x, -sprite_size[1] / 4+ant.y, 0},{-sprite_size[0] / 4 + ant.x, sprite_size[1] / 4+ant.y, 0}};
        glBufferData(GL_ARRAY_BUFFER, sizeof(cube_vertices_2), cube_vertices_2, GL_DYNAMIC_DRAW);
        glEnableVertexAttribArray(pos_coord_box);
        glVertexAttribPointer(pos_coord_box, 3, GL_FLOAT, GL_FALSE, 0, 0);
        glBindBuffer(GL_ARRAY_BUFFER, vbo_color_box_2);
        glEnableVertexAttribArray(pos_color_box);
        glVertexAttribPointer(pos_color_box, 4, GL_UNSIGNED_BYTE, GL_TRUE, 0, 0);
        for(int i = 0; i < 6; i++) {
            glDrawElements(GL_TRIANGLE_FAN, 4, GL_UNSIGNED_SHORT, (void*)(i * 4 * sizeof(GLushort)));
        }
    }
}

}
}
