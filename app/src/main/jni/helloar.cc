/**
 * Copyright (c) 2015-2016 VisionStar Information Technology (Shanghai) Co., Ltd. All Rights Reserved.
 * EasyAR is the registered trademark or trademark of VisionStar Information Technology (Shanghai) Co., Ltd in China
 * and other countries for the augmented reality technology developed by VisionStar Information Technology (Shanghai) Co., Ltd.
 */

#include "ar.hpp"
#include "renderer.hpp"
#include "box_renderer.hpp"
#include "png_renderer.hpp"
#include "sprite.h"
#include <jni.h>
#include <GLES3/gl31.h>
#include <cmath>
#include <climits>
#include <time.h>


#ifdef ANDROID
#include <android/log.h>
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, "EasyAR", __VA_ARGS__)
#else
#define LOGI(...) printf(__VA_ARGS__)
#endif

#define JNIFUNCTION_NATIVE(sig) Java_cn_easyar_samples_helloarmultitargetst_MainActivity_##sig
#define MAX_SPRITES 6
#define FOOT_TARGET 0
#define SPRITE_TARGET 1

extern "C" {
  JNIEXPORT jboolean JNICALL JNIFUNCTION_NATIVE(nativeInit(JNIEnv* env, jobject object));
  JNIEXPORT void JNICALL JNIFUNCTION_NATIVE(nativeDestory(JNIEnv* env, jobject object));
  JNIEXPORT void JNICALL JNIFUNCTION_NATIVE(nativeInitGL(JNIEnv* env, jobject object));
  JNIEXPORT void JNICALL JNIFUNCTION_NATIVE(nativeResizeGL(JNIEnv* env, jobject object, jint w, jint h));
  JNIEXPORT void JNICALL JNIFUNCTION_NATIVE(nativeRender(JNIEnv* env, jobject obj));
  JNIEXPORT void JNICALL JNIFUNCTION_NATIVE(nativeRotationChange(JNIEnv* env, jobject obj, jboolean portrait));
  JNIEXPORT void JNICALL JNIFUNCTION_NATIVE(contactChanged(JNIEnv* env, jobject, jboolean contact));
};

namespace EasyAR {
  namespace samples {

    class HelloAR : public AR
    {
      public:
        HelloAR();
        ~HelloAR() {
            if (renderer != NULL)
                delete renderer;
        }
        virtual void initGL();
        virtual void resizeGL(int width, int height);
        virtual void render();
        void updateSprites();

        static const char * foot_image;
        static const char * game_image;
        static const float activationThreshold;
        static const int deathTimeout;

        void setContact(bool contact) {
          foot_contact = contact;
        }
        Renderer * renderer;

      private:
        sprite sprites[MAX_SPRITES];
        Vec2I view_size;
        bool foot_contact;
    };

    const char * HelloAR::foot_image = "pebbles-small";
    const char * HelloAR::game_image = "candy";
    const float HelloAR::activationThreshold = 2;
    const int HelloAR::deathTimeout = 25;

    HelloAR::HelloAR() {
      view_size[0] = -1;
      srand(time(NULL));
      renderer = new Smashing::Box_Renderer();
      //renderer = new Smashing::PNG_Renderer();
    }

    void HelloAR::initGL()
    {
      renderer->init();
      augmenter_ = Augmenter();
      augmenter_.attachCamera(camera_);
    }

    void HelloAR::resizeGL(int width, int height)
    {
      view_size = Vec2I(width, height);
    }

    void HelloAR::updateSprites() {
      for(int i = 0; i < MAX_SPRITES; i++) {
        sprite::SpriteState cState = sprites[i].state;
        if(cState == sprite::SpriteState::ALIVE)
          sprites[i].move();
      }
    }

    void HelloAR::render() {

      /*
    glClearColor(0.f, 0.f, 0.f, 1.f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    Frame frame = augmenter_.newFrame();
    if(view_size[0] > 0){
        AR::resizeGL(view_size[0], view_size[1]);
        if(camera_ && camera_.isOpened())
            view_size[0] = -1;
    }
    augmenter_.setViewPort(viewport_);
    augmenter_.drawVideoBackground();
    glViewport(viewport_[0], viewport_[1], viewport_[2], viewport_[3]);

    for (int i = 0; i < frame.targets().size(); ++i) {
        AugmentedTarget::Status status = frame.targets()[i].status();
        if (status == AugmentedTarget::kTargetStatusTracked) {
            Matrix44F projectionMatrix = getProjectionGL(camera_.cameraCalibration(), 0.2f, 500.f);
            Matrix44F cameraview = getPoseGL(frame.targets()[i].pose());
            ImageTarget target = frame.targets()[i].target().cast_dynamic<ImageTarget>();
            renderer->render(projectionMatrix, cameraview, target.size(), sprites, MAX_SPRITES);
        }
    }
    */

      glClearColor(0.f, 0.f, 0.f, 1.f);
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

      Frame frame = augmenter_.newFrame();
      if(view_size[0] > 0){
        AR::resizeGL(view_size[0], view_size[1]);
        if(camera_ && camera_.isOpened())
          view_size[0] = -1;
      }

      augmenter_.setViewPort(viewport_);
      augmenter_.drawVideoBackground();
      glViewport(viewport_[0], viewport_[1], viewport_[2], viewport_[3]);

      // Find the indices of the specific targets
      int game_target = -1, foot_target = -1;
      for (int i = 0; i < frame.targets().size(); ++i) {
        Target target = frame.targets()[i].target();
        if (strcmp(target.name(), HelloAR::foot_image) == 0)
          foot_target = i;
        else if (strcmp(target.name(), HelloAR::game_image) == 0)
          game_target = i;
      }

      // Get positioning data for the foot
      Matrix34F foot_pose;
      bool foot_valid = false;
      if (foot_target != -1 &&
          frame.targets()[foot_target].status() == AugmentedTarget::kTargetStatusTracked) {
        foot_pose = frame.targets()[foot_target].pose();
        foot_valid = true;
      }

      Matrix44F glFootPose;
      if (foot_valid) {
        glFootPose = getPoseGL(foot_pose);
      }
      // Printing the pose for right now

      // If the game target is not in the scene - just return
      // ---- may need to render without objects?
      if (game_target == -1 ||
          frame.targets()[game_target].status() != AugmentedTarget::kTargetStatusTracked) {
        return;
      }


      AugmentedTarget gameMat = frame.targets()[game_target];
      Matrix34F gamePose = gameMat.pose();
      ImageTarget gameTarget = gameMat.target().cast_dynamic<ImageTarget>();

      // Look through the sprites for collisions
      if (foot_valid) {
        for (int i = 0; i < MAX_SPRITES; ++i) {
          if (sprites[i].state == sprite::SpriteState::ALIVE) {
            LOGI("Sprite %d with x: %.2f and y: %.2f", i, sprites[i].x, sprites[i].y);
            //LOGI("Foot 1 target with coordinates: x: %.2f y: %.2f z: %.2f", foot_pose.data[3], foot_pose.data[7], foot_pose.data[11]);
            //for (int j = 0; j < 4; ++j) {
              //LOGI("Foot %d (%.2f, %.2f, %.2f, %.2f)", j, glFootPose.data[j*4], glFootPose.data[j*4 + 1], glFootPose.data[j*4 + 2], glFootPose.data[j*4 + 3]);
             //}

              //LOGI("Foot 2 target with coordinates: x: %.2f y: %.2f z: %.2f", foot_pose.data[3], foot_pose.data[4], foot_pose.data[5]);
              //LOGI("Foot 3 target with coordinates: x: %.2f y: %.2f z: %.2f", foot_pose.data[6], foot_pose.data[7], foot_pose.data[8]);
              //LOGI("Foot 4 target with coordinates: x: %.2f y: %.2f z: %.2f", foot_pose.data[9], foot_pose.data[10], foot_pose.data[11]);

            //LOGI("Game target with coordinates: x: %.2f y: %.2f z: %.2f", gamePose.data[0], gamePose.data[1], gamePose.data[2]);
            if (abs(sprites[i].x - foot_pose.data[0]) < HelloAR::activationThreshold &&
                abs(sprites[i].y - foot_pose.data[1]) < HelloAR::activationThreshold) {
                if (foot_contact) {
                  sprites[i].state = sprite::SpriteState::SMASHED;
                  LOGI("Sprite %d now smashed", i);
                } else {
                  sprites[i].state = sprite::SpriteState::HIDDEN;
                  LOGI("Sprite %d now hidden", i);
                }
            }
          } else if (sprites[i].state == sprite::SpriteState::HIDDEN) {
            if (abs(sprites[i].x - foot_pose.data[0]) >= HelloAR::activationThreshold &&
                abs(sprites[i].y - foot_pose.data[1]) >= HelloAR::activationThreshold) {
              sprites[i].state = sprite::SpriteState::ALIVE;
            } else {
              if (foot_contact) {
                sprites[i].state = sprite::SpriteState::SMASHED;
                sprites[i].time_out = HelloAR::deathTimeout;
              }
            }
          } else if (sprites[i].state == sprite::SpriteState::SMASHED) {
            if (sprites[i].time_out == 0) {
              sprites[i].state = sprite::SpriteState::DEAD;
            } else {
              sprites[i].time_out--;
            }
          }
        }
      } else { // If no foot, make all non-dead ones alive
        for (int i = 0; i < MAX_SPRITES; ++i) {
          if (sprites[i].state != sprite::SpriteState::DEAD) {
            sprites[i].state = sprite::SpriteState::ALIVE;
          }
        }
      }

      // Get the projection and cameraview for the game target
      Matrix44F projectionMatrix = getProjectionGL(camera_.cameraCalibration(), 0.2f, 500.f);
      Matrix44F cameraView = getPoseGL(frame.targets()[game_target].pose());

      Vec2F data = gameTarget.size();
      data[1] = frame.targets()[game_target].pose().data[2];


      // Now render the sprites
      renderer->render(projectionMatrix, cameraView, gameTarget.size(), sprites, MAX_SPRITES);
    }
  }
}

EasyAR::samples::HelloAR ar;

JNIEXPORT jboolean JNICALL JNIFUNCTION_NATIVE(nativeInit(JNIEnv*, jobject))
{
  bool status = ar.initCamera();

  // Target for gameboard
  ar.loadFromJsonFile("targets.json", EasyAR::samples::HelloAR::foot_image);

  // Target for foot
  ar.loadFromJsonFile("targets.json", EasyAR::samples::HelloAR::game_image);

  //ar.loadAllFromJsonFile("targets2.json");
  //ar.loadFromImage("namecard.jpg");
  status &= ar.start();
  return status;
}

JNIEXPORT void JNICALL JNIFUNCTION_NATIVE(nativeDestory(JNIEnv*, jobject))
{
  ar.clear();
}

JNIEXPORT void JNICALL JNIFUNCTION_NATIVE(nativeInitGL(JNIEnv*, jobject))
{
  ar.initGL();
}

JNIEXPORT void JNICALL JNIFUNCTION_NATIVE(nativeResizeGL(JNIEnv*, jobject, jint w, jint h))
{
  ar.resizeGL(w, h);
}

JNIEXPORT void JNICALL JNIFUNCTION_NATIVE(nativeRender(JNIEnv*, jobject))
{
  ar.updateSprites();
  ar.render();
}

JNIEXPORT void JNICALL JNIFUNCTION_NATIVE(nativeRotationChange(JNIEnv*, jobject, jboolean portrait))
{
  ar.setPortrait(portrait);
}

JNIEXPORT void JNICALL JNIFUNCTION_NATIVE(contactChanged(JNIEnv*, jobject, jboolean contact)) {
  ar.setContact(contact);
  LOGI("Contact change JNI with value: %d", contact);
  if (contact)
    ar.renderer->rand_color();
}
