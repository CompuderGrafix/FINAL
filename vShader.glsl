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

float PI = 4.0 * atan(1.0);
float magnitude1 = 0.02;
float magnitude2 = 0.03;
float period1 = 0.5;
float period2 = 1.0;
const int cyclelength = 750; //ms

void main() 
{	
	vec4 vModulated = (vPosition).xyzw; 
	if (modulate)
	{		
		float theta1 = PI * (time / 1000.0 + vPosition.x / period1);
		float theta2 = PI * (time / 1000.0 + vPosition.z / period2);
		
		//make "origin" of periodic floor waves dance back and forth
		int itime = int(time) % (2*cyclelength);
		itime = abs(itime - cyclelength);		
			
		float t = float(itime);
		vModulated.y = vModulated.y + magnitude1 * sin(theta1) * (vModulated.x - 10.0 * (t - cyclelength/2.0) / cyclelength) + magnitude2 * cos(theta2) * (vModulated.z - 10.0 * (t - cyclelength/2.0) / cyclelength);
	}
    gl_Position = Projection * ModelView * ModelOrientation * vModulated;
    fPosition = ModelView * ModelOrientation * vModulated;
    
    // Transform vertex normal into eye coordinates
    fNormal = normalize(ModelView * ModelOrientation *vec4(vNormal, 0.0)).xyz;
    
    fTexcoord = vTextureCoords;
} 
