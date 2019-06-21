#version 330
layout(location = 0) in vec3 position;
layout(location = 1) in vec2 texcoord;
layout(location = 2) in vec3 normal;

uniform mat4 model;
uniform mat4 vp;
uniform sampler2D text;
uniform vec3 light_color;
uniform vec3 object_color;
uniform vec3 light_pos;
uniform vec3 eye_pos;
uniform bool blinn_phong_on;
uniform float shininess;

out vec4 v_color;

void main()
{
	vec3 v_position;
	vec2 v_uv;
	vec3 v_normal;

    v_position = (model*vec4(position, 1.0)).xyz;
    v_uv = texcoord;
    v_normal = (model*vec4(normal, 0.0)).xyz;
    gl_Position = vp*model*vec4(position, 1.0);

	// Set default color
    v_color = vec4(object_color, 1.0); // Color the user select

    // Ambient
    vec3 ambient = 0.1 * v_color.xyz;

    // Diffuse
    vec3 light_dir = normalize(light_pos - v_position);
    vec3 normal = normalize(v_normal);
    float diff = max(dot(light_dir, normal), 0.0);
    vec3 diffuse = diff * v_color.xyz;
	
    // Specular
    vec3 view_dir = normalize(eye_pos - v_position);
    float spec = 0.0;
    if(blinn_phong_on)
    {
        vec3 halfway = normalize(light_dir + view_dir);  
        spec = pow(max(dot(normal, halfway), 0.0), shininess);
    }
    else
    {
        vec3 reflect_dir = reflect(-light_dir, normal);
        spec = pow(max(dot(view_dir, reflect_dir), 0.0), shininess);
    }
    vec3 specular = light_color * spec; 

    v_color = vec4(ambient + diffuse + specular, 1.0f);
}