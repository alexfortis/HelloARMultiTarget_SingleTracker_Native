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

const char* square_vert="uniform mat4 trans;\n"
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

const char* square_frag="#ifdef GL_ES\n"
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

void Renderer::init()
{
    // Setup shaders
    program_square = glCreateProgram();
    GLuint vertShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertShader, 1, &square_vert, 0);
    glCompileShader(vertShader);
    GLuint fragShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragShader, 1, &square_frag, 0);
    glCompileShader(fragShader);
    glAttachShader(program_square, vertShader);
    glAttachShader(program_square, fragShader);
    glLinkProgram(program_square);
    glUseProgram(program_square);

    // ???
    pos_coord_square = glGetAttribLocation(program_square, "coord");
    pos_color_square = glGetAttribLocation(program_square, "color");
    pos_trans_square = glGetUniformLocation(program_square, "trans");
    pos_proj_square = glGetUniformLocation(program_square, "proj");

    // Vertex coordinates
    glGenBuffers(1, &vbo_coord_square);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_coord_square);
    const GLfloat square_vertices[4][3] = {
      /* +z */ {1.0f / 2, 1.0f / 2, 0.01f / 2}, {1.0f / 2, -1.0f / 2, 0.01f / 2}, {-1.0f / 2, -1.0f / 2, 0.01f / 2}, {-1.0f / 2, 1.0f /2, 0.01f/2}
    }
    glBufferData(GL_ARRAY_BUFFER, sizeof(square_vertices), square_vertices, GL_DYNAMIC_DRAW);

    // Vertex colors
    glGenBuffers(1, &vbo_color_square);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_color_square);
    const GLubyte square_vertex_colors[4][4] = {
      {225, 0, 0, 128}, {0, 255, 0, 128}, {0, 0, 255, 128}, {0, 0, 0, 0, 128}
    }
    glBufferData(GL_ARRAY_BUFFER, sizeof(square_vertex_colors), square_vertex_colors, GL_STATIC_DRAW);

    /*
    glGenBuffers(1, &vbo_color_square_2);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_color_square_2);
    const GLubyte cube_vertex_colors_2[8][4] = {
        {255, 0, 0, 255}, {255, 255, 0, 255}, {0, 255, 0, 255}, {255, 0, 255, 255},
        {255, 0, 255, 255}, {255, 255, 255, 255}, {0, 255, 255, 255}, {255, 0, 255, 255}};
    const GLubyte pyramid_vertex_colors_2[8][4] = {
        {255, 0, 0, 255}, {255, 255, 0, 255}, {0, 255, 0, 255}, {255, 0, 255, 255},
        {255, 0, 255, 255}, {255, 255, 255, 255}, {0, 255, 255, 255}, {255, 0, 255, 255}};
    glBufferData(GL_ARRAY_BUFFER, sizeof(pyramid_vertex_colors_2), pyramid_vertex_colors_2, GL_STATIC_DRAW);
    */

    // Group vertices into faces
    glGenBuffers(1, &vbo_faces_square);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo_faces_square);
    const CLushort square_face[1][4] = {
      /* +z */ {3, 2, 1, 0}
    }
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(square_face), square_face, GL_STATIC_DRAW);
}

void Renderer::render(const Matrix44F& projectionMatrix, const Matrix44F& cameraview, Vec2F size)
{
    // setup shaders
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_DEPTH_TEST);
    glUseProgram(program_square);

    // Pass vertex coords attributes to shader
    glBindBuffer(GL_ARRAY_BUFFER, vbo_coord_square);
    glEnableVertexAttribArray(pos_coord_square);
    glVertexAttribPointer(pos_coord_square, 3, GL_FLOAT, GL_FALSE, 0, 0);
    // Pass vertex coords values to shader
    glBindBuffer(GL_ARRAY_BUFFER, vbo_coord_square);
    float height = size[0] / 1000;
    const GLfloat square_vertices[4][3] = {
      /* +z */ {size[0] / 2, size[1] / 2, height / 2}, {size[0] / 2, -size[1] / 2, height / 2}, {-size[0] / 2, -size[1] / 2, height / 2}, {-size[0] / 2, size[1] / 2, height / 2}
    }
    glBufferData(GL_ARRAY_BUFFER, sizeof(square_vertices), square_vertices, GL_DYNAMIC_DRAW);
    // Pass vertex colors to shader
    glBindBuffer(GL_ARRAY_BUFFER, vbo_color_square);
    glEnableVertexAttribArray(pos_color_square);
    glVertexAttribPointer(pos_color_square, 4, GL_UNSIGNED_BYTE, GL_TRUE, 0, 0);
    // Pass projection view matrices to get eye-view from world-model-view
    glUniformMatrix4fv(pos_trans_square, 1, 0, cameraview.data);
    glUniformMatrix4fv(pos_proj_square, 1, 0, projectionMatrix.data);
    // Render
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo_faces_square);
    for(int i = 0; i < 1; i++) {
        glDrawElements(GL_TRIANGLE_FAN, 4, GL_UNSIGNED_SHORT, (void*)(i * 4 * sizeof(GLushort)));
    }

    // Pass vertex coords attributes to shader
    glBindBuffer(GL_ARRAY_BUFFER, vbo_coord_square);
    glEnableVertexAttribArray(pos_coord_square);
    glVertexAttribPointer(pos_coord_square, 3, GL_FLOAT, GL_FALSE, 0, 0);
    // Pass vertex coords values to shader
    const GLfloat square_vertices[4][3] = {
      /* +z */ {size[0] / 4 + 3, size[1] / 4, height / 4}, {size[0] / 4 + 3, -size[1] / 4, height / 4}, {-size[0] / 4 + 3, -size[1] / 4, height / 4}, {-size[0] / 4 + 3, size[1] / 4, height / 4}
    }
    glBufferData(GL_ARRAY_BUFFER, sizeof(pyramid_vertices_2), pyramid_vertices_2, GL_DYNAMIC_DRAW);
    // Pass vertex color attributes to shader
    glBindBuffer(GL_ARRAY_BUFFER, vbo_color_square_2);
    glEnableVertexAttribArray(pos_color_square);
    glVertexAttribPointer(pos_color_square, 4, GL_UNSIGNED_BYTE, GL_TRUE, 0, 0);
    // Render
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo_faces_square);
    for(int i = 0; i < 1; i++) {
        glDrawElements(GL_TRIANGLE_FAN, 4, GL_UNSIGNED_SHORT, (void*)(i * 4 * sizeof(GLushort)));
    }
}

}
}
