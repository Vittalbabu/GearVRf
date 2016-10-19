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


#include "objects/hybrid_object.h"
#include "objects/components/render_data.h"
#include "objects/render_pass.h"
#include "engine/renderer/renderer.h"
namespace gvr {
extern bool use_multiview;
RenderData:: RenderData() :
            Component(RenderData::getComponentType()), mesh_(0), light_(0), use_light_(
                    false), use_lightmap_(false), batching_(true), render_mask_(
                    DEFAULT_RENDER_MASK), batch_(nullptr), rendering_order_(
                    DEFAULT_RENDERING_ORDER), hash_code_dirty_(true), offset_(
                    false), offset_factor_(0.0f), offset_units_(0.0f), depth_test_(
                    true), alpha_blend_(true), alpha_to_coverage_(false), sample_coverage_(
                    1.0f), invert_coverage_mask_(GL_FALSE), draw_mode_(
                    GL_TRIANGLES), texture_capturer(0), shaderID_(0), renderdata_dirty_(true), gl_ubo_(nullptr){
         if(use_multiview)
            uniform_desc_ = " mat4 u_view_[2]; mat4 u_mvp_[2]; mat4 u_mv_[2]; mat4 u_mv_it_[2]; mat4 u_model;";
          else
            uniform_desc_ = " mat4 u_view; mat4 u_mvp; mat4 u_mv; mat4 u_mv_it; mat4 u_model;";
    }
void RenderData::add_pass(RenderPass* render_pass) {
    render_pass_list_.push_back(render_pass);
    render_pass->add_listener(this);
    renderdata_dirty_ = true;
}

const RenderPass* RenderData::pass(int pass) const {
    if (pass >= 0 && pass < render_pass_list_.size()) {
        return render_pass_list_[pass];
    }
    return nullptr;
}

void RenderData::set_mesh(Mesh* mesh) {
    if(mesh_)
        mesh_->remove_listener(this);
    mesh_ = mesh;
    mesh->add_listener(this);
    renderdata_dirty_ = true;
}
void RenderData::set_renderdata_dirty(bool dirty_){
    renderdata_dirty_ = dirty_;
}
bool RenderData::cull_face(int pass) const {
    if (pass >= 0 && pass < render_pass_list_.size()) {
        return render_pass_list_[pass]->cull_face();
    }
    return nullptr;
}

void RenderData::set_cull_face(int cull_face, int pass) {
    if (pass >= 0 && pass < render_pass_list_.size()) {
        render_pass_list_[pass]->set_cull_face(cull_face);
        renderdata_dirty_ = true;
    }
}
Material* RenderData::material(int pass) const {
    if (pass >= 0 && pass < render_pass_list_.size()) {
        return render_pass_list_[pass]->material();
    }
    return nullptr;
}

void RenderData::set_material(Material* material, int pass) {
    if (pass >= 0 && pass < render_pass_list_.size()) {
        render_pass_list_[pass]->set_material(material);
        renderdata_dirty_ = true;
    }
}
bool compareRenderDataByShader(RenderData* i, RenderData* j) {
    // Compare renderData by their material's shader type
    // Note: multi-pass renderData is skipped for now and put to later position,
    // since each of the passes has a separate material (and shader as well).
    // An advanced sorting may be added later to take multi-pass into account
    if (j->pass_count() > 1) {
        return true;
    }

    if (i->pass_count() > 1) {
        return false;
    }

    return i->get_shader() < j->get_shader();
}
bool compareRenderDataByOrderShaderDistance(RenderData* i,
            RenderData* j) {
    //1. rendering order needs to be sorted first to guarantee specified correct order
    if (i->rendering_order() == j->rendering_order()) {

        if (i->get_shader() == j->get_shader()) {

                int no_passes1 = i->pass_count();
                int no_passes2 = j->pass_count();

                if(no_passes1 == no_passes2){

                   for(int pass=0; pass < no_passes1; pass++){

                        if (i->material(pass) == j->material(pass)) {

                            if(i->cull_face(pass) == j->cull_face(pass)){

                                if (i->getHashCode().compare(j->getHashCode()) == 0) {

                                      // if it is a transparent object, sort by camera distance from back to front
                                    if (i->rendering_order() >= RenderData::Transparent
                                           && i->rendering_order() < RenderData::Overlay) {
                                        return i->camera_distance() > j->camera_distance();
                                    }
                                        // otherwise sort from front to back
                                    return i->camera_distance() < j->camera_distance();
                                }
                                return i->getHashCode() < j->getHashCode();
                            }
                            return i->cull_face(pass) < j->cull_face(pass);
                        }
                        return i->material(pass) < j->material(pass);
                    }
               }
               return no_passes1 < no_passes2;
            }
            return i->get_shader() < j->get_shader();
        }
        return i->rendering_order() < j->rendering_order();
    }
}