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
float magnitude = 0.1;
float period = 2.0;

void main() 
{	
	vec4 vModulated = (vPosition).xyzw; 
	if (modulate)
	{		
		float theta1 = PI * (time / 1000.0 + 2*vPosition.x / period);
		float theta2 = PI * (time / 1000.0 + 2*vPosition.z / period);
		vModulated.y = vModulated.y + magnitude * sin(theta1) + magnitude * cos(theta2);
	}
    gl_Position = Projection * ModelView * ModelOrientation * vModulated;
    fPosition = ModelView * ModelOrientation * vModulated;
    
    // Transform vertex normal into eye coordinates
    fNormal = normalize(ModelView * ModelOrientation *vec4(vNormal, 0.0)).xyz;
    
    fTexcoord = vTextureCoords;
} 
