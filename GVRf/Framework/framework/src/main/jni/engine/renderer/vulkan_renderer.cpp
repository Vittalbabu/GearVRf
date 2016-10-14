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
 * Renders a scene, a screen.
 ***************************************************************************/

#include "renderer.h"
#include "gl/gl_program.h"
#include "glm/gtc/matrix_inverse.hpp"

#include "eglextension/tiledrendering/tiled_rendering_enhancer.h"
#include "objects/material.h"
#include "objects/post_effect_data.h"
#include "objects/scene.h"
#include "objects/scene_object.h"
#include "objects/components/camera.h"
#include "objects/components/render_data.h"
#include "objects/textures/render_texture.h"
#include "objects/mesh.h"
#include "shaders/shader_manager.h"
#include "shaders/post_effect_shader_manager.h"
#include "util/gvr_gl.h"
#include "util/gvr_log.h"
#include "batch_manager.h"
#include "renderer.h"
#include "vulkan_renderer.h"
#include <unordered_map>
#include <unordered_set>
#include "objects/uniform_block.h"
namespace gvr {
     void VulkanRenderer::renderCamera(Scene* scene, Camera* camera,
             ShaderManager* shader_manager,
             PostEffectShaderManager* post_effect_shader_manager,
             RenderTexture* post_effect_render_texture_a,
             RenderTexture* post_effect_render_texture_b) {


            std::vector <VkDescriptorSet> allDescriptors;
              //  LOGI("VK calling draw %d", render_data_vector.size());
              int swapChainIndex = vulkanCore_->AcquireNextImage();

                for(auto &rdata : render_data_vector) {
               //     LOGI("VK calling rdata");
                // Creating and initializing Uniform Buffer for Each Render Data
                if(rdata->uniform_dirty){
                const std::vector<glm::vec3>& vertices=  rdata->mesh()->vertices();
                 const std::vector<unsigned short> & indices =  rdata->mesh()->triangles();

               rdata->getVkData().createDescriptor(vulkanCore_->getDevice(),vulkanCore_);

                rdata->material(0)->createDescriptor(vulkanCore_->getDevice(),vulkanCore_);

              vulkanCore_->InitLayoutRenderData(rdata);
                GVR_VK_Vertices& vert = rdata->getVkData().getVkVertices();
               GVR_VK_Indices& indices1 = rdata->getVkData().getVkIndices();
                vulkanCore_->InitVertexBuffersFromRenderData(vertices, vert, indices1, indices);

               //  VulkanUniformBlock& transform_ubo = rdata->getTransformUBO();
              //  transform_ubo.createBuffer(vulkanCore_->getDevice(),vulkanCore_);
              //  GVR_Uniform& transformUbo = transform_ubo.getBuffer();

                vulkanCore_->InitDescriptorSetForRenderData( rdata);
                vulkanCore_->InitPipelineForRenderData(vert, rdata);
                vulkanCore_->updateMaterialUniform(scene,camera, rdata);
                rdata->uniform_dirty = false;
                }

                    allDescriptors.push_back(rdata->getVkData().m_descriptorSet);
                    vulkanCore_->UpdateUniforms(scene,camera, rdata);


                }
                 vulkanCore_->BuildCmdBufferForRenderData(allDescriptors, swapChainIndex, render_data_vector);
                vulkanCore_->DrawFrameForRenderData(swapChainIndex);

     }

}