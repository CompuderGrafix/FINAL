/**
 * From the OpenGL Programming wikibook: http://en.wikibooks.org/wiki/OpenGL_Programming
 * This file is in the public domain.
 * Contributors: Martin Kraus, Sylvain Beucler
 */
 
in  vec4 vPosition;
in  vec3 vNormal;
in  vec4 vColor;
varying vec4 color;
varying vec4 position;  // position of the vertex (and fragment) in world space
varying vec3 varyingNormalDirection;  // surface normal vector in world space

uniform vec4 LightAmbient, LightDiffuse, LightSpecular;
uniform vec4 LightDiffuse2, LightSpecular2;
uniform vec4 MaterialAmbient, MaterialSpecular;
uniform mat4 ModelView;
uniform mat4 Projection;
uniform vec4 LightPosition;
uniform vec4 LightPosition2;
uniform float Shininess;

void main()
{
  position = ModelView * vPosition;
  varyingNormalDirection = normalize( ModelView*vec4(vNormal, 0.0) ).xyz;

  color = vColor;
  gl_Position = Projection * ModelView * vPosition;
}
