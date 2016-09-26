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
package org.gearvrf;

public class GVRLightmapShader extends GVRShader
{
    private String vertexShader =
        "attribute vec3 a_position;\n" +
        "attribute vec3 a_normal;\n" +
        "attribute vec2 a_texcoord;\n" +
        "uniform mat4 u_mvp;\n" +
        "varying vec2 diffuse_coord;\n" +
        "void main() {\n" +
        " vec4 pos = u_mvp * vec4(a_position, 1.0);\n" +
        " diffuse_coord = a_texcoord;\n" +
        " gl_Position = pos;\n" +
        "}";

    private String fragmentShader =
        "varying vec2 coord;\n" +
        "uniform sampler2D u_main_texture;\n" +
        "uniform sampler2D u_lightmap_texture;\n" +
        "uniform vec2  u_lightmap_offset;\n" +
        "uniform vec2  u_lightmap_scale;\n" +
        "void main() {\n" +
        " vec4 color;\n" +
        " vec4 lightmap_color;\n" +
        " vec2 lightmap_coord = (diffuse_coord * u_lightmap_scale) + u_lightmap_offset;\n" +
        // Beast exports the texture with vertical flip
        " lightmap_color = texture2D(u_lightmap_texture, vec2(lightmap_coord.x, 1.0 - lightmap_coord.y));\n" +
        " color = texture2D(u_main_texture, diffuse_coord);\n" +
        " gl_FragColor = color * lightmap_color;\n" +
        "}";

    public GVRLightmapShader()
    {
        super("float2 u_lightmap_offset float2 u_lightmap_scale");
        setSegment("FragmentTemplate", fragmentShader);
        setSegment("VertexTemplate", vertexShader);
    }
}