#version 460 core
out vec4 FragColor;

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoord;

uniform vec3 lightPos;
uniform sampler2D foxTex;

// Light properties
vec3 lightColor = vec3(1.0f, 1.0f, 1.0f);

void main()
{
    // Ambient
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * lightColor;

    // Diffuse
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

    vec3 textureColor = texture(foxTex, TexCoord).rgb;

    vec3 result = (ambient + diffuse) * textureColor;
    FragColor = vec4(result, 1.0);
}
