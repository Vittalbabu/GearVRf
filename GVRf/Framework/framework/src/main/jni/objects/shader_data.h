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
 * Data for doing a post effect on the scene.
 ***************************************************************************/

#ifndef SHADER_DATA_H_
#define SHADER_DATA_H_

#include <map>
#include <memory>
#include <string>

#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "objects/hybrid_object.h"
#include "objects/textures/texture.h"

namespace gvr {
class Texture;

class ShaderData: public HybridObject {
public:
    ShaderData() :
            native_shader_(0), textures_(), floats_(), vec2s_(), vec3s_(), vec4s_(), mat4s_() { }

    ~ShaderData() {
    }

    int get_shader() {
        return native_shader_;
    }

    void set_shader(long shader) {
        native_shader_ = shader;
    }

    Texture* getTexture(const std::string& key) const {
        auto it = textures_.find(key);
        if (it != textures_.end()) {
            return it->second;
        } else {
            std::string error = "Material::getTexture() : " + key
                    + " not found";
            throw error;
        }
    }

    //A new api to return a texture even it is NULL without throwing a error,
    //otherwise it will be captured abruptly by the error handler
    Texture* getTextureNoError(const std::string& key) const {
        auto it = textures_.find(key);
        if (it != textures_.end()) {
            return it->second;
        } else {
            return NULL;
        }
    }

    virtual void setTexture(const std::string& key, Texture* texture) {
        textures_[key] = texture;
        //By the time the texture is being set to its attaching material, it is ready
        //This is guaranteed by upper java layer scheduling
        texture->setReady(true);
    }

    float getFloat(const std::string& key) {
        auto it = floats_.find(key);
        if (it != floats_.end()) {
            return it->second;
        } else {
            std::string error = "Material::getFloat() : " + key + " not found";
            throw error;
        }
    }

    virtual void setFloat(const std::string& key, float value) {
        floats_[key] = value;
    }

    glm::vec2 getVec2(const std::string& key) {
        auto it = vec2s_.find(key);
        if (it != vec2s_.end()) {
            return it->second;
        } else {
            std::string error = "Material::getVec2() : " + key + " not found";
            throw error;
        }
    }

    virtual void setVec2(const std::string& key, glm::vec2 vector) {
        vec2s_[key] = vector;
    }

    glm::vec3 getVec3(const std::string& key) {
        auto it = vec3s_.find(key);
        if (it != vec3s_.end()) {
            return it->second;
        } else {
            std::string error = "Material::getVec3() : " + key + " not found";
            throw error;
        }
    }

    virtual void setVec3(const std::string& key, glm::vec3 vector) {
        vec3s_[key] = vector;
    }

    glm::vec4 getVec4(const std::string& key) {
        auto it = vec4s_.find(key);
        if (it != vec4s_.end()) {
            return it->second;
        } else {
            std::string error = "Material::getVec4() : " + key + " not found";
            throw error;
        }
    }

    virtual void setVec4(const std::string& key, glm::vec4 vector) {
        vec4s_[key] = vector;
    }

    glm::mat4 getMat4(const std::string& key) {
        auto it = mat4s_.find(key);
        if (it != mat4s_.end()) {
            return it->second;
        } else {
            std::string error = "Material::getMat4() : " + key + " not found";
            throw error;
        }
    }

    virtual bool hasTexture() const {
        return (textures_.size() > 0);
    }

    bool hasUniform(const std::string& key) const {
        if (vec3s_.find(key) != vec3s_.end()) {
            return true;
        }
        if (vec2s_.find(key) != vec2s_.end()) {
            return true;
        }
        if (vec4s_.find(key) != vec4s_.end()) {
            return true;
        }
        if (mat4s_.find(key) != mat4s_.end()) {
            return true;
        }
        if (floats_.find(key) != floats_.end()) {
            return true;
        }
        return false;
    }

    virtual void setMat4(const std::string& key, glm::mat4 matrix) {
        mat4s_[key] = matrix;
    }

    bool areTexturesReady() {
        for (auto it = textures_.begin(); it != textures_.end(); ++it) {
            Texture* tex = it->second;
            if ((tex == NULL) || !tex->isReady())
                return false;
        }
        return true;
    }

private:
    ShaderData(const ShaderData& post_effect_data);
    ShaderData(ShaderData&& post_effect_data);
    ShaderData& operator=(const ShaderData& post_effect_data);
    ShaderData& operator=(ShaderData&& post_effect_data);

protected:
    long native_shader_;
    std::map<std::string, Texture*> textures_;
    std::map<std::string, float> floats_;
    std::map<std::string, glm::vec2> vec2s_;
    std::map<std::string, glm::vec3> vec3s_;
    std::map<std::string, glm::vec4> vec4s_;
    std::map<std::string, glm::mat4> mat4s_;
};

}
#endif
