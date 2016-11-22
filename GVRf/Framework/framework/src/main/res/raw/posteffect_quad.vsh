// Copyright 2015 Samsung Electronics Co., LTD
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

attribute vec3 a_position;
attribute vec2 a_texcoord;

varying vec2 diffuse_coord;
varying vec2 v_overlay_coord;

void main() {
  v_scene_coord = a_texcoord.xy;
  
  // Textures loaded from an Android Bitmap are upside down relative to textures generated by EGL
  v_overlay_coord = vec2(a_texcoord.x, 1.0 - a_texcoord.y);

  gl_Position = vec4(a_position, 1.0);
}
