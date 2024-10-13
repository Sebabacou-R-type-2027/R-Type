#version 130

uniform float iTime;        // Declare iTime as a uniform float
uniform vec3 iResolution;   // Declare iResolution as a uniform vec3

void mainImage( out vec4 O, vec2 u )
{
    O = vec4(0,0,0,1);
}

void main() {
    mainImage(gl_FragColor, gl_FragCoord.xy);
}