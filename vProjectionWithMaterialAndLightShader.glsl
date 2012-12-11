#version 150 

in vec4 vPosition;
in vec3 vNormal;
in vec2 vTextureCoords;

uniform mat4 ModelView;
uniform mat4 Projection;

out vec4 fPosition;
out vec3 fNormal;
out vec2 fTexcoord;

void main() 
{
	fPosition = vPosition;
    gl_Position = Projection * ModelView * vPosition;
    
    fNormal = vNormal;
    fTexcoord = vTextureCoords;
} 
