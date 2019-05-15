/* Permutation 042 Imitation
 * Copyright (C) 2019  Alexander Kraus <nr4@z10.info>
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

// Global constants
const float pi = acos(-1.);
const vec3 c = vec3(1.0, 0.0, -1.0);
float a = 1.0;

// Hash function
void rand(in vec2 x, out float num)
{
    x += 400.;
    num = fract(sin(dot(sign(x)*abs(x) ,vec2(12.9898,78.233)))*43758.5453);
}

void lfnoise(in vec2 t, out float n)
{
    vec2 i = floor(t);
    t = fract(t);
    t = smoothstep(c.yy, c.xx, t);
    vec2 v1, v2;
    rand(i, v1.x);
    rand(i+c.xy, v1.y);
    rand(i+c.yx, v2.x);
    rand(i+c.xx, v2.y);
    v1 = c.zz+2.*mix(v1, v2, t.y);
    n = mix(v1.x, v1.y, t.x);
}

void mfnoise(in vec2 x, in float d, in float b, in float e, out float n)
{
    n = 0.;
    float a = 1., nf = 0., buf;
    for(float f = d; f<b; f *= 2.)
    {
        lfnoise(f*x, buf);
        n += a*buf;
        a *= e;
        nf += 1.;
    }
    n *= (1.-e)/(1.-pow(e, nf));
}

void dvoronoi(in vec2 x, out float d, out vec2 z)
{
    vec2 y = floor(x);
       float ret = 1.;
    vec2 pf=c.yy, p;
    float df=10.;
    
    for(int i=-1; i<=1; i+=1)
        for(int j=-1; j<=1; j+=1)
        {
            p = y + vec2(float(i), float(j));
            float pa;
            rand(p, pa);
            p += pa;
            
            d = length(x-p);
            
            if(d < df)
            {
                df = d;
                pf = p;
            }
        }
    for(int i=-1; i<=1; i+=1)
        for(int j=-1; j<=1; j+=1)
        {
            p = y + vec2(float(i), float(j));
            float pa;
            rand(p, pa);
            p += pa;
            
            vec2 o = p - pf;
            d = length(.5*o-dot(x-pf, o)/dot(o,o)*o);
            ret = min(ret, d);
        }
    
    d = ret;
    z = pf;
}

// iq's smooth minimum
float smoothmin( float a, float b, float k )
{
    float h = max( k-abs(a-b), 0.0 )/k;
    return min( a, b ) - h*h*h*k*(1.0/6.0);
}

void add(in vec2 sda, in vec2 sdb, out vec2 sdf)
{
  sdf = mix(sda, sdb, step(sdb.x, sda.x));
}

// Extrusion
void zextrude(in float z, in float d2d, in float h, out float d)
{
    vec2 w = vec2(-d2d, abs(z)-0.5*h);
    d = length(max(w,0.0));
}

// 2D box
void dbox(in vec2 p, in vec2 b, out float dst)
{
  	vec2 d = abs(p) - b;
  	dst = length(max(d,0.0)) + min(max(d.x,d.y),0.0); 
}

// Distance to line segment
void line(in vec3 x, in vec3 p1, in vec3 p2, out float dst)
{
    vec3 d = p2-p1;
    dst = length(x-mix(p1, p2, clamp(dot(x-p1, d)/dot(d,d),0.,1.)));
}

void stroke(in float d, in float s, out float dst)
{
  dst = abs(d)-s;
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
  d.x = smoothmin(d.x,da,.2);
  d.y = mix(1.,2.,step(.1*n,x.z));
  
  // smaller branches
  float z = mod(x.z,.05)-.025, zi = (x.z-z)/.05;
  vec2 rp;//= vec2(0;
  rand(zi*c.xx+r,rp.x);
  rand(zi*c.xx+r+1332.,rp.y);
  rp *= vec2(1.,2.*pi);

  float nz;
  lfnoise(5.*length(y-.01*n2)*c.xx, nz);
  

  line(vec3(y-.01*n2, z+.01*nz), c.yyy, vec3(rp.x*vec2(cos(rp.y),sin(rp.y)),.0*rp.x), da);
  stroke(da, mix(1.,.1,smoothstep(0.,1.,clamp(length(vec3(y-.01*n2, z))/.7,0.,1.)))*.01*(.3+rp.x+n2.x), da);
  d.x = smoothmin(d.x,da,.05);
}

// Normal
const float dx = 5.e-4;
void normal(in vec3 x, out vec3 n)
{
    vec2 s, na;
    scene(x,s);
    scene(x+dx*c.xyy, na);
    n.x = na.x;
    scene(x+dx*c.yxy, na);
    n.y = na.x;
    scene(x+dx*c.yyx, na);
    n.z = na.x;
    n = normalize(n-s.x);
}

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
           +.6*vec3(0.04,0.13,0.12)*abs(pow(dot(reflect(-l,n),dir),3.));
    }
    if(s.y == 1.)
    {
      colorize(x.xy,col);
      .5*col
            + .2*col*abs(dot(l,n))
           +.6*col*abs(pow(dot(reflect(-l,n),dir),3.));
    }
    
    col = mix(col, mix(vec3(0.91,0.87,0.68),vec3(0.07,0.21,0.21),clamp(length(uv),0.,1.)), clamp(d/10.,0.,1.));
    
    fragColor = clamp(vec4(col,1.0),0.,1.);
}

