#version 150
/// @file Colour.fs
/// @brief a basic unshaded solid colour shader
/// @brief the colour to shade draw with
uniform vec4 Colour;
out vec4 fragColour;

void main ()
{
  fragColour = Colour;
}

