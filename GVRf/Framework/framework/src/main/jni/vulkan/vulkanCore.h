/* Copyright 2015 Samsung Electronics Co., LTD
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */


#ifndef FRAMEWORK_VULKANCORE_H
#define FRAMEWORK_VULKANCORE_H

#define VK_USE_PLATFORM_ANDROID_KHR

#include <android/native_window_jni.h>	// for native window JNI
#include "vulkan/vulkan_wrapper.h"
#include "vulkanInfoWrapper.h"
#include <vector>
#include "glm/glm.hpp"

#define GVR_VK_CHECK(X) if (!(X)) { LOGD("VK_CHECK Failure"); assert((X));}
#define GVR_VK_VERTEX_BUFFER_BIND_ID 0
#define GVR_VK_SAMPLE_NAME "GVR Vulkan"
#define VK_KHR_ANDROID_SURFACE_EXTENSION_NAME "VK_KHR_android_surface"


namespace gvr {
class Scene;
class RenderData;
class Camera;
extern  uint8_t *oculusTexData;

const char shader_tri_frag[]={
        0x03, 0x02, 0x23, 0x07, 0x00, 0x00, 0x01, 0x00, 0x01, 0x00, 0x08, 0x00,
          0x0d, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x11, 0x00, 0x02, 0x00,
          0x01, 0x00, 0x00, 0x00, 0x0b, 0x00, 0x06, 0x00, 0x01, 0x00, 0x00, 0x00,
          0x47, 0x4c, 0x53, 0x4c, 0x2e, 0x73, 0x74, 0x64, 0x2e, 0x34, 0x35, 0x30,
          0x00, 0x00, 0x00, 0x00, 0x0e, 0x00, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00,
          0x01, 0x00, 0x00, 0x00, 0x0f, 0x00, 0x06, 0x00, 0x04, 0x00, 0x00, 0x00,
          0x04, 0x00, 0x00, 0x00, 0x6d, 0x61, 0x69, 0x6e, 0x00, 0x00, 0x00, 0x00,
          0x09, 0x00, 0x00, 0x00, 0x10, 0x00, 0x03, 0x00, 0x04, 0x00, 0x00, 0x00,
          0x07, 0x00, 0x00, 0x00, 0x03, 0x00, 0x03, 0x00, 0x02, 0x00, 0x00, 0x00,
          0x90, 0x01, 0x00, 0x00, 0x04, 0x00, 0x09, 0x00, 0x47, 0x4c, 0x5f, 0x41,
          0x52, 0x42, 0x5f, 0x73, 0x65, 0x70, 0x61, 0x72, 0x61, 0x74, 0x65, 0x5f,
          0x73, 0x68, 0x61, 0x64, 0x65, 0x72, 0x5f, 0x6f, 0x62, 0x6a, 0x65, 0x63,
          0x74, 0x73, 0x00, 0x00, 0x04, 0x00, 0x09, 0x00, 0x47, 0x4c, 0x5f, 0x41,
          0x52, 0x42, 0x5f, 0x73, 0x68, 0x61, 0x64, 0x69, 0x6e, 0x67, 0x5f, 0x6c,
          0x61, 0x6e, 0x67, 0x75, 0x61, 0x67, 0x65, 0x5f, 0x34, 0x32, 0x30, 0x70,
          0x61, 0x63, 0x6b, 0x00, 0x05, 0x00, 0x04, 0x00, 0x04, 0x00, 0x00, 0x00,
          0x6d, 0x61, 0x69, 0x6e, 0x00, 0x00, 0x00, 0x00, 0x05, 0x00, 0x05, 0x00,
          0x09, 0x00, 0x00, 0x00, 0x75, 0x46, 0x72, 0x61, 0x67, 0x43, 0x6f, 0x6c,
          0x6f, 0x72, 0x00, 0x00, 0x47, 0x00, 0x04, 0x00, 0x09, 0x00, 0x00, 0x00,
          0x1e, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x13, 0x00, 0x02, 0x00,
          0x02, 0x00, 0x00, 0x00, 0x21, 0x00, 0x03, 0x00, 0x03, 0x00, 0x00, 0x00,
          0x02, 0x00, 0x00, 0x00, 0x16, 0x00, 0x03, 0x00, 0x06, 0x00, 0x00, 0x00,
          0x20, 0x00, 0x00, 0x00, 0x17, 0x00, 0x04, 0x00, 0x07, 0x00, 0x00, 0x00,
          0x06, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x20, 0x00, 0x04, 0x00,
          0x08, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x07, 0x00, 0x00, 0x00,
          0x3b, 0x00, 0x04, 0x00, 0x08, 0x00, 0x00, 0x00, 0x09, 0x00, 0x00, 0x00,
          0x03, 0x00, 0x00, 0x00, 0x2b, 0x00, 0x04, 0x00, 0x06, 0x00, 0x00, 0x00,
          0x0a, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x3f, 0x2b, 0x00, 0x04, 0x00,
          0x06, 0x00, 0x00, 0x00, 0x0b, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
          0x2c, 0x00, 0x07, 0x00, 0x07, 0x00, 0x00, 0x00, 0x0c, 0x00, 0x00, 0x00,
          0x0a, 0x00, 0x00, 0x00, 0x0b, 0x00, 0x00, 0x00, 0x0b, 0x00, 0x00, 0x00,
          0x0a, 0x00, 0x00, 0x00, 0x36, 0x00, 0x05, 0x00, 0x02, 0x00, 0x00, 0x00,
          0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00,
          0xf8, 0x00, 0x02, 0x00, 0x05, 0x00, 0x00, 0x00, 0x3e, 0x00, 0x03, 0x00,
          0x09, 0x00, 0x00, 0x00, 0x0c, 0x00, 0x00, 0x00, 0xfd, 0x00, 0x01, 0x00,
          0x38, 0x00, 0x01, 0x00
};
const int shader_tri_frag_size=424;

const char shader_tri_vert[]={
        0x03, 0x02, 0x23, 0x07, 0x00, 0x00, 0x01, 0x00, 0x01, 0x00, 0x08, 0x00,
          0x28, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x11, 0x00, 0x02, 0x00,
          0x01, 0x00, 0x00, 0x00, 0x11, 0x00, 0x02, 0x00, 0x20, 0x00, 0x00, 0x00,
          0x0b, 0x00, 0x06, 0x00, 0x01, 0x00, 0x00, 0x00, 0x47, 0x4c, 0x53, 0x4c,
          0x2e, 0x73, 0x74, 0x64, 0x2e, 0x34, 0x35, 0x30, 0x00, 0x00, 0x00, 0x00,
          0x0e, 0x00, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00,
          0x0f, 0x00, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00,
          0x6d, 0x61, 0x69, 0x6e, 0x00, 0x00, 0x00, 0x00, 0x0d, 0x00, 0x00, 0x00,
          0x19, 0x00, 0x00, 0x00, 0x03, 0x00, 0x03, 0x00, 0x02, 0x00, 0x00, 0x00,
          0x90, 0x01, 0x00, 0x00, 0x04, 0x00, 0x09, 0x00, 0x47, 0x4c, 0x5f, 0x41,
          0x52, 0x42, 0x5f, 0x73, 0x65, 0x70, 0x61, 0x72, 0x61, 0x74, 0x65, 0x5f,
          0x73, 0x68, 0x61, 0x64, 0x65, 0x72, 0x5f, 0x6f, 0x62, 0x6a, 0x65, 0x63,
          0x74, 0x73, 0x00, 0x00, 0x04, 0x00, 0x09, 0x00, 0x47, 0x4c, 0x5f, 0x41,
          0x52, 0x42, 0x5f, 0x73, 0x68, 0x61, 0x64, 0x69, 0x6e, 0x67, 0x5f, 0x6c,
          0x61, 0x6e, 0x67, 0x75, 0x61, 0x67, 0x65, 0x5f, 0x34, 0x32, 0x30, 0x70,
          0x61, 0x63, 0x6b, 0x00, 0x05, 0x00, 0x04, 0x00, 0x04, 0x00, 0x00, 0x00,
          0x6d, 0x61, 0x69, 0x6e, 0x00, 0x00, 0x00, 0x00, 0x05, 0x00, 0x06, 0x00,
          0x0b, 0x00, 0x00, 0x00, 0x67, 0x6c, 0x5f, 0x50, 0x65, 0x72, 0x56, 0x65,
          0x72, 0x74, 0x65, 0x78, 0x00, 0x00, 0x00, 0x00, 0x06, 0x00, 0x06, 0x00,
          0x0b, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x67, 0x6c, 0x5f, 0x50,
          0x6f, 0x73, 0x69, 0x74, 0x69, 0x6f, 0x6e, 0x00, 0x06, 0x00, 0x07, 0x00,
          0x0b, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x67, 0x6c, 0x5f, 0x50,
          0x6f, 0x69, 0x6e, 0x74, 0x53, 0x69, 0x7a, 0x65, 0x00, 0x00, 0x00, 0x00,
          0x06, 0x00, 0x07, 0x00, 0x0b, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00,
          0x67, 0x6c, 0x5f, 0x43, 0x6c, 0x69, 0x70, 0x44, 0x69, 0x73, 0x74, 0x61,
          0x6e, 0x63, 0x65, 0x00, 0x05, 0x00, 0x03, 0x00, 0x0d, 0x00, 0x00, 0x00,
          0x00, 0x00, 0x00, 0x00, 0x05, 0x00, 0x04, 0x00, 0x11, 0x00, 0x00, 0x00,
          0x6d, 0x61, 0x74, 0x72, 0x69, 0x78, 0x00, 0x00, 0x06, 0x00, 0x04, 0x00,
          0x11, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x6d, 0x76, 0x70, 0x00,
          0x05, 0x00, 0x05, 0x00, 0x13, 0x00, 0x00, 0x00, 0x6d, 0x61, 0x74, 0x72,
          0x69, 0x63, 0x65, 0x73, 0x00, 0x00, 0x00, 0x00, 0x05, 0x00, 0x03, 0x00,
          0x19, 0x00, 0x00, 0x00, 0x70, 0x6f, 0x73, 0x00, 0x48, 0x00, 0x05, 0x00,
          0x0b, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0b, 0x00, 0x00, 0x00,
          0x00, 0x00, 0x00, 0x00, 0x48, 0x00, 0x05, 0x00, 0x0b, 0x00, 0x00, 0x00,
          0x01, 0x00, 0x00, 0x00, 0x0b, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00,
          0x48, 0x00, 0x05, 0x00, 0x0b, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00,
          0x0b, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x47, 0x00, 0x03, 0x00,
          0x0b, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x48, 0x00, 0x04, 0x00,
          0x11, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x05, 0x00, 0x00, 0x00,
          0x48, 0x00, 0x05, 0x00, 0x11, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
          0x23, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x48, 0x00, 0x05, 0x00,
          0x11, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0x00, 0x00, 0x00,
          0x10, 0x00, 0x00, 0x00, 0x47, 0x00, 0x03, 0x00, 0x11, 0x00, 0x00, 0x00,
          0x02, 0x00, 0x00, 0x00, 0x47, 0x00, 0x04, 0x00, 0x13, 0x00, 0x00, 0x00,
          0x22, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x47, 0x00, 0x04, 0x00,
          0x13, 0x00, 0x00, 0x00, 0x21, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
          0x47, 0x00, 0x04, 0x00, 0x19, 0x00, 0x00, 0x00, 0x1e, 0x00, 0x00, 0x00,
          0x00, 0x00, 0x00, 0x00, 0x13, 0x00, 0x02, 0x00, 0x02, 0x00, 0x00, 0x00,
          0x21, 0x00, 0x03, 0x00, 0x03, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00,
          0x16, 0x00, 0x03, 0x00, 0x06, 0x00, 0x00, 0x00, 0x20, 0x00, 0x00, 0x00,
          0x17, 0x00, 0x04, 0x00, 0x07, 0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00,
          0x04, 0x00, 0x00, 0x00, 0x15, 0x00, 0x04, 0x00, 0x08, 0x00, 0x00, 0x00,
          0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x2b, 0x00, 0x04, 0x00,
          0x08, 0x00, 0x00, 0x00, 0x09, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00,
          0x1c, 0x00, 0x04, 0x00, 0x0a, 0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00,
          0x09, 0x00, 0x00, 0x00, 0x1e, 0x00, 0x05, 0x00, 0x0b, 0x00, 0x00, 0x00,
          0x07, 0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00, 0x0a, 0x00, 0x00, 0x00,
          0x20, 0x00, 0x04, 0x00, 0x0c, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00,
          0x0b, 0x00, 0x00, 0x00, 0x3b, 0x00, 0x04, 0x00, 0x0c, 0x00, 0x00, 0x00,
          0x0d, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x15, 0x00, 0x04, 0x00,
          0x0e, 0x00, 0x00, 0x00, 0x20, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00,
          0x2b, 0x00, 0x04, 0x00, 0x0e, 0x00, 0x00, 0x00, 0x0f, 0x00, 0x00, 0x00,
          0x00, 0x00, 0x00, 0x00, 0x18, 0x00, 0x04, 0x00, 0x10, 0x00, 0x00, 0x00,
          0x07, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x1e, 0x00, 0x03, 0x00,
          0x11, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x20, 0x00, 0x04, 0x00,
          0x12, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x11, 0x00, 0x00, 0x00,
          0x3b, 0x00, 0x04, 0x00, 0x12, 0x00, 0x00, 0x00, 0x13, 0x00, 0x00, 0x00,
          0x02, 0x00, 0x00, 0x00, 0x20, 0x00, 0x04, 0x00, 0x14, 0x00, 0x00, 0x00,
          0x02, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x17, 0x00, 0x04, 0x00,
          0x17, 0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00,
          0x20, 0x00, 0x04, 0x00, 0x18, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00,
          0x17, 0x00, 0x00, 0x00, 0x3b, 0x00, 0x04, 0x00, 0x18, 0x00, 0x00, 0x00,
          0x19, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x2b, 0x00, 0x04, 0x00,
          0x08, 0x00, 0x00, 0x00, 0x1a, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
          0x20, 0x00, 0x04, 0x00, 0x1b, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00,
          0x06, 0x00, 0x00, 0x00, 0x2b, 0x00, 0x04, 0x00, 0x08, 0x00, 0x00, 0x00,
          0x20, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x2b, 0x00, 0x04, 0x00,
          0x06, 0x00, 0x00, 0x00, 0x23, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x3f,
          0x20, 0x00, 0x04, 0x00, 0x26, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00,
          0x07, 0x00, 0x00, 0x00, 0x36, 0x00, 0x05, 0x00, 0x02, 0x00, 0x00, 0x00,
          0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00,
          0xf8, 0x00, 0x02, 0x00, 0x05, 0x00, 0x00, 0x00, 0x41, 0x00, 0x05, 0x00,
          0x14, 0x00, 0x00, 0x00, 0x15, 0x00, 0x00, 0x00, 0x13, 0x00, 0x00, 0x00,
          0x0f, 0x00, 0x00, 0x00, 0x3d, 0x00, 0x04, 0x00, 0x10, 0x00, 0x00, 0x00,
          0x16, 0x00, 0x00, 0x00, 0x15, 0x00, 0x00, 0x00, 0x41, 0x00, 0x05, 0x00,
          0x1b, 0x00, 0x00, 0x00, 0x1c, 0x00, 0x00, 0x00, 0x19, 0x00, 0x00, 0x00,
          0x1a, 0x00, 0x00, 0x00, 0x3d, 0x00, 0x04, 0x00, 0x06, 0x00, 0x00, 0x00,
          0x1d, 0x00, 0x00, 0x00, 0x1c, 0x00, 0x00, 0x00, 0x41, 0x00, 0x05, 0x00,
          0x1b, 0x00, 0x00, 0x00, 0x1e, 0x00, 0x00, 0x00, 0x19, 0x00, 0x00, 0x00,
          0x09, 0x00, 0x00, 0x00, 0x3d, 0x00, 0x04, 0x00, 0x06, 0x00, 0x00, 0x00,
          0x1f, 0x00, 0x00, 0x00, 0x1e, 0x00, 0x00, 0x00, 0x41, 0x00, 0x05, 0x00,
          0x1b, 0x00, 0x00, 0x00, 0x21, 0x00, 0x00, 0x00, 0x19, 0x00, 0x00, 0x00,
          0x20, 0x00, 0x00, 0x00, 0x3d, 0x00, 0x04, 0x00, 0x06, 0x00, 0x00, 0x00,
          0x22, 0x00, 0x00, 0x00, 0x21, 0x00, 0x00, 0x00, 0x50, 0x00, 0x07, 0x00,
          0x07, 0x00, 0x00, 0x00, 0x24, 0x00, 0x00, 0x00, 0x1d, 0x00, 0x00, 0x00,
          0x1f, 0x00, 0x00, 0x00, 0x22, 0x00, 0x00, 0x00, 0x23, 0x00, 0x00, 0x00,
          0x91, 0x00, 0x05, 0x00, 0x07, 0x00, 0x00, 0x00, 0x25, 0x00, 0x00, 0x00,
          0x16, 0x00, 0x00, 0x00, 0x24, 0x00, 0x00, 0x00, 0x41, 0x00, 0x05, 0x00,
          0x26, 0x00, 0x00, 0x00, 0x27, 0x00, 0x00, 0x00, 0x0d, 0x00, 0x00, 0x00,
          0x0f, 0x00, 0x00, 0x00, 0x3e, 0x00, 0x03, 0x00, 0x27, 0x00, 0x00, 0x00,
          0x25, 0x00, 0x00, 0x00, 0xfd, 0x00, 0x01, 0x00, 0x38, 0x00, 0x01, 0x00
};
const int shader_tri_vert_size=1212;




class VulkanCore {
public:
    // Return NULL if Vulkan inititialisation failed. NULL denotes no Vulkan support for this device.
    static VulkanCore* getInstance(ANativeWindow * newNativeWindow = nullptr) {
        if (!theInstance) {
            theInstance = new VulkanCore(newNativeWindow);
        }
        if (theInstance->m_Vulkan_Initialised)
            return theInstance;
        return NULL;
    }
    void UpdateUniforms(Scene* scene, Camera* camera, RenderData* render_data);
     void InitUniformBuffersForRenderData(GVR_Uniform &m_modelViewMatrixUniform);
     void InitDescriptorSetForRenderData(GVR_Uniform &m_modelViewMatrixUniform, VkDescriptorSet &m_descriptorSet);
     void BuildCmdBufferForRenderData(std::vector <VkDescriptorSet> &allDescriptors, int &swapChainIndex, std::vector<RenderData*>& render_data_vector);
     void DrawFrameForRenderData(int &swapChainIndex);
      int AcquireNextImage();
      void InitVertexBuffersFromRenderData(const std::vector<glm::vec3>& vertices, GVR_VK_Vertices &m_vertices, GVR_VK_Indices &m_indices, const std::vector<unsigned short> & indices);
     //void InitVertexBuffersFromRenderData(GVR_VK_Vertices &m_vertices, GVR_VK_Indices &m_indices);
      void InitPipelineForRenderData(GVR_VK_Vertices &m_vertices, VkPipeline &m_pipeline);
      VkShaderModule CreateShaderModuleAscii(const uint32_t* code, uint32_t size);
      bool GetMemoryTypeFromProperties( uint32_t typeBits, VkFlags requirements_mask, uint32_t* typeIndex);
private:
    std::vector<VkFence> waitFences;
    static VulkanCore* theInstance;
    VulkanCore(ANativeWindow * newNativeWindow) : m_pPhysicalDevices(NULL){
        m_Vulkan_Initialised = false;
        initVulkanCore(newNativeWindow);
    }
    bool CreateInstance();
    VkShaderModule CreateShaderModule(std::vector<uint32_t> code, uint32_t size);
    bool GetPhysicalDevices();
    void initVulkanCore(ANativeWindow * newNativeWindow);
    bool InitDevice();
    void InitSurface();
    void InitSwapchain(uint32_t width, uint32_t height);

    void InitCommandbuffers();
    void InitVertexBuffers();
    void InitLayouts();
    void InitRenderPass();
    void InitFrameBuffers();
    void InitSync();
    void BuildCmdBuffer();

    void InitUniformBuffers();


    bool m_Vulkan_Initialised;
    ANativeWindow * m_androidWindow;

    VkInstance m_instance;
    VkPhysicalDevice* m_pPhysicalDevices;
    VkPhysicalDevice m_physicalDevice;
    VkPhysicalDeviceProperties m_physicalDeviceProperties;
    VkPhysicalDeviceMemoryProperties m_physicalDeviceMemoryProperties;
    VkDevice m_device;
    uint32_t m_physicalDeviceCount;
    uint32_t m_queueFamilyIndex;
    VkQueue m_queue;
    VkSurfaceKHR m_surface;
    VkSurfaceFormatKHR m_surfaceFormat;

    VkSwapchainKHR m_swapchain;
    GVR_VK_SwapchainBuffer* m_swapchainBuffers;

    uint32_t m_swapchainCurrentIdx;
    uint32_t m_height;
    uint32_t m_width;
    uint32_t m_swapchainImageCount;
    VkSemaphore m_backBufferSemaphore;
    VkSemaphore m_renderCompleteSemaphore;
    VkFramebuffer* m_frameBuffers;

    VkCommandPool m_commandPool;
    GVR_VK_DepthBuffer* m_depthBuffers;
    GVR_VK_Vertices m_vertices;

    VkDescriptorSetLayout m_descriptorLayout;
    VkPipelineLayout  m_pipelineLayout;
    VkRenderPass m_renderPass;
    VkPipeline m_pipeline;
    OutputBuffer* m_outputBuffers;
    uint8_t * texDataVulkan;
    int imageIndex = 0;
    uint8_t *finaloutput;
    GVR_Uniform m_modelViewMatrixUniform;
    VkDescriptorPool m_descriptorPool;
    VkDescriptorSet m_descriptorSet;
    GVR_VK_Indices m_indices;
};


extern VulkanCore gvrVulkanCore;
}
#endif //FRAMEWORK_VULKANCORE_H
