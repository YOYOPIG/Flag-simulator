#version 330
layout(location=0) out vec4 color;

uniform sampler2D text;
uniform vec3 light_color;
uniform vec3 object_color;
uniform vec3 light_pos;
uniform vec3 eye_pos;
uniform bool blinn_phong_on;
uniform float shininess;

in vec3 g_position;
in vec2 g_uv;
in vec3 g_normal;

void main()
{
	// Set default color
	//color = vec4(object_color, 1.0)*texture(text, g_uv);
    color = vec4(object_color, 1.0);
	
    // Ambient
    vec3 ambient = 0.1 * color.xyz;

    // Diffuse
    vec3 light_dir = normalize(light_pos - g_position);
    vec3 normal = normalize(g_normal);
    float diff = max(dot(light_dir, normal), 0.0);
    vec3 diffuse = diff * color.xyz;

    // Specular
    vec3 view_dir = normalize(eye_pos - g_position);
    float spec = 0.0;
    if(blinn_phong_on)
    {
		// Use n,h instead of v,r 
        vec3 halfway = normalize(light_dir + view_dir);  
        spec = pow(max(dot(normal, halfway), 0.0), shininess);
    }
    else
    {
        vec3 reflect_dir = reflect(-light_dir, normal);
        spec = pow(max(dot(view_dir, reflect_dir), 0.0), shininess);
    }
    vec3 specular = light_color * spec; 
    color = vec4(ambient + diffuse + specular, 1.0f);
}