#version 150

const int numberOfLightsFollowingCamera = 1;
const int numberOfLights = 5;

in vec4 fPosition;
in vec3 fNormal;
in vec2 fTexcoord;
uniform sampler2D mytexture;

uniform vec4 LightAmbient, LightPosition[numberOfLights], LightDiffuse[numberOfLights], LightSpecular[numberOfLights];
uniform vec4 MaterialAmbient, MaterialDiffuse, MaterialSpecular;
uniform mat4 ModelView;
uniform mat4 Projection;
uniform float MaterialShininess;
uniform bool UseTexture;

uniform bool modulate;
uniform float time;

void main() 
{ 
    // Transform vertex position into eye coordinates
    vec3 pos = (fPosition).xyz;

    // Compute terms in the illumination equation
    vec4 ambient = LightAmbient * MaterialAmbient;
    
    // Use texture or material
    vec4 tempColor;
    if(UseTexture) {
		vec2 flipped_texcoord = vec2(fTexcoord.x, 1.0 - fTexcoord.y);
		tempColor = texture2D(mytexture, flipped_texcoord);
    } else {
    	tempColor = MaterialDiffuse;
    }

	gl_FragColor = ambient;

	for(int index = 0; index < numberOfLights; index++) {
	
		vec3 lightPos;
		if(index >= numberOfLightsFollowingCamera) {
			lightPos = (ModelView * LightPosition[index]).xyz;
		} else {
			lightPos = LightPosition[index].xyz;
		}
		
	    vec3 L = normalize(lightPos - pos);
	    vec3 E = normalize(-pos);
	    vec3 H = normalize(L + E);
	    
		float Kd = max( dot(L, fNormal), 0.0 );
	    vec4  diffuse = Kd * LightDiffuse[index] * tempColor;
	
	    float Ks = pow( max(dot(fNormal, H), 0.0), MaterialShininess);
	    vec4  specular = Ks * LightSpecular[index] * MaterialSpecular;
	    if( dot(L, fNormal) < 0.0) {
	        specular = vec4(0.0, 0.0, 0.0, 1.0);
	    } 
	
		gl_FragColor = gl_FragColor + diffuse + specular;
	}
    gl_FragColor.a = 1.0;
} 

