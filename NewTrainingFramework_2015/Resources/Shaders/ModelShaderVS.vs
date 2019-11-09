attribute vec3 a_posL;
attribute vec3 a_binormL;
attribute vec3 a_normL;
attribute vec3 a_tgtL;
attribute vec2 a_uvL;

uniform mat4 u_matrix;
uniform sampler2D u_texture;

varying vec2 v_uv;

void main()
{
gl_Position = u_matrix * vec4(a_posL, 1.0);
v_uv = a_uvL;
}