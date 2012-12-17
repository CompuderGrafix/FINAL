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
uniform float fTime;

float PI = 4.0 * atan(1.0);

void main() 
{ 
	float theta = PI * fTime / 1000.0;
	
	//spin lights not bound to camera around world Y... reverse directions on lights w/ index == 1%2
	mat4 spinInSky[2];
	spinInSky[0] = mat4(vec4(cos(theta), 0.0, -sin(theta), 0.0),	//column1
							 vec4(0.0,1.0,0.0,0.0),						//column2
							 vec4(sin(theta), 0.0, cos(theta), 0.0),	//column3
							 vec4(0.0,0.0,0.0,1.0));					//column4
	spinInSky[1] = mat4(vec4(cos(-theta/3.0), 0.0, -sin(-theta/3.0), 0.0),	//column1
							 vec4(0.0,1.0,0.0,0.0),						//column2
							 vec4(sin(-theta/3.0), 0.0, cos(-theta/3.0), 0.0),	//column3
							 vec4(0.0,0.0,0.0,1.0));					//column4
						 
						 

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
			lightPos = (ModelView * spinInSky[index%2] * LightPosition[index]).xyz;
		} else {
			lightPos = (LightPosition[index]).xyz;
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

