#include "box_renderer.hpp"

#include <cstdlib>
#include <ctime>

#if defined __APPLE__
#include <OpenGLES/ES3/gl.h>
#else
#include <GLES3/gl31.h>
#endif

#ifdef ANDROID
#include <android/log.h>
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, "Box_Renderer", __VA_ARGS__)
#define LOGW(...) __android_log_print(ANDROID_LOG_WARN, "Box_Renderer", __VA_ARGS__)
#else
#define LOGI(...) printf(__VA_ARGS__)
#define LOGW(...) fprintf(stderr, __VA_ARGS__)
#endif

namespace Smashing {

  Box_Renderer::Box_Renderer() {
    this->vertex_shader_path = "shaders/square.vsh";
    this->fragment_shader_path = "shaders/square.fsh";
    colors_2[0][0] = colors_2[0][1] = colors_2[0][2] = 0; colors_2[0][3] = 255;
    colors_2[1][0] = colors_2[1][1] = colors_2[1][2] = 0; colors_2[1][3] = 255;
    colors_2[2][0] = colors_2[2][1] = colors_2[2][2] = 0; colors_2[2][3] = 255;
    colors_2[3][0] = colors_2[3][1] = colors_2[3][2] = 0; colors_2[3][3] = 255;
    colors_2[4][0] = colors_2[4][1] = colors_2[4][2] = 0; colors_2[4][3] = 255;
    colors_2[5][0] = colors_2[5][1] = colors_2[5][2] = 0; colors_2[5][3] = 255;
    colors_2[6][0] = colors_2[6][1] = colors_2[6][2] = 0; colors_2[6][3] = 255;
    colors_2[7][0] = colors_2[7][1] = colors_2[7][2] = 0; colors_2[7][3] = 255;
  }

  Box_Renderer::Box_Renderer(const std::string &vsp, const std::string &fsp) : Box_Renderer()  {
    this->vertex_shader_path = vsp;
    this->fragment_shader_path = fsp;
  }

  Box_Renderer::~Box_Renderer() {}

  void Box_Renderer::init() {
    EasyAR::samples::Renderer::init(); // call base class init

    // ??? semantic binding?
    pos_coord_box = glGetAttribLocation(program, "coord");
    pos_color_box = glGetAttribLocation(program, "color");
    pos_trans_box = glGetUniformLocation(program, "trans");
    pos_proj_box = glGetUniformLocation(program, "proj");
    // Create VBOs
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
        {0, 0, 0, 255}, {0, 0, 0, 255}, {0, 0, 0, 255}, {0, 0, 0, 255},
        {0, 0, 0, 255}, {0, 0, 0, 255}, {0, 0, 0, 255}, {0, 0, 0, 255}};
    glBufferData(GL_ARRAY_BUFFER, sizeof(colors_2), colors_2, GL_STATIC_DRAW);

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

  void Box_Renderer::render(const EasyAR::Matrix44F &projectionMatrix,
                            const EasyAR::Matrix44F &cameraview,
                            EasyAR::Vec2F size,
                            sprite *sprites, size_t nsprites) {
   /* if(!printed) {
        printed = true;
        LOGI("size[0]=%.2f, size[1]=%.2f\n", size[0], size[1]);
    }*/
    EasyAR::Vec2F sprite_size;
    sprite_size[0] = size[0]/5;
    sprite_size[1] = size[1]/5;

    float height = size[0] / 1000;
    for(size_t i = 0; i < nsprites; i++) {
      sprite ant = sprites[i];
      switch(ant.state) {
        case sprite::SpriteState::ALIVE:
          {
          glBindBuffer(GL_ARRAY_BUFFER, vbo_coord_box);
          float base = size[1];
          //float height = base + 2.0f;

          const GLfloat cube_vertices[8][3] = {
              /* +z */{size[0] / 2, size[1] / 2, height / 2}, {size[0] / 2, -size[1] / 2, height / 2}, {-size[0] / 2, -size[1] / 2, height / 2}, {-size[0] / 2, size[1] / 2, height / 2},
              /* -z */{size[0] / 2, size[1] / 2, 0}, {size[0] / 2, -size[1] / 2, 0}, {-size[0] / 2, -size[1] / 2, 0}, {-size[0] / 2, size[1] / 2, 0}};


          glBufferData(GL_ARRAY_BUFFER, sizeof(cube_vertices), cube_vertices, GL_DYNAMIC_DRAW);

          glEnable(GL_BLEND);
          glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
          glEnable(GL_DEPTH_TEST);
          glUseProgram(program);

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


          //const GLfloat cube_vertices_2[8][3] = {
            //  /* +z */{sprite_size[0] / 4 + ant.x, sprite_size[1] / 4 + ant.y, height},{sprite_size[0] / 4 + ant.x, -sprite_size[1] / 4 + ant.y, height},{-sprite_size[0] / 4 + ant.x, -sprite_size[1] / 4 + ant.y, height},{-sprite_size[0] / 4 + ant.x, sprite_size[1] / 4 + ant.y, height},
            //  /* -z */{sprite_size[0] / 4 + ant.x, sprite_size[1] / 4 + ant.y, base},{sprite_size[0] / 4 + ant.x, -sprite_size[1] / 4 + ant.y, base},{-sprite_size[0] / 4 + ant.x, -sprite_size[1] / 4 + ant.y, base},{-sprite_size[0] / 4 + ant.x, sprite_size[1] / 4 + ant.y, base}};

          glBufferData(GL_ARRAY_BUFFER, sizeof(cube_vertices_2), cube_vertices_2, GL_DYNAMIC_DRAW);
          glEnableVertexAttribArray(pos_coord_box);
          glVertexAttribPointer(pos_coord_box, 3, GL_FLOAT, GL_FALSE, 0, 0);
          glBindBuffer(GL_ARRAY_BUFFER, vbo_color_box_2);
          glBufferData(GL_ARRAY_BUFFER, sizeof(colors_2), colors_2, GL_DYNAMIC_DRAW);
          glEnableVertexAttribArray(pos_color_box);
          glVertexAttribPointer(pos_color_box, 4, GL_UNSIGNED_BYTE, GL_TRUE, 0, 0);
          for(int i = 0; i < 6; i++) {
              glDrawElements(GL_TRIANGLE_FAN, 4, GL_UNSIGNED_SHORT, (void*)(i * 4 * sizeof(GLushort)));
          }
          break;
          }
        case sprite::SpriteState::SMASHED:
        {
          //flat
          glBindBuffer(GL_ARRAY_BUFFER, vbo_coord_box);
                    float base = size[1];
                    //float height = base + 2.0f;

                    const GLfloat cube_vertices[8][3] = {
                        /* +z */{size[0] / 2, size[1] / 2, height / 2}, {size[0] / 2, -size[1] / 2, height / 2}, {-size[0] / 2, -size[1] / 2, height / 2}, {-size[0] / 2, size[1] / 2, height / 2},
                        /* -z */{size[0] / 2, size[1] / 2, 0}, {size[0] / 2, -size[1] / 2, 0}, {-size[0] / 2, -size[1] / 2, 0}, {-size[0] / 2, size[1] / 2, 0}};


                    glBufferData(GL_ARRAY_BUFFER, sizeof(cube_vertices), cube_vertices, GL_DYNAMIC_DRAW);

                    glEnable(GL_BLEND);
                    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
                    glEnable(GL_DEPTH_TEST);
                    glUseProgram(program);

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
                        /* +z */{sprite_size[0] / 4 + ant.x, sprite_size[1] / 4 + ant.y, 3*height},{sprite_size[0] / 4 + ant.x, -sprite_size[1] / 4 + ant.y, 3*height},{-sprite_size[0] / 4 + ant.x, -sprite_size[1] / 4 + ant.y, 3*height},{-sprite_size[0] / 4 + ant.x, sprite_size[1] / 4 + ant.y, 3*height},
                        /* -z */{sprite_size[0] / 4 + ant.x, sprite_size[1] / 4 + ant.y, 0},{sprite_size[0] / 4 + ant.x, -sprite_size[1] / 4 + ant.y, 0},{-sprite_size[0] / 4 + ant.x, -sprite_size[1] / 4 + ant.y, 0},{-sprite_size[0] / 4 + ant.x, sprite_size[1] / 4 + ant.y, 0}};


                    //const GLfloat cube_vertices_2[8][3] = {
                      //  /* +z */{sprite_size[0] / 4 + ant.x, sprite_size[1] / 4 + ant.y, height},{sprite_size[0] / 4 + ant.x, -sprite_size[1] / 4 + ant.y, height},{-sprite_size[0] / 4 + ant.x, -sprite_size[1] / 4 + ant.y, height},{-sprite_size[0] / 4 + ant.x, sprite_size[1] / 4 + ant.y, height},
                      //  /* -z */{sprite_size[0] / 4 + ant.x, sprite_size[1] / 4 + ant.y, base},{sprite_size[0] / 4 + ant.x, -sprite_size[1] / 4 + ant.y, base},{-sprite_size[0] / 4 + ant.x, -sprite_size[1] / 4 + ant.y, base},{-sprite_size[0] / 4 + ant.x, sprite_size[1] / 4 + ant.y, base}};

                    glBufferData(GL_ARRAY_BUFFER, sizeof(cube_vertices_2), cube_vertices_2, GL_DYNAMIC_DRAW);
                    glEnableVertexAttribArray(pos_coord_box);
                    glVertexAttribPointer(pos_coord_box, 3, GL_FLOAT, GL_FALSE, 0, 0);
                    glBindBuffer(GL_ARRAY_BUFFER, vbo_color_box_2);
                    glBufferData(GL_ARRAY_BUFFER, sizeof(colors_2), colors_2, GL_DYNAMIC_DRAW);
                    glEnableVertexAttribArray(pos_color_box);
                    glVertexAttribPointer(pos_color_box, 4, GL_UNSIGNED_BYTE, GL_TRUE, 0, 0);
                    for(int i = 0; i < 6; i++) {
                        glDrawElements(GL_TRIANGLE_FAN, 4, GL_UNSIGNED_SHORT, (void*)(i * 4 * sizeof(GLushort)));
                    }
                    break;
                  }
        default: continue;
      }



    }
  }
        void Box_Renderer::rand_color() {
          for (int i = 0; i < 3; ++i) {
                colors_2[0][i] = (float)rand()/RAND_MAX*255; colors_2[0][3] = 255;
                colors_2[1][i] = (float)rand()/RAND_MAX*255; colors_2[1][3] = 255;
                colors_2[2][i] = (float)rand()/RAND_MAX*255; colors_2[2][3] = 255;
                colors_2[3][i] = (float)rand()/RAND_MAX*255; colors_2[3][3] = 255;
                colors_2[4][i] = (float)rand()/RAND_MAX*255; colors_2[4][3] = 255;
                colors_2[5][i] = (float)rand()/RAND_MAX*255; colors_2[5][3] = 255;
                colors_2[6][i] = (float)rand()/RAND_MAX*255; colors_2[6][3] = 255;
                colors_2[7][i] = (float)rand()/RAND_MAX*255; colors_2[7][3] = 255;
           }
                //LOGI("New color value: %d", colors_2[0][2]);
        }
}
