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
#include "vulkan/vulkanCore.h"
#include "objects/uniform_block.h"
#include "vulkan/vulkan_headers.h"
namespace gvr {
class RenderData;
class Color;

class Material: public ShaderData {
public:
    explicit Material() : ShaderData(), shader_feature_set_(0), listener_(new Listener()),mat_ubo_(nullptr),material_dirty_(true),uniform_desc_(" ")
            , vk_descriptor(nullptr){
    }

    ~Material() {
        delete mat_ubo_;
        delete vk_descriptor;
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
        material_dirty_ = true;
    }


    virtual void setVec2(const std::string& key, glm::vec2 vector) {
        ShaderData::setVec2(key, vector);
        listener_->notify_listeners(true);
         material_dirty_ = true;
    }


    virtual void setVec3(const std::string& key, glm::vec3 vector) {
        ShaderData::setVec3(key, vector);
        listener_->notify_listeners(true);
    }

    virtual void setVec4(const std::string& key, glm::vec4 vector) {
        ShaderData::setVec4(key, vector);
        listener_->notify_listeners(true);
         material_dirty_ = true;
    }

    bool hasTexture() const {
        return (main_texture != NULL) || (textures_.size() > 0);
    }

    virtual void setMat4(const std::string& key, glm::mat4 matrix) {
        ShaderData::setMat4(key, matrix);
        listener_->notify_listeners(true);
         material_dirty_ = true;
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
    void setMaterialDirty(bool dirty){
        material_dirty_ = dirty;
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
     
     void createVkMaterialDescriptor(VkDevice &device,VulkanCore* vk){
         vk_descriptor->createDescriptor(device,vk,MATERIAL_UBO_INDEX,VK_SHADER_STAGE_FRAGMENT_BIT);
    }
    Descriptor* getDescriptor(){
        return vk_descriptor;
    }
    std::string getType(std::string type){
        if(type.empty()){
            LOGE("ERROR: type cannot be empty");
            return type;
        }

        if (type.find("int") != std::string::npos)
            return "int4";
        if (type.find("float") != std::string::npos)
            return "float4";

    }

    void convertDescriptor(std::string& uniform_desc){
            const char* p = uniform_desc.c_str();
            const char* type_start;
            int type_size;
            const char* name_start;
            int name_size;
            while (*p)
            {
                while (std::isspace(*p) || *p == ';' || *p == ',')
                    ++p;
                type_start = p;
                if (*p == 0)
                    break;
                while (std::isalnum(*p))
                    ++p;
                type_size = p - type_start;
                if (type_size == 0)
                {
                    LOGE("UniformBlock: SYNTAX ERROR: expecting data type material\n");
                    break;
                }
                std::string type(type_start, type_size);
                std::string modified_type;
                if(type.compare("float4") == 0 || type.compare("int4") == 0 || type.compare("mat4") == 0)
                    modified_type = type;
                else
                    modified_type = getType(type);

                uniform_desc_ = uniform_desc_ + modified_type + " ";
                while (std::isspace(*p))
                    ++p;
                name_start = p;
                while (std::isalnum(*p) || (*p == '_') || (*p == '[') || (*p == ']'))
                    ++p;

                name_size = p - name_start;
                std::string name(name_start, name_size);

                uniform_desc_ = uniform_desc_ + name + "; ";
            }
    }
   void setUniformDesc(std::string uniform_desc){
        convertDescriptor(uniform_desc);
       LOGE("setting matertial descriptor %s", uniform_desc_.c_str());
        vk_descriptor = new Descriptor(uniform_desc_);
    }
    bool isMaterialDirty(){
        return material_dirty_;
    }
      GLUniformBlock* bindUbo(int program_id, int index, const char* name, const char* desc){
                       GLUniformBlock* ubo = new GLUniformBlock(desc);
                       ubo->setGLBindingPoint(index);
                       ubo->setBlockName(name);
                       ubo->bindBuffer(program_id);
                       return ubo;
             }
             void bindMaterialUbo(int program_id){
                 if(mat_ubo_ == nullptr){
                     mat_ubo_ = bindUbo(program_id,MATERIAL_UBO_INDEX,"Material_ubo",uniform_desc_.c_str() );
                 }
                 else
                     mat_ubo_->bindBuffer(program_id);
             }
         GLUniformBlock* getMatUbo(){
            return mat_ubo_;
         }
private:
    GLUniformBlock *mat_ubo_;
    bool material_dirty_;
    std::string uniform_desc_;
    bool ubo_init = false;
    Material(const Material& material);
    Material(Material&& material);
    Material& operator=(const Material& material);
    Material& operator=(Material&& material);

private:
    Descriptor* vk_descriptor;
    Listener* listener_;
    Texture* main_texture = NULL;
    unsigned int shader_feature_set_;
};
}
#endif
