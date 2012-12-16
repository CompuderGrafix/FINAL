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

uniform bool modulate;
uniform float time;

float PI = 4.0 * acos(1.0/sqrt(2.0));
float magnitude = 1.0;
float period = 10.0;

void main() 
{	
	vec4 vModulated = (vPosition).xyzw; 
	if (modulate)
	{
		vModulated.z = vModulated.x;
	}
    gl_Position = Projection * ModelView * ModelOrientation * vModulated;
    fPosition = ModelView * ModelOrientation * vModulated;
    
    // Transform vertex normal into eye coordinates
    fNormal = normalize(ModelView * ModelOrientation *vec4(vNormal, 0.0)).xyz;
    
    fTexcoord = vTextureCoords;
} 
