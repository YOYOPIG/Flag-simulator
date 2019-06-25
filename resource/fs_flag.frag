#version 330
layout(location=0) out vec4 color;

uniform vec3 light_color;
uniform vec3 object_color;
uniform vec3 light_pos;
uniform vec3 eye_pos;
uniform bool blinn_phong_on;
uniform bool minnaert;
uniform float shininess;

in vec3 g_position;
in vec2 g_uv;
in vec3 g_normal;

void main()
{
	// Set default color
    color = vec4(object_color, 1.0);
	
    // Ambient
    vec3 ambient = 0.1 * color.xyz;

    // Diffuse (Lambert)
    vec3 light_dir = normalize(light_pos - g_position);
    vec3 normal = normalize(g_normal);
    float diff = abs(dot(light_dir, normal));
    vec3 diffuse = diff * color.xyz;

    // Specular
    vec3 view_dir = normalize(eye_pos - g_position);
    float spec = 0.0;
    if(blinn_phong_on)
    {
		// Use n,h instead of v,r 
        vec3 halfway = normalize(light_dir + view_dir);  
        spec = pow(abs(dot(normal, halfway)), shininess);
    }
    else
    {
        vec3 reflect_dir = reflect(-light_dir, normal);
        spec = pow(abs(dot(view_dir, reflect_dir)), shininess);
    }
    vec3 specular = light_color * spec; 
    color = vec4(ambient + diffuse + specular, 1.0f);

    if(minnaert)
    {
        // Minnaert
        float nDotL = dot(normal, light_dir);
        float nDotE = dot(normal, view_dir);
        float darkenCoefficient = 0.5;
        float dist = length(light_dir);
        float intensity = abs(nDotL * pow(nDotL * nDotE, darkenCoefficient) / dist);

        vec3 result = ambient + intensity * diffuse * light_color;

        color = vec4(result, 1.0f);
    }
}
