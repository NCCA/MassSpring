#version 330 core


uniform mat4 MVP;

in vec3 inVert;
in vec2 inUV;
in vec3 inNormal;


void main()
{
  gl_Position = MVP*vec4(inVert,1.0);
}
