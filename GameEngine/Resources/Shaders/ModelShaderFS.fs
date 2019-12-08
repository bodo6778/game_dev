precision highp float;

varying vec2 v_uv;
varying vec4 v_pos;
varying vec4 v_Wnorm;

uniform vec3 u_camera;
uniform float u_fog_r;
uniform float u_fog_R;
uniform vec3 u_fog_color;
uniform float u_is_reflected;

uniform sampler2D u_texture_0;
uniform samplerCube u_texture_skybox;

void main()
{
	vec4 obj_color = texture2D(u_texture_0, v_uv);

	// Discard if transparent
	if (obj_color.a < 0.1)
	{
		discard;
	}

	// Apply reflection
	if (u_is_reflected != 0.0)
	{
		vec3 vec_camera = v_pos.xyz - u_camera;
		vec3 dirReflect = reflect(normalize(vec_camera), normalize(v_Wnorm.xyz));
		obj_color = obj_color * 0.5 + textureCube(u_texture_skybox, dirReflect) * 0.5;
		obj_color.w = 1.0;
	}

	// Apply fog
	float distance  = distance(vec4(u_camera, 1.0), obj_color);
	float alpha = (clamp(distance, u_fog_r, u_fog_R) - u_fog_r) / (u_fog_R - u_fog_r); 
	gl_FragColor = alpha * vec4(u_fog_color, 1.0) + (1.0 - alpha) * obj_color;	
}