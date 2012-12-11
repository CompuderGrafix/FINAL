#version 150

in vec4 color;
in vec4 fPosition;
in vec3 fNormal;
in vec2 fTexcoord;
uniform sampler2D mytexture;

uniform vec4 LightAmbient, LightDiffuse, LightSpecular;
uniform vec4 LightDiffuse2, LightSpecular2;
uniform vec4 MaterialAmbient, MaterialDiffuse, MaterialSpecular;
uniform mat4 ModelView;
uniform mat4 Projection;
uniform vec4 LightPosition;
uniform vec4 LightPosition2;
uniform float MaterialShininess;
uniform bool UseTexture;

void main() 
{ 
    // Transform vertex position into eye coordinates
    vec3 pos = (ModelView * fPosition).xyz;
	vec3 lightPos = LightPosition.xyz;
	vec3 lightPos2 = (ModelView * LightPosition2).xyz;
	
    vec3 L = normalize( lightPos - pos );
    vec3 E = normalize( -pos );
    vec3 H = normalize( L + E );
    
    vec3 L2 = normalize( lightPos2 - pos );
    vec3 H2 = normalize( L2 + E );

    // Transform vertex normal into eye coordinates
    vec3 N = normalize( ModelView*vec4(fNormal, 0.0) ).xyz;

    // Compute terms in the illumination equation
    vec4 ambient = LightAmbient * MaterialAmbient;
    
    vec4 tempColor;
    if(UseTexture) {
		vec2 flipped_texcoord = vec2(fTexcoord.x, 1.0 - fTexcoord.y);
		tempColor = texture2D(mytexture, flipped_texcoord);
    } else {
    	tempColor = MaterialDiffuse;
    }

    float Kd = max( dot(L, N), 0.0 );
    vec4  diffuse = Kd * LightDiffuse * tempColor;

    float Ks = pow( max(dot(N, H), 0.0), MaterialShininess );
    vec4  specular = Ks * LightSpecular * MaterialSpecular;
    
    if( dot(L, N) < 0.0 ) {
        specular = vec4(0.0, 0.0, 0.0, 1.0);
    } 
    
    float Kd2 = max( dot(L2, N), 0.0 );
    vec4  diffuse2 = Kd2*LightDiffuse2 * tempColor;

    float Ks2 = pow( max(dot(N, H2), 0.0), MaterialShininess );
    vec4  specular2 = Ks2 * LightSpecular2 * MaterialSpecular;
    
    if( dot(L2, N) < 0.0 ) {
        specular2 = vec4(0.0, 0.0, 0.0, 1.0);
    } 
    
    gl_FragColor = ambient + diffuse + specular + diffuse2 + specular2;
    gl_FragColor.a = 1.0;

    //gl_FragColor = color;
	//vec2 flipped_texcoord = vec2(f_texcoord.x, 1.0 - f_texcoord.y);
	//gl_FragColor = texture2D(mytexture, flipped_texcoord);
} 

