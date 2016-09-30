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

import java.util.HashMap;
import java.util.Map;
import java.lang.reflect.*;

import org.gearvrf.GVRShaderTemplate;
import org.gearvrf.GVRContext;
import org.gearvrf.utility.Log;

/**
 * Manages custom shaders, for rendering scene objects.
 *
 * Get the singleton from {@link GVRContext#getMaterialShaderManager()}.
 */
public class GVRShaderManager extends GVRHybridObject
{
    GVRShaderManager(GVRContext gvrContext)
    {
        this(gvrContext, NativeShaderManager.ctor());
    }

    protected GVRShaderManager(GVRContext gvrContext, long ctor)
    {
        super(gvrContext, ctor);
    }

    public int addShader(String signature, String uniformDescriptor, String textureDescriptor, String vertexDescriptor, String vertexShader, String fragmentShader)
    {
        return NativeShaderManager.addShader(getNative(), signature,
                uniformDescriptor, textureDescriptor, vertexDescriptor,
                vertexShader, fragmentShader);
    }

    public int getShader(String signature)
    {
        return NativeShaderManager.getShader(getNative(), signature);
    }

    /**
     * Retrieves the Material Shader ID associated with the
     * given shader template class.
     *
     * A shader template is capable of generating multiple variants
     * from a single shader source. The exact vertex and fragment
     * shaders are generated by GearVRF based on the lights
     * being used and the material attributes. you may subclass
     * GVRShaderTemplate to create your own shader templates.
     *
     * @param shaderClass shader class to find (subclass of GVRShader)
     * @return GVRShaderId associated with that shader template
     * @see GVRShaderTemplate GVRShader
     */
    public GVRShaderId getShaderType(Class<? extends GVRShader> shaderClass)
    {
        GVRShaderId shaderId = mShaderTemplates.get(shaderClass);
        GVRContext ctx = getGVRContext();

        if (shaderId == null)
        {
            shaderId = new GVRShaderId(shaderClass);
            mShaderTemplates.put(shaderClass, shaderId);
            shaderId.getTemplate(ctx);
        }
        return shaderId;
    }

    /**
     * Maps the shader template class to the instance of the template.
     * Only one shader template of each class is necessary since
     * shaders are global.
     */
    protected Map<Class<? extends GVRShader>, GVRShaderId> mShaderTemplates = new HashMap<Class<? extends GVRShader>, GVRShaderId>();
}

class NativeShaderManager {
    static native long ctor();

    static native int addShader(long shaderManager, String signature,
                                String uniformDescriptor, String textureDescriptor, String vertexDescriptor,
                                String vertexShader, String fragmentShader);

    static native int getShader(long shaderManager, String signature);
}
