/* Tunguska by Team210 - 64k Intro at Solskogen 2k19
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

#version 130

uniform float iTime, iProgress;
uniform vec2 iResolution;
 
// Global constants
const float pi = acos(-1.);
const vec3 c = vec3(1.0, 0.0, -1.0);
float a = 1.0, ry = 1.0;

// Hash function
void rand(in vec2 x, out float num)
{
    x += 400.;
    num = fract(sin(dot(sign(x)*abs(x) ,vec2(12.9898,78.233)))*43758.5453);
}

// 2D box
void dbox2(in vec2 x, in vec2 b, out float d)
{
	vec2 da = abs(x)-b;
	d = length(max(da,c.yy)) + min(max(da.x,da.y),0.0);
}

// Simple numbers
void dnumber(in vec2 x, in int number, in vec2 size, out float d)
{
    if(number == 0)
    {
        dbox2(x, size, d);
        float da;
        dbox2(x, vec2(.4,.6)*size, da);
        d = max(d, -da);
    }
    else if(number == 1)
    {
        dbox2(x,vec2(.4,1.)*size, d);
    }
    else if(number == 2)
    {
        dbox2(x, size, d);
        float da;
        dbox2(x+vec2(.3,-.4)*size, vec2(.8,.2)*size, da);
        d = max(d, -da);
    	dbox2(x-vec2(.3,-.4)*size, vec2(.8,.2)*size, da);
        d = max(d, -da);
    }
    else if(number == 3)
    {
        dbox2(x, size, d);
        float da;
        dbox2(x+vec2(.3,-.4)*size, vec2(.8,.2)*size, da);
        d = max(d, -da);
    	dbox2(x+vec2(.3,.4)*size, vec2(.8,.2)*size, da);
        d = max(d, -da);
    }
    else if(number == 4)
    {
        dbox2(x, size, d);
        float da;
        dbox2(x-vec2(0.,.6)*size, vec2(.5,.5)*size, da);
        d = max(d, -da);
    	dbox2(x-vec2(-.3,-.7)*size, vec2(.8,.4)*size, da);
        d = max(d, -da);
    }
    else if(number == 5)
    {
        dbox2(x, size, d);
        float da;
        dbox2(x+vec2(.3,.4)*size, vec2(.8,.2)*size, da);
        d = max(d, -da);
    	dbox2(x-vec2(.3,.4)*size, vec2(.8,.2)*size, da);
        d = max(d, -da);
    }
    else if(number == 6)
    {
        dbox2(x, size, d);
        float da;
        dbox2(x+vec2(0.,.4)*size, vec2(.5,.2)*size, da);
        d = max(d, -da);
    	dbox2(x-vec2(.3,.4)*size, vec2(.8,.2)*size, da);
        d = max(d, -da);
    }
    else if(number == 7)
    {
        dbox2(x, size, d);
        float da;
    	dbox2(x-vec2(-.3,-.3)*size, vec2(.8,.9)*size, da);
        d = max(d, -da);
    }
    else if(number == 8)
    {
        dbox2(x, size, d);
        float da;
        dbox2(x+vec2(0.,.4)*size, vec2(.5,.2)*size, da);
        d = max(d, -da);
    	dbox2(x-vec2(0.,.4)*size, vec2(.5,.2)*size, da);
        d = max(d, -da);
    }
    else if(number == 9)
    {
        dbox2(-x, size, d);
        float da;
        dbox2(-x+vec2(0.,.4)*size, vec2(.5,.2)*size, da);
        d = max(d, -da);
    	dbox2(-x-vec2(.3,.4)*size, vec2(.8,.2)*size, da);
        d = max(d, -da);
    }
}

// Convert progress to %.2d
void dprogress(in vec2 x, in vec2 size, out float d)
{
    int n = int(floor(10.*iProgress));
    dnumber(x, n, size, d);
    float da;
    n = int(floor(100.*iProgress)) - 10 * n;
    dnumber(x-2.1*size.x*c.xy, n, size, da);
    d = min(d,da);
}

// Distance to regular voronoi
void dvoronoi(in vec2 x, out float d, out vec2 ind)
{
    vec2 y = floor(x);
   	float ret = 1.;
    
    //find closest control point. ("In which cell am I?")
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
    
    //compute voronoi distance: minimum distance to any edge
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
    ind = pf;
}

void dteam210(in vec2 x, in float size, out float d)
{
    dbox2(x, vec2(.2,1.)*size, d);
    d = min(d, abs(length(x-vec2(.8,0.)*size)-.8*size)-.2*size);
    d = min(d, abs(length(x+vec2(.8,0.)*size)-.8*size)-.2*size);
    float da;
    dbox2(x+2.85*size*c.xy,2.*vec2(size), da);
    d = max(d,-da);
}

// compute distance to regular triangle
void dtriangle2(in vec2 uv, in float r, out float d)
{
    float dp = 2.*pi/3.;
    vec2 p0 = r*vec2(cos(pi/2.), -sin(pi/2.)),
        p1 = r*vec2(cos(pi/2.+dp), -sin(pi/2.+dp)),
        p2 = r*vec2(cos(pi/2.+2.*dp), -sin(pi/2.+2.*dp)), 
        pd = p2-p1;
    
    d = min(dot(uv-p0,c.xz*(p1-p0).yx),dot(uv-p1, pd.yx*c.xz));
	d = min(d, dot(uv-p2, (p0-p2).yx*c.xz))/length(pd);
}

void mainImage( out vec4 fragColor, in vec2 fragCoord )
{
    a = iResolution.x/iResolution.y;
    ry = 1.5/iResolution.y;
    vec2 uv = fragCoord/iResolution.yy-0.5*vec2(a, 1.0);
    vec3 col = c.yyy;
    
    // Progress text
    float d;
    dprogress(uv+.51*c.xy, vec2(.03,.04), d);
    col = mix(col,vec3(0.75,0.20,0.26), smoothstep(ry, -ry, d));
    
    // Bar outline
    dbox2(uv, vec2(.4,.04), d);
    d = abs(d)-.002;
    col = mix(col, vec3(0.92,0.89,0.84), smoothstep(ry, -ry, d));
    
    // Bar content
    dbox2(uv+(.42-.42*iProgress)*c.xy, vec2(iProgress*.42,.06), d);
    vec3 fc = mix(vec3(0.24,0.24,0.24), vec3(0.18,0.18,0.18), clamp((uv.x+.42)/.84,0.,1.));
    vec2 ind;
    float v, v0;
    dvoronoi(30.*uv, v, ind);
    v = abs(v)-.1;
    fc = mix(fc, vec3(0.75,0.20,0.26), smoothstep(ry,-ry,v));
    v0 = v;
    v = abs(v)-.05;
    float va;
    dvoronoi(90.*uv, va, ind);
    v *= va/30.;
    float r;
    rand(ind, r);
    fc = mix(fc, vec3(0.75,0.19,0.24), smoothstep(-ry,ry,v0*v)*r);
    fc = mix(fc, c.yyy, smoothstep(ry,-ry,v));
    col = mix(col, fc, smoothstep(ry, -ry, d+.03));
    
    // 210 Logo
    dteam210(uv-.13*c.yx+.025*c.xy-(-.42+.84*iProgress)*c.xy, .05, d);
    col = mix(col, vec3(0.24,0.24,0.24), smoothstep(ry,-ry,d));
    d = abs(d-.01)-.002;
    col = mix(col, vec3(0.92,0.89,0.84), smoothstep(ry,-ry,d));
    
    // Red progress triangle
    dtriangle2(uv-.055*c.yx+.025*c.xy-(-.42+.84*iProgress)*c.xy, .01, d);
    col = mix(col, vec3(0.76,0.20,0.25), smoothstep(ry,-ry, -d));
    
    fragColor = vec4(clamp(col,0.0,1.0),1.0);
}

void main()
{
    mainImage(gl_FragColor, gl_FragCoord.xy);
}
