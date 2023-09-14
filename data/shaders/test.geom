#version 460 core
layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

smooth in vec3 FragPos[];  // Input from the vertex shader
smooth in vec2 TexCoord[]; 
smooth out vec3 GeomFragPos;  // Output for the fragment shader
smooth out vec2 GeomTexCoord;  // Output for the fragment shader

flat out vec3 faceNormal;  // Flat out variable, will not be interpolated

void main() {
    vec3 a = FragPos[0];
    vec3 b = FragPos[1];
    vec3 c = FragPos[2];

    // Calculate the normal
    faceNormal = normalize(cross(b - a, c - b));

    for(int i = 0; i < 3; i++) {
        GeomFragPos = FragPos[i];  // Pass through FragPos
		GeomTexCoord = TexCoord[i];  // Pass through TexCoord
        gl_Position = gl_in[i].gl_Position;
        EmitVertex();
    }
    EndPrimitive();
}
