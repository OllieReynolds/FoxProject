#version 460 core
out vec4 FragColor;

smooth in vec3 GeomFragPos;  // Input from the geometry shader
smooth in vec2 GeomTexCoord;  // Input from the geometry shader
in vec2 TexCoord;
flat in vec3 faceNormal;

uniform vec3 lightPos;
uniform sampler2D foxTex;

// Constants for attenuation
float constant = 1.0;
float linear = 0.00005;
float quadratic = 0.0002;

// Ambient term (Le in Rendering Equation)
vec3 ambientTerm = vec3(0.5);

// Light properties
vec3 lightColor = vec3(1.0f, 1.0f, 1.0f);

void main()
{
    vec3 norm = normalize(faceNormal);
    vec3 lightDir = normalize(lightPos - GeomFragPos);
	vec3 viewDir = normalize(-GeomFragPos);

    // Calculate attenuation
    float distance = length(lightPos - GeomFragPos);
    float attenuation = 1.0 / (constant + linear * distance + quadratic * (distance * distance));

    // Compute incoming light
    vec3 Li = lightColor * attenuation;

    // Compute the Rendering Equation
    vec3 brdf = vec3(max(0.0, dot(norm, lightDir)));  // Using Lambertian reflection
    vec3 Lo = ambientTerm  + brdf * Li * max(0.0, dot(norm, lightDir));

    vec3 textureColor = texture(foxTex, GeomTexCoord).rgb;

    // Final color
    vec3 finalColor = Lo * textureColor;
    FragColor = vec4(finalColor, 1.0);
}
