#version 150 

in vec4 vPosition;
in vec3 vNormal;
in vec2 vTextureCoords;

uniform mat4 Projection;
uniform mat4 ModelView;
uniform mat4 ModelOrientation;

out vec4 fPosition;
out vec3 fNormal;
out vec2 fTexcoord;

void main() 
{
    gl_Position = Projection * ModelView * ModelOrientation * vPosition;
    fPosition = vPosition;
    
    // Transform vertex normal into eye coordinates
    fNormal = normalize( ModelView * ModelOrientation *vec4(vNormal, 0.0) ).xyz;
    
    fTexcoord = vTextureCoords;
} 
