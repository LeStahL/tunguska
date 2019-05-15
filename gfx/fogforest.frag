/* Endeavor by Team210 - 64k intro by Team210 at Revision 2k19
 * Copyright (C) 2018  Alexander Kraus <nr4@z10.info>
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#version 130

uniform float iTime;
uniform vec2 iResolution;

// Global constants
const float pi = acos(-1.);
const vec3 c = vec3(1.0, 0.0, -1.0);
float a = 1.0;

// Hash function
void rand(in vec2 x, out float num);
void lfnoise(in vec2 t, out float n);
void mfnoise(in vec2 x, in float d, in float b, in float e, out float n);
void dvoronoi(in vec2 x, out float d, out vec2 z);
void smoothmin(in float a, in float b, in float k, out float dst);
void add(in vec2 sda, in vec2 sdb, out vec2 sdf);
void zextrude(in float z, in float d2d, in float h, out float d);
void dbox(in vec2 p, in vec2 b, out float dst);
void line(in vec3 x, in vec3 p1, in vec3 p2, out float dst);
void stroke(in float d, in float s, out float dst);

// Hash function
void rand3(in vec3 x, out float num)
{
    num = fract(sin(dot(sign(x)*abs(x) ,vec3(12.9898,78.233,45.1232)))*43758.5453);
}

// Arbitrary-frequency 2D noise
void lfnoise3(in vec3 t, out float num)
{
    vec3 i = floor(t);
    t = fract(t);
    //t = ((6.*t-15.)*t+10.)*t*t*t;  // TODO: add this for slower perlin noise
    t = smoothstep(c.yyy, c.xxx, t); // TODO: add this for faster value noise
    vec2 v1, v2, v3, v4;
    rand3(i, v1.x);
    rand3(i+c.xyy, v1.y);
    rand3(i+c.yxy, v2.x);
    rand3(i+c.xxy, v2.y);
    rand3(i+c.yyx, v3.x);
    rand3(i+c.xyx, v3.y);
    rand3(i+c.yxx, v4.x);
    rand3(i+c.xxx, v4.y);
    v1 = c.zz+2.*mix(v1, v2, t.y);
    v3 = c.zz+2.*mix(v3, v4, t.y);
    v2.x = -1.+2.*mix(v1.x, v1.y, t.x);
    v2.y = -1.+2.*mix(v3.x, v3.y, t.x);
    num = mix(v2.x, v2.y, t.z);
}

// Make noise multi-frequency
void mfnoise3(in vec3 x, in float fmin, in float fmax, in float alpha, out float dst)
{
    dst = 0.;
    float a = 1., nf = 0.;
    for(float f = fmin; f<fmax; f = f*2.)
    {
        float buf;
        lfnoise3(f*x, buf);
        dst += a*buf;
        a *= alpha;
        nf += 1.;
    }
    dst *= (1.-alpha)/(1.-pow(alpha, nf));
}

// Scene
void scene(in vec3 x, out vec2 d)
{
    x.y += .1*iTime;
    float n;
    
    // Floor
    mfnoise(x.xy, 4.,4.e2,.35, n);
    d = vec2(x.z-.05*n,1.);
    
    // Trees
    float v, da;
    vec2 vi;
    dvoronoi(x.xy, v, vi);
    vec2 r;
    rand(vi, r.x);
    rand(vi+1301., r.y);
    vec2 y = x.xy-vi, 
        n2;
    lfnoise(3.*x.z*c.xx-r,n2.x);
    lfnoise(4.*x.z*c.xx+33.1*r, n2.y);
    da = length(y-.01*n2)-.07*mix(1.,.7,smoothstep(0., 1.,clamp(x.z*3.,0.,1.)));
    //zextrude(x.z,-da,10.,da);
    //add(d, vec2(da,2.), d);
    smoothmin(d.x,da,.2,d.x);
    d.y = mix(1.,2.,step(.1*n,x.z));
    
    // smaller branches
    float z = mod(x.z,.05)-.025, zi = (x.z-z)/.05;
    vec2 rp;//= vec2(0;
    rand(zi*c.xx+r,rp.x);
    rand(zi*c.xx+r+1332.,rp.y);
    rp *= vec2(1.,2.*pi);

    float nz;
    lfnoise(5.*length(y-.01*n2)*c.xx-33.*zi, nz);
    

    line(vec3(y-.01*n2, z+.01*nz), c.yyy, vec3(rp.x*vec2(cos(rp.y),sin(rp.y)),.05*rp.x), da);
    stroke(da, mix(1.,.1,smoothstep(0.,1.,clamp(length(vec3(y-.01*n2, z))/.7,0.,1.)))*.01*(.3+rp.x+n2.x), da);
    smoothmin(d.x,da,.05,d.x);
}

// Normal
const float dx = 5.e-4;
void normal(in vec3 x, out vec3 n);

// Texture
void colorize(in vec2 x, out vec3 col)
{
  x.y += .1*iTime;
  float n;
    mfnoise(x.xy, 4.e0,4.e2,.65, n);
  col = mix(vec3(0.55,0.69,0.37), vec3(0.00,0.02,0.04), .9+.1*n);
}

void mainImage( out vec4 fragColor, in vec2 fragCoord )
{
     // Set up coordinates
    a = iResolution.x/iResolution.y;
    vec2 uv = fragCoord/iResolution.yy-0.5*vec2(a, 1.0);
    vec3 col = c.yyy;
    
    // Camera setup
    float pp = .3*iTime;
    vec3 o = c.yzy+.2*c.yyx, 
        t = c.yyy+.3*c.yyx,
        dir = normalize(t-o),
        r = normalize(c.xyy),
        u = normalize(cross(r,dir)),
        n,
        x;
    t += uv.x*r + uv.y*u;
    dir = normalize(t-o);
    vec2 s;
    float d = 0.;//-(o.z-.05)/dir.z;
    int N = 350,
        i;
    
    // Raymarching
    for(i=0; i<N; ++i)
    {
        x = o + d * dir;
        scene(x,s);
        if(s.x < 1.e-4*max(d*d,1.)) break;
        if(d>10.)break;
        d += min(.01*max(d,1.),s.x);
    }
    
    // Illumination
    vec3 l = normalize(x+c.yxx);
    if(i<N)
    {
	    normal(x,n);
        //colorize(x.xy, col);
    }

    
    if(s.y == 2.)//Treess
    {
      
    col = .2*vec3(0.05,0.12,0.12)
        + .2*vec3(0.05,0.12,0.12)*abs(dot(l,n))
        + .6*vec3(0.04,0.13,0.12)*abs(pow(dot(reflect(-l,n),dir),3.));
    }
    if(s.y == 1.)
    {
        colorize(x.xy,col);
            .5*col
            + .2*col*abs(dot(l,n))
            +.6*col*abs(pow(dot(reflect(-l,n),dir),3.));
    }
    vec3 c1 =  mix(vec3(0.91,0.87,0.68),vec3(0.07,0.21,0.21),clamp(length(uv),0.,1.));
    float noiz;
    mfnoise3(x,1.,100.,.65,noiz);
    noiz = .5+.5*noiz;
    //noiz *= smoothstep(.3,.5,clamp(x.z,0.,1.));
    c1 = mix(c1, vec3(0.29,0.60,0.47), noiz);
    col = mix(col, c1, clamp(d/10.,0.,1.));
    
    fragColor = clamp(vec4(col,1.0),0.,1.);
}

void main()
{
    mainImage(gl_FragColor, gl_FragCoord.xy);
}
