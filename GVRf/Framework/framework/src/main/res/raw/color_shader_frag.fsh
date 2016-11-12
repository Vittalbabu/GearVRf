#version 300 es
precision mediump float;

layout (std140) uniform Material_ubo{
    vec4 u_color;
};
out vec4 outColor;
void main()
{
    outColor = vec4(u_color.x,u_color.y,u_color.z, 1);
}