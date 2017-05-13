/**
 * Copyright (c) 2015-2016 VisionStar Information Technology (Shanghai) Co., Ltd. All Rights Reserved.
 * EasyAR is the registered trademark or trademark of VisionStar Information Technology (Shanghai) Co., Ltd in China
 * and other countries for the augmented reality technology developed by VisionStar Information Technology (Shanghai) Co., Ltd.
 */

#include "ar.hpp"
#include "renderer.hpp"
#include "sprite.h"
#include <jni.h>
#include <GLES3/gl31.h>
#include <cmath>
#include <climits>


#ifdef ANDROID
#include <android/log.h>
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, "EasyAR", __VA_ARGS__)
#else
#define LOGI(...) printf(__VA_ARGS__)
#endif

#define JNIFUNCTION_NATIVE(sig) Java_cn_easyar_samples_helloarmultitargetst_MainActivity_##sig
#define MAX_SPRITES 2
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

      private:
        sprite sprites[MAX_SPRITES];
        Vec2I view_size;
        Renderer renderer;
        bool foot_contact;
    };

    const char * HelloAR::foot_image = "pebbles";
    const char * HelloAR::game_image = "candy";
    const float HelloAR::activationThreshold = 0.5;
    const int HelloAR::deathTimeout = 25;

    HelloAR::HelloAR() : renderer("shaders/png.vsh", "shaders/png.fsh") {
      view_size[0] = -1;
      srand(time(NULL));
    }

    void HelloAR::initGL()
    {
      renderer.init();
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

      /*
      if (foot_target != -1) {
        LOGI("Foot target index: %d\n", foot_target);
      }

      if (game_target != -1) {
        LOGI("Game target index: %d\n", game_target);
      }
      */

      // Get positioning data for the foot
      Matrix34F foot_pose;
      bool foot_valid = false;
      if (foot_target != -1 && 
          frame.targets()[foot_target].status() == AugmentedTarget::kTargetStatusTracked) {
        foot_pose = frame.targets()[foot_target].pose();
        foot_valid = true;
      }

      // Printing the pose for right now

      // If the game target is not in the scene - just return
      // ---- may need to render without objects?
      if (game_target == -1 || 
          frame.targets()[game_target].status() != AugmentedTarget::kTargetStatusTracked) {
        return;
      }

      // Get the projection and cameraview for the game target
      Matrix44F projectionMatrix = getProjectionGL(camera_.cameraCalibration(), 0.2f, 500.f);
      Matrix44F cameraView = getPoseGL(frame.targets()[game_target].pose());

      ImageTarget gameTarget = frame.targets()[game_target].target().cast_dynamic<ImageTarget>();

      // Look through the sprites for collisions
      if (foot_valid) {
        for (int i = 0; i < MAX_SPRITES; ++i) {
          if (sprites[i].state == sprite::SpriteState::ALIVE) {
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
              sprites[i].state == sprite::SpriteState::DEAD;
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

      // Now render the sprites
      renderer.render(projectionMatrix, cameraView, gameTarget.size(), sprites, MAX_SPRITES);
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
}