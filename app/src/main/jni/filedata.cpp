#include "filedata.hpp"

#include <cassert>
#include <jni.h>

#define JNIFUNCTION_NATIVE(sig) Java_cn_easyar_samples_helloarmultitargetst_PlatformAssetUtils_##sig

namespace Smashing {
  
  // Methods
  
  FileData::FileData(const std::string &path) {
    AAsset *asset =
      AAssetManager_open(asset_manager, path.c_str(), AASSET_MODE_STREAMING);
    assert(asset != NULL);

    data_length_ = AAsset_getLength(asset);
    data_ = AAsset_getBuffer(asset);
    file_handle_ = asset;
  }

  FileData::~FileData() {
    assert(this->file_handle_ != NULL);
    AAsset_close((AAsset *)(this->file_handle_));
  }

  const void *FileData::data(void) { return data_; }
  long FileData::data_length(void) { return data_length_; }
  
}

//JNIEXPORT void JNICALL JNIFUNCTION_NATIVE(init_1asset_1manager(JNIEnv *env, jobject thiz, jobject assetManager)) {
//JNIEXPORT void JNICALL Java_cn_easyar_samples_helloarmultitargetst_PlatformAssetUtils_init_1asset_1manager(JNIEnv *env, jclass thisClass, jobject assetManager) {
//    Smashing::asset_manager = AAssetManager_fromJava(env, assetManager);
//}
