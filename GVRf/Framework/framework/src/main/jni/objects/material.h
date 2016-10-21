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
 * Links textures and shaders.
 ***************************************************************************/

#ifndef MATERIAL_H_
#define MATERIAL_H_

#include <map>
#include <memory>
#include <string>

#include "glm/glm.hpp"

#include "objects/shader_data.h"
#include "objects/textures/texture.h"
#include "objects/components/render_data.h"
#include "objects/components/event_handler.h"
#include "objects/uniform_block.h"
namespace gvr {
class RenderData;
class Color;

class Material: public ShaderData {
public:
    explicit Material() : ShaderData(), shader_feature_set_(0), listener_(new Listener()),gl_ubo_("float u_opacity; float3 u_color; float4 ambient_color; float4 diffuse_color; float4 specular_color; float4 emissive_color; float specular_exponent") {
    }

    ~Material() {
        //delete gl_ubo_;
    }


    virtual void setTexture(const std::string& key, Texture* texture) {
        ShaderData::setTexture(key, texture);
        if (key == "main_texture") {
            main_texture = texture;
        }
        listener_->notify_listeners(true);
    }

    virtual void setFloat(const std::string& key, float value) {
        ShaderData::setFloat(key, value);
        listener_->notify_listeners(true);
    }


    virtual void setVec2(const std::string& key, glm::vec2 vector) {
        ShaderData::setVec2(key, vector);
        listener_->notify_listeners(true);
    }


    virtual void setVec3(const std::string& key, glm::vec3 vector) {
        ShaderData::setVec3(key, vector);
        listener_->notify_listeners(true);
    }

    virtual void setVec4(const std::string& key, glm::vec4 vector) {
        ShaderData::setVec4(key, vector);
        listener_->notify_listeners(true);
    }

    bool hasTexture() const {
        return (main_texture != NULL) || (textures_.size() > 0);
    }

    virtual void setMat4(const std::string& key, glm::mat4 matrix) {
        ShaderData::setMat4(key, matrix);
        listener_->notify_listeners(true);
    }

    int get_shader_feature_set() {
        return shader_feature_set_;
    }

    void set_shader_feature_set(int feature_set) {
        shader_feature_set_ = feature_set;
    }

    virtual bool isMainTextureReady() {
        return (main_texture != NULL) && main_texture->isReady();
    }

    bool isTextureReady(const std::string& name) {
        auto it = textures_.find(name);
        if (it != textures_.end()) {
            return ((Texture*) it->second)->isReady();
        } else {
            return false;
        }
    }

    void add_listener(Listener* listener){
        listener_->add_listener(listener);
    }

    void add_listener(RenderData* render_data){
        if(render_data)
            listener_->add_listener(render_data);
    }

    void remove_listener(Listener* listener){
        listener_->remove_listener(listener);
    }

    void notify_listener(bool dirty){
        listener_->notify_listeners(dirty);
    }
   void setUniformDesc(std::string uniform_desc){
        uniform_desc_ = uniform_desc;
    }
/*  void bindUbo(int program_id){

        if(gl_ubo_== nullptr){
           gl_ubo_ = new GLUniformBlock(uniform_desc_);
           gl_ubo_->setGLBindingPoint(MATERIAL_UBO_INDEX);
           gl_ubo_->setBlockName("Material_ubo");
           gl_ubo_->bindBuffer(program_id);
        }
    }
    GLUniformBlock* getUbo(){
        return gl_ubo_;
    }
*/

 void bindUbo(int program_id){
        if(!ubo_init){
            ubo_init = true;
           gl_ubo_.setGLBindingPoint(MATERIAL_UBO_INDEX);
           gl_ubo_.setBlockName("Material_ubo");
           gl_ubo_.bindBuffer(program_id);

        }
    }
    GLUniformBlock& getUbo(){
        return gl_ubo_;
    }
private:
    GLUniformBlock gl_ubo_;
    std::string uniform_desc_;
    bool ubo_init = false;
    Material(const Material& material);
    Material(Material&& material);
    Material& operator=(const Material& material);
    Material& operator=(Material&& material);

private:
    Listener* listener_;
    Texture* main_texture = NULL;
    unsigned int shader_feature_set_;
};
}
#endif
