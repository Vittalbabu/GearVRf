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
 * A shader which an user can add in run-time.
 ***************************************************************************/

#include "custom_shader.h"
#include "engine/renderer/renderer.h"
#include "gl/gl_program.h"
#include "objects/shader_data.h"
#include "objects/scene.h"
#include "objects/mesh.h"
#include "objects/textures/texture.h"
#include "objects/components/render_data.h"
#include "util/gvr_gl.h"

#include <sys/time.h>


namespace gvr {
    class UniformLocation : public Shader::ShaderVisitor
    {
    private:
        ShaderData* material_;

    public:
         UniformLocation(Shader* shader) : ShaderVisitor(shader), material_(NULL) { }
        void visit(const std::string& key, const std::string& type, int size);
    };

    class UniformUpdate : public Shader::ShaderVisitor
    {
    private:
        ShaderData* material_;

    public:
        UniformUpdate(Shader* shader, ShaderData* material) : ShaderVisitor(shader), material_(material) { }
        void visit(const std::string& key, const std::string& type, int size);
    };

    class TextureUpdate : public Shader::ShaderVisitor
    {
    private:
        ShaderData* material_;

    public:
        int     TextureIndex;
        bool    AllTexturesAvailable = true;
        TextureUpdate(Shader* shader, ShaderData* material) : ShaderVisitor(shader), material_(material), TextureIndex(0) { }
        void setLocation(const std::string& key, const std::string& type, int size);
        void visit(const std::string& key, const std::string& type, int size);
    };

    class AttributeLocation : public Shader::ShaderVisitor
    {
    private:
        Mesh*   mesh_;
    public:
        AttributeLocation(Shader* shader, Mesh* mesh) : ShaderVisitor(shader), mesh_(mesh) { }
        void visit(const std::string& key, const std::string& type, int size);
    };
#if 0
    void UniformUpdate::visit(const std::string& key, const std::string& type, int size)
    {

        int loc = shader_->getLocation(key);
        if (loc < 0)
        {
            LOGE("SHADER::uniform: %s location not found", key.c_str());
            return;
        }


        const float* fv;
        const int* iv;
     //   if (Shader::LOG_SHADER)
     //    LOGE("SHADER::uniform:value: %s location: %d", key.c_str(), loc);
        switch (tolower(type[0]))
        {
            case 'f':
            case 'm':
            fv = material_->getFloatVec(key, size);
            if (fv != NULL) {
                switch (size) {
                    case 1:
                    LOGE("unifrom is %s and value is %f", key.c_str(),*fv);
                    glUniform1fv(loc, 1, fv);
                    break;

                    case 2:

                    glUniform2fv(loc, 1, fv);
                    break;

                    case 3:
                        LOGE("unifrom is %s and value is %f %f %f ", key.c_str(),fv[0],fv[1],fv[2]);
                    glUniform3fv(loc, 1, fv);
                    break;

                    case 4:
                        LOGE("unifrom is %s and value is %f %f %f %f ", key.c_str(),fv[0],fv[1],fv[2],fv[3]);
                    glUniform4fv(loc, 1, fv);
                    break;

                    case 16:

                    glUniformMatrix4fv(loc, 1, 0, fv);
                    break;
                }
            }
            break;

            case 'i':
            iv = material_->getIntVec(key, size);
            if (iv != NULL)
                switch (size)
                {
                    case 1:

                    glUniform1iv(loc, 1, iv);
                    break;

                    case 2:

                    glUniform2iv(loc, 1, iv);
                    break;

                    case 3:

                    glUniform3iv(loc, 1, iv);
                    break;

                    case 4:

                    glUniform4iv(loc, 1, iv);
                    break;
                }
            break;
            }
    }
    #endif
#if 1
    void UniformUpdate::visit(const std::string& key, const std::string& type, int size)
    {
        int loc;

        GLUniformBlock* ubo = material_->getMatUbo();

        const float* fv;
        const int* iv;
        glm::vec4 data(1.0,1.0,1.0,1.0);
     //   if (Shader::LOG_SHADER)
     //    LOGE("SHADER::uniform:value: %s location: %d", key.c_str(), loc);
        switch (tolower(type[0]))
        {
            case 'f':
            case 'm':
            fv = material_->getFloatVec(key, size);
            if (fv != NULL) {
                switch (size) {
                    case 1:
                    data.x = *fv;
                      //  LOGE("uniform is %s and value is %f", key.c_str(),*fv);
                    ubo->setVec(key,glm::value_ptr(data), 4);
                    break;

                    case 2:
                    data.x = *fv;
                    data.y = fv[1];
                      //  LOGE("uniform is %s and value is %f %f", key.c_str(),data[0],data[1]);
                    ubo->setVec(key,glm::value_ptr(data), 4);
                    break;

                    case 3:
                     data.x = fv[0];
                     data.y=  fv[1];
                     data.z = fv[2];
                       // LOGE("uniform is %s and value is %f %f %f", key.c_str(),data[0],data[1], data[2]);
                     ubo->setVec(key,glm::value_ptr(data), 4);
                    break;

                    case 4:
                     //   LOGE("uniform is %s and value is %f %f %f %f", key.c_str(),fv[0],fv[1], fv[2],fv[3]);
                        ubo->setVec(key,fv, 4);
                 //       ubo->setVec(key,glm::value_ptr(data), 4);
                    break;

                    case 16:
                    ubo->setVec(key,fv, 16);
                    break;
                }
            }
            break;

            case 'i':
            iv = material_->getIntVec(key, size);
            if (iv != NULL)
                switch (size)
                {
                    case 1:
                    ubo->setInt(key,*iv);
                    //glUniform1iv(loc, 1, iv);
                    break;

                    case 2:
                    ubo->setIntVec(key,iv,2);
                    //glUniform2iv(loc, 1, iv);
                    break;

                    case 3:
                    ubo->setIntVec(key,iv,3);
                    //glUniform3iv(loc, 1, iv);
                    break;

                    case 4:
                    ubo->setIntVec(key,iv, 4);
                   // glUniform4iv(loc, 1, iv);
                    break;
                }
            break;
            }
    }
#endif
    void UniformLocation::visit(const std::string& key, const std::string& type, int size)
    {
        int loc = shader_->getLocation(key);
        if (loc < 0)
        {
            loc = glGetUniformLocation(shader_->getProgramId(), key.c_str());
            if (loc >= 0) {
                shader_->setLocation(key, loc);
            //    if (Shader::LOG_SHADER) LOGE("SHADER::uniform:location: %s location: %d", key.c_str(), loc);
            }
        }
    }

    void TextureUpdate::visit(const std::string& key, const std::string& type, int size)
    {
        int loc = shader_->getLocation(key);
        if (loc < 0)
        {
            loc = glGetUniformLocation(shader_->getProgramId(), key.c_str());
            if (loc >= 0) {
             //   if (Shader::LOG_SHADER) LOGE("SHADER::texture: %s location not found", key.c_str());
                AllTexturesAvailable = false;
                return;
            }
            shader_->setLocation(key, loc);
        }
        Texture* texture = material_->getTextureNoError(key);
        if (texture != NULL)
        {
            glActiveTexture(GL_TEXTURE0 + TextureIndex);
            glBindTexture(texture->getTarget(), texture->getId());
            glUniform1i(loc, TextureIndex++);
        }
        else
        {
            AllTexturesAvailable = false;
        }
    }

    void AttributeLocation::visit(const std::string& key, const std::string& type, int size)
    {
        int loc = shader_->getLocation(key);
        if (loc < 0)
        {
            loc = glGetAttribLocation(shader_->getProgramId(), key.c_str());
            if (loc >= 0)
            {
                shader_->setLocation(key, loc);
              //  if (Shader::LOG_SHADER) LOGE("SHADER::attribute:location: %s location: %d", key.c_str(), loc);
                switch (size)
                {
                    case 1:
                    mesh_->setVertexAttribLocF(loc, key);
                    break;

                    case 2:
                    mesh_->setVertexAttribLocV2(loc, key);
                    break;

                    case 3:
                    mesh_->setVertexAttribLocV3(loc, key);
                    break;

                    case 4:
                    mesh_->setVertexAttribLocV4(loc, key);
                    break;
                }
            }
        }
    }

const bool Shader::LOG_SHADER = true;

Shader::Shader(int id,
               const std::string& signature,
               const std::string& uniformDescriptor,
               const std::string& textureDescriptor,
               const std::string& vertexDescriptor,
               const std::string& vertex_shader,
               const std::string& fragment_shader)
    : program_(nullptr),
      signature_(signature),
      id_(id),
      uniformDescriptor_(uniformDescriptor),
      textureDescriptor_(textureDescriptor),
      vertexDescriptor_(vertexDescriptor),
      vertexShader_(vertex_shader),
      fragmentShader_(fragment_shader) {

}

void Shader::initializeOnDemand(RenderState* rstate, Mesh* mesh) {
    if (nullptr == program_) {
        std::string modified_frag_shader;
        if(fragmentShader_.find("samplerExternalOES")!= std::string::npos){
            LOGE("inside enabled");
            std::istringstream iss(fragmentShader_.c_str());
            const char* extensions = (const char*) glGetString(GL_EXTENSIONS);
            std::string extension_string;
            if(strstr(extensions, "GL_OES_EGL_image_external_essl3")){
                extension_string = "#extension GL_OES_EGL_image_external_essl3 : require \n";
                LOGE("enabling essl3");
            }
            else {
                extension_string = "#extension GL_OES_EGL_image_external : require\n";
                LOGE("enabling non essl3");
            }
            std::string line;
            while (std::getline(iss, line)){
                if(line.find("GL_OES_EGL_image_external") != std::string::npos){
                    LOGE("found");
                    modified_frag_shader = modified_frag_shader + extension_string + "\n";
                }
                else{
                    LOGE("not found");
                    modified_frag_shader = modified_frag_shader + line + "\n";
                 }
            }

        }
        else {
            modified_frag_shader = fragmentShader_;
        }

        program_ = new GLProgram(vertexShader_.c_str(), modified_frag_shader.c_str());
      //  if (LOG_SHADER) LOGD("SHADER: creating GLProgram %d", program_->id());
        if (use_multiview && !(strstr(vertexShader_.c_str(), "gl_ViewID_OVR")
                               && strstr(vertexShader_.c_str(), "GL_OVR_multiview2")
                               && strstr(vertexShader_.c_str(), "GL_OVR_multiview2"))) {
            std::string error = "Your shaders are not multiview";
            LOGE("Your shaders are not multiview");
            throw error;
        }
        const char* temp =  vertexShader_.c_str();


        if (use_multiview) {
            u_mvp_ = glGetUniformLocation(program_->id(), "u_mvp_[0]");
            u_view_ = glGetUniformLocation(program_->id(), "u_view_[0]");
            u_mv_ = glGetUniformLocation(program_->id(), "u_mv_[0]");
            u_mv_it_ = glGetUniformLocation(program_->id(), "u_mv_it_[0]");
        }
        else {
            u_mvp_ = glGetUniformLocation(program_->id(), "u_mvp");
            u_view_ = glGetUniformLocation(program_->id(), "u_view");
            u_mv_ = glGetUniformLocation(program_->id(), "u_mv");
            u_mv_it_ = glGetUniformLocation(program_->id(), "u_mv_it");
        }
        u_right_ = glGetUniformLocation(program_->id(), "u_right");
        u_model_ = glGetUniformLocation(program_->id(), "u_model");
        vertexShader_.clear();
        fragmentShader_.clear();
        if (LOG_SHADER) LOGD("SHADER: Custom shader added program %d", program_->id());
        if (LOG_SHADER) LOGD("SHADER: getting texture locations");
        UniformLocation uvisit(this);
        {
            std::lock_guard <std::mutex> lock(textureVariablesLock_);
            forEach(textureDescriptor_, uvisit);
        }
        if (LOG_SHADER) LOGD("SHADER: getting uniform locations");
        {
            std::lock_guard <std::mutex> lock(uniformVariablesLock_);
            forEach(uniformDescriptor_, uvisit);
        }
        if (LOG_SHADER) LOGD("SHADER: getting attribute locations");
        {
            std::lock_guard <std::mutex> lock(attributeVariablesLock_);
            AttributeLocation avisit(this, mesh);
            forEach(vertexDescriptor_, avisit);
        }
    }

}


void Shader::forEach(const std::string& descriptor, ShaderVisitor& visitor)
{
    const char *p = descriptor.c_str();
    const char *type_start;
    int type_size;
    const char *name_start;
    int name_size;

    while (*p) {
        while (std::isspace(*p) || std::ispunct(*p))
            ++p;
        type_start = p;
        if (*p == 0)
            break;
        while (std::isalnum(*p))
            ++p;
        type_size = p - type_start;
        if (type_size == 0) {
            LOGE("SHADER: SYNTAX ERROR: expecting data type %s\n", descriptor.c_str());
            break;
        }
        std::string type(type_start, type_size);
        while (std::isspace(*p))
            ++p;
        name_start = p;
        while (std::isalnum(*p) || (*p == '_'))
            ++p;
        name_size = p - name_start;
        if (name_size == 0) {
            LOGE("SHADER: SYNTAX ERROR: expecting name\n");
            break;
        }
        std::string name(name_start, name_size);
        visitor.visit(name, type, calcSize(type));
    }
}

int Shader::calcSize(std::string type)
{
    if (type == "float") return 1;
    if (type == "float3") return 3;
    if (type == "float4") return 4;
    if (type == "float2") return 2;
    if (type == "int") return 1;
    if (type == "int3") return 4;
    if (type == "int4") return 4;
    if (type == "float2") return 2;
    if (type == "mat4") return 16;
    if (type == "mat3") return 12;
    return 0;
}


Shader::~Shader() {
    //if (LOG_SHADER) LOGE("SHADER: deleting shader %s %d %p", signature_.c_str(), id_, this);
    delete program_;
}


void Shader::render(RenderState* rstate, RenderData* render_data, ShaderData* material) {
    LOGE("in render %s", render_data->owner_object()->name().c_str());
    if (!material->areTexturesReady())
    {
        if (LOG_SHADER) LOGE("textures are not ready for %s", render_data->owner_object()->name().c_str());
        return;
    }

    Mesh* mesh = render_data->mesh();
    initializeOnDemand(rstate, mesh);
    if (program_->id() == 0)
    {
        LOGE("SHADER: shader could not be generated %s", signature_.c_str());
    }
    if (LOG_SHADER) LOGD("SHADER: rendering with program %d", program_->id());
    glUseProgram(program_->id());

    /*
     * Bind textures
     */
    int texIndex = 0;
    {
        std::lock_guard<std::mutex> lock(textureVariablesLock_);
        TextureUpdate tvisit(this, material);
        forEach(textureDescriptor_, tvisit);
        texIndex = tvisit.TextureIndex;
        if (!tvisit.AllTexturesAvailable)
        {
            LOGE("textures are not ready for %s", render_data->owner_object()->name().c_str());
            return;
        }
    }
    /*
     * Update the bone matrices
     */
    int a_bone_indices = glGetAttribLocation(program_->id(), "a_bone_indices");
    int a_bone_weights = glGetAttribLocation(program_->id(), "a_bone_weights");
    int u_bone_matrices = glGetUniformLocation(program_->id(), "u_bone_matrix[0]");
    if ((a_bone_indices >= 0) ||
        (a_bone_weights >= 0) ||
        (u_bone_matrices >= 0)) {
        LOGE("bones present");
        glm::mat4 finalTransform;
        mesh->setBoneLoc(a_bone_indices, a_bone_weights);
        mesh->generateBoneArrayBuffers(program_->id());
        int nBones = mesh->getVertexBoneData().getNumBones();
        if (nBones > MAX_BONES)
            nBones = MAX_BONES;
        for (int i = 0; i < nBones; ++i) {
            finalTransform = mesh->getVertexBoneData().getFinalBoneTransform(i);
         //   glUniformMatrix4fv(u_bone_matrices + i, 1, GL_FALSE, glm::value_ptr(finalTransform));
        }
        render_data->bindBonesUbo(program_->id());
        GLUniformBlock* bones_ubo = render_data->getBonesUbo();

        std::vector<glm::mat4>& bone_matrices = mesh->getVertexBoneData().getBoneMatrices();
        bones_ubo->setMat4("u_bone_matrix", &bone_matrices[0][0][0]);
        bones_ubo->render(program_->id());

        checkGlError("Shader after bones");
    }
    /*
     * Update values of uniform variables
     */
     rstate->scene->bindTransformUbo(program_->id());
     GLUniformBlock* transform_ubo =  rstate->scene->getTransformUbo();

     if(use_multiview){
        transform_ubo->setMat4("u_view_", glm::value_ptr(rstate->uniforms.u_view_[0]));
        transform_ubo->setMat4("u_mvp_", glm::value_ptr(rstate->uniforms.u_mvp_[0]));
        transform_ubo->setMat4("u_mv_", glm::value_ptr(rstate->uniforms.u_mv_[0]));
        transform_ubo->setMat4("u_mv_it_", glm::value_ptr(rstate->uniforms.u_mv_it_[0]));
    }
    else {
        transform_ubo->setMat4("u_view", glm::value_ptr(rstate->uniforms.u_view));
        transform_ubo->setMat4("u_mvp", glm::value_ptr(rstate->uniforms.u_mvp));
        transform_ubo->setMat4("u_mv", glm::value_ptr(rstate->uniforms.u_mv));
        transform_ubo->setMat4("u_mv_it", glm::value_ptr(rstate->uniforms.u_mv_it));
    }
    transform_ubo->setMat4("u_model", glm::value_ptr(rstate->uniforms.u_model));
    if(program_ == nullptr || transform_ubo == nullptr)
        LOGE("program or ubo is null");

    transform_ubo->render(program_->id());

    /*
     * Update material uniforms
     */


     Material* mat = static_cast<Material*>(material);
     if(!uniformDescriptor_.empty())
        mat->bindMaterialUbo(program_->id());

     GLUniformBlock* mat_ubo = mat->getMatUbo();

    if(mat && mat->isMaterialDirty())
    {
        std::lock_guard<std::mutex> lock(uniformVariablesLock_);
        UniformUpdate uvisit(this, material);
        forEach(uniformDescriptor_, uvisit);
        mat->setMaterialDirty(false);
    }

   LOGE("calling mat render");
    if(mat_ubo){
        mat_ubo->render(program_->id());
    }

    /*
     * Update the uniforms for the lights
     */
    const std::vector<Light*>& lightlist = rstate->scene->getLightList();
    bool castShadow = false;

    for (auto it = lightlist.begin();
         it != lightlist.end();
         ++it) {
        Light* light = (*it);
         if (light != NULL) {
            light->render(program_->id(), texIndex);
            if (light->castShadow())
                castShadow = true;
         }
    }
    if (castShadow) {
    	Light::bindShadowMap(program_->id(), texIndex);
    }
    checkGlError("Shader::render");
}
} /* namespace gvr */
