#include "filedata.hpp"

#include <cassert>
#include <jni.h>

#include "android/asset_manager_jni.h"

namespace Smashing {

  static AAssetManager *asset_manager;

  JNIEXPORT void JNICALL JNIFUNCTION_NATIVE(init_asset_manager(JNIEnv *env, jobject, jobject assetManager)) {
    asset_utils = AAssetManager_fromJava(env, assetManager);
  }
  
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

  void *FileData::data(void) { return data_; }
  long FileData::data_length(void) { return data_length_; }
  
}
