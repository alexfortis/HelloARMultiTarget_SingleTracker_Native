#include "filedata.hpp"

#include <cassert>
#include <jni.h>

#ifdef ANDROID
#include <android/log.h>
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, "FileData", __VA_ARGS__)
#define LOGW(...) __android_log_print(ANDROID_LOG_WARN, "FileData", __VA_ARGS__)
#else
#define LOGI(...) printf(__VA_ARGS__)
#define LOGW(...) fprintf(stderr, __VA_ARGS__)
#endif

#define JNIFUNCTION_NATIVE(sig) Java_cn_easyar_samples_helloarmultitargetst_PlatformAssetUtils_##sig

extern "C" {
  JNIEXPORT void JNICALL JNIFUNCTION_NATIVE(init_1asset_1manager(JNIEnv *env, jobject thiz, jobject assetManager));
}

namespace Smashing {
  
  // Methods
  
  FileData::FileData(const std::string &path) {
    if (!asset_manager) {
        LOGW("AssetManager not setup correctly\n");
    }
    //LOGI("Getting FileData for %s started\n", path.c_str());
    AAsset *asset =
      AAssetManager_open(asset_manager, path.c_str(), AASSET_MODE_STREAMING);
    assert(asset != NULL);
    data_length_ = AAsset_getLength(asset);
    data_ = AAsset_getBuffer(asset);
    file_handle_ = asset;
    //LOGI("Getting FileData for %s complete\n", path.c_str());
  }

  FileData::~FileData() {
    assert(this->file_handle_ != NULL);
    AAsset_close((AAsset *)(this->file_handle_));
  }

  const void *FileData::data(void) { return data_; }
  long FileData::data_length(void) { return data_length_; }
  
}


JNIEXPORT void JNICALL JNIFUNCTION_NATIVE(init_1asset_1manager(JNIEnv *env, jobject thiz, jobject assetManager)) {
  LOGI("INIT_ASSET_MANAGER CALLED!\n");
  Smashing::asset_manager = AAssetManager_fromJava(env, assetManager);
  if (!Smashing::asset_manager) {
    LOGW("Failed to setup AssetManager properly!\n");
  }
}