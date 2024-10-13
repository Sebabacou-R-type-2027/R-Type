#version 130

uniform float iTime;        // Time uniform
uniform vec3 iResolution;   // Resolution uniform

#define iterations 18
#define formuparam 0.53

#define volsteps 10
#define stepsize 0.05

#define zoom   0.800
#define tile   0.850
#define speed  0.002

#define brightness 0.002
#define darkmatter 0.9
#define distfading 0.730
#define saturation 0.9

out vec4 fragColor; // Output fragment color

void mainImage( out vec4 fragColor, in vec2 fragCoord )
{
    vec2 uv = fragCoord.xy / iResolution.xy;
    uv.y*=iResolution.y/iResolution.x;

	vec3 dir=vec3(uv*zoom,1.);
	float time=iTime*speed+.25;

    float a1=0.5;
	float a2=0.5;
	mat2 rot1=mat2(cos(a1),sin(a1),-sin(a1),cos(a1));
	mat2 rot2=mat2(cos(a2),sin(a2),-sin(a2),cos(a2));
	dir.xz*=rot1;
	dir.xy*=rot2;
	vec3 from=vec3(1.,.5,0.5);
	from+=vec3(time*2.,time,-2.);
	from.xz*=rot1;
	from.xy*=rot2;

    float s=0.1,fade=1.;
	vec3 v=vec3(0.);

	for (int r=0; r<volsteps; r++) {
		vec3 p=from+s*dir*.5;
		p = abs(vec3(tile)-mod(p,vec3(tile*2.))); // tiling fold
		float pa,a=pa=0.;
		for (int i=0; i<iterations; i++) {
			p=abs(p)/dot(p,p)-formuparam; // the magic formula
			a+=abs(length(p)-pa); // absolute sum of average change
			pa=length(p);
		}
		float dm=max(0.,darkmatter-a*a*.0005); //dark matter
		a*=a*a; // add contrast
		if (r>6) fade*=1.-dm; // dark matter, don't render near
		//v+=vec3(dm,dm*.5,0.);
		v+=fade;
		v+=vec3(s,s*s,s*s)*a*brightness*fade; // coloring based on distance
		fade*=distfading; // distance fading
		s+=stepsize;
	}
	mix(vec3(length(v * v)),v,saturation); //color adjust
	fragColor = vec4(v*.006,1.);

}


void main() {
    mainImage(fragColor, gl_FragCoord.xy);
}
