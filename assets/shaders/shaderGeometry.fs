#version 330 core
layout (location = 0) out vec3 gPosition;
layout (location = 1) out vec3 gNormal;
layout (location = 2) out vec4 gAlbedoSpec;

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;    
    float shininess;
};


in vec2 TexCoords;
in vec3 FragPos;
in vec3 Normal;

uniform vec3 colour;
uniform float albedo;

void main()
{    
    // fragment position gbuffer
    gPosition = FragPos;
    // per-fragment normals gbuffer
    gNormal = normalize(Normal);
    // diffuse per-fragment color
    gAlbedoSpec.rgb = colour;
    // specular intensity in gAlbedoSpec's alpha component
    gAlbedoSpec.a = albedo;
}
