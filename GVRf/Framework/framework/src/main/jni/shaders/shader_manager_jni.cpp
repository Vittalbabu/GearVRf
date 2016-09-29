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

/***************************************************************************
 * JNI
 ***************************************************************************/

#include "shader_manager.h"

#include "util/gvr_jni.h"

namespace gvr {
extern "C" {
    JNIEXPORT jlong JNICALL
    Java_org_gearvrf_NativeShaderManager_ctor(JNIEnv * env, jobject obj);

    JNIEXPORT jlong JNICALL
    Java_org_gearvrf_NativeShaderManager_addShader(
            JNIEnv * env, jobject obj, jlong jshader_manager,
            jstring signature,
            jstring uniformDesc,
            jstring textureDesc,
            jstring vertexDesc,
            jstring vertex_shader,
            jstring fragment_shader);

    JNIEXPORT jlong JNICALL
    Java_org_gearvrf_NativeShaderManager_getShaderMap(
            JNIEnv * env, jobject obj, jlong jshader_manager, jint id);

    JNIEXPORT jint JNICALL
    Java_org_gearvrf_NativeShaderManager_getShader(
            JNIEnv * env, jobject obj, jlong jshader_manager, jstring signature);
}

JNIEXPORT jlong JNICALL
Java_org_gearvrf_NativeShaderManager_ctor(JNIEnv * env,
    jobject obj) {
    return reinterpret_cast<jlong>(new ShaderManager());
}

JNIEXPORT jlong JNICALL
Java_org_gearvrf_NativeShaderManager_addShader(
    JNIEnv * env, jobject obj, jlong jshader_manager,
    jstring signature,
    jstring uniformDesc,
    jstring textureDesc,
    jstring vertexDesc,
    jstring vertex_shader,
    jstring fragment_shader) {
    const char *sig_str = env->GetStringUTFChars(signature, 0);
    const char* uniform_str = env->GetStringUTFChars(uniformDesc, 0);
    const char* texture_str = env->GetStringUTFChars(textureDesc, 0);
    const char* vdesc_str = env->GetStringUTFChars(vertexDesc, 0);
    const char *vertex_str = env->GetStringUTFChars(vertex_shader, 0);
    const char *fragment_str = env->GetStringUTFChars(fragment_shader, 0);
    std::string native_sig(sig_str);
    std::string native_vertex_shader(vertex_str);
    std::string native_fragment_shader(fragment_str);
    std::string native_udesc(uniform_str);
    std::string native_tdesc(texture_str);
    std::string native_vdesc(vdesc_str);
    ShaderManager* shader_manager = reinterpret_cast<ShaderManager*>(jshader_manager);
    long id = shader_manager->addShader(native_sig, native_udesc, native_tdesc, native_vdesc, native_vertex_shader, native_fragment_shader);
    env->ReleaseStringUTFChars(vertex_shader, vertex_str);
    env->ReleaseStringUTFChars(fragment_shader, fragment_str);
    env->ReleaseStringUTFChars(signature, sig_str);
    env->ReleaseStringUTFChars(uniformDesc, uniform_str);
    env->ReleaseStringUTFChars(textureDesc, texture_str);
    env->ReleaseStringUTFChars(vertexDesc, vdesc_str);
    return id;
}

JNIEXPORT jlong JNICALL
Java_org_gearvrf_NativeShaderManager_getShaderMap(
    JNIEnv * env, jobject obj, jlong jshader_manager, jint id) {
    ShaderManager* shader_manager = reinterpret_cast<ShaderManager*>(jshader_manager);
    Shader* shader = shader_manager->getShader(id);
    return reinterpret_cast<jlong>(shader);
}

JNIEXPORT jint JNICALL
Java_org_gearvrf_NativeShaderManager_getShader(
    JNIEnv * env, jobject obj, jlong jshader_manager, jstring signature) {
    ShaderManager* shader_manager = reinterpret_cast<ShaderManager*>(jshader_manager);
    const char *sig_str = env->GetStringUTFChars(signature, 0);
    std::string native_sig(sig_str);
    Shader* shader = shader_manager->findShader((const std::string&) native_sig);

    env->ReleaseStringUTFChars(signature, sig_str);
    if (shader != NULL)
    {
        return shader->getShaderID();
    }
    return 0;
}

}
