#version 130

#define rot(a) mat2(cos(a),sin(a),-sin(a),cos(a))


uniform float iTime;        // Declare iTime as a uniform float
uniform vec3 iResolution;   // Declare iResolution as a uniform vec3

void mainImage( out vec4 O, vec2 u )
{
    float t = (iTime * 0.25) *.01+.25, s=.1, f=1. ,a,l;
    vec3  R = iResolution,p,
          D = vec3( ( u - .5*R.xy ) / R.x*.4 , .5 ),
          o = vec3(1,.5,.5) + vec3(t+t, t, -2);

    O -= O;

    mat2 r1 = rot(.5);
    D.xz *= r1; o.xz *= r1;

    for (int i,r=0; r++<10; f*=.73, s+=.1 ) {
        p = abs( mod( o + s*D ,1.7) -.85 );
        a = t = 0.;
        for ( i=0; i++<15; t=l )
            l = length( p= abs(p)/dot(p,p) - .53 ),
            a += abs(l-t);

        a *= a*a;
        r>7 ? f *= min(1., .7 + a*a*.001 ) : f;
        O.rgb += f + s*vec3(1,s,s*s*s)*a*.0015*f;
    }

    O = .0085*O + .0015*length(O);
}

void main() {
    mainImage(gl_FragColor, gl_FragCoord.xy);
}
