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

/**
 * Shader which horizontally flips a texture and blends it with a color.
 * This shader assumes the vertex position is in eye coordinates - it
 * does not use the model, view or projection matrices.
 * It also ignores light sources.
 *
 * @<code>
 *     a_position   position vertex attribute
 *     a_texcoord   normal vertex attribute
 *     u_color      color to blend
 *     u_factor     blend factor (0 to 1)
 *     u_texture    texture to blend
 * </code>
 */
public class GVRHorizontalFlipShader extends GVRShaderTemplate
{
    private String vertexShader =
            "attribute vec3 a_position;\n" +
            "attribute vec2 a_texcoord;\n" +
            "varying vec2 diffuse_coord;\n" +
            "void main() {\n" +
            "  diffuse_coord = vec2(a_texcoord.x, 1.0 - a_texcoord.y);\n" +
            "  gl_Position = vec3(a_position, 1.0);\n" +
            "}\n";

    private String fragmentShader  =
            "precision highp float;\n" +
            "uniform sampler2D u_texture;\n" +
            "uniform vec3 u_color;\n" +
            "uniform float u_factor;\n" +
            "varying vec2 diffuse_coord;\n" +
            "void main() {\n" +
            "  vec4 tex = texture2D(u_texture, diffuse_coord);\n"  +
            "  vec3 color = tex.rgb * (1.0 - u_factor) + u_color * u_factor;\n" +
            "  float alpha = tex.a;\n" +
            "  gl_FragColor = vec4(color, alpha);\n" +
            "}\n";

    public GVRHorizontalFlipShader(GVRContext ctx)
    {
        super("float3 u_color float u_factor", "sampler2D u_texture", "float3 a_position float2 a_texcoord");
        setSegment("FragmentTemplate", fragmentShader);
        setSegment("VertexTemplate", vertexShader);
    }

    protected void setMaterialDefaults(GVRShaderData material)
    {
        material.setVec3("u_color", 1, 1, 1);
        material.setFloat("u_factor", 1);
    }
}