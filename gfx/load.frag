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
    int n = int(floor(10.*clamp(iProgress,0.,.9999)));
    dnumber(x, n, size, d);
    float da;
    n = int(floor(100.*clamp(iProgress,0.,.9999))) - 10 * n;
    dnumber(x-2.1*size.x*c.xy, n, size, da);
    d = min(d,da);
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

// distance to gear
void dgear(in vec2 x, in vec2 r, in float n, out float d)
{
    float p = atan(x.y,x.x);
    p = mod(p, 2.*pi/n)*n/2./pi;
    d = mix(length(x)-r.x, length(x)-r.y, step(p,.5));
}

void mainImage( out vec4 fragColor, in vec2 fragCoord )
{
    a = iResolution.x/iResolution.y;
    ry = 1.5/iResolution.y;
    vec2 uv = fragCoord/iResolution.yy-0.5*vec2(a, 1.0);
    vec3 col = c.yyy;
    
    // Progress text
    float d, v;
    dprogress(uv+.51*c.xy, vec2(.03,.04), d);
    col = mix(col,vec3(0.75,0.20,0.26), smoothstep(ry, -ry, d));
    
    // Bar outline
    dbox2(uv, vec2(.4,.04), d);
    d = abs(d)-.002;
    col = mix(col, vec3(0.92,0.89,0.84), smoothstep(ry, -ry, d));
    
    // Bar content
    dbox2(uv+(.42-.42*clamp(iProgress,0.,.9999))*c.xy, vec2(clamp(iProgress,0.,.9999)*.42,.06), d);
    vec3 fc = vec3(0.76,0.20,0.25);
    col = mix(col, fc, smoothstep(ry, -ry, d+.03));
    
    // 210 Logo
    dteam210(uv-.13*c.yx+.025*c.xy-(-.42+.84*clamp(iProgress,0.,.9999))*c.xy, .05, d);
    col = mix(col, vec3(0.24,0.24,0.24), smoothstep(ry,-ry,d));
    d = abs(d-.01)-.002;
    col = mix(col, vec3(0.92,0.89,0.84), smoothstep(ry,-ry,d));
    
    // Red progress triangle
    dtriangle2(uv-.055*c.yx+.025*c.xy-(-.42+.84*clamp(iProgress,0.,.9999))*c.xy, .01, d);
    col = mix(col, vec3(0.76,0.20,0.25), smoothstep(ry,-ry, -d));
    
    // Upper phone task bar background
    dbox2(uv-.475*c.yx, vec2(a,.025), d);
    col = mix(col, vec3(0.24,0.24,0.24), smoothstep(ry,-ry, d));
    
    // Attention sign
    dtriangle2((uv-vec2(-.48*a,.47))*c.xz, .025, d);
    d = -d;
    dbox2(uv-vec2(-.48*a,.477), .5*vec2(.005,.017), v);
    d = max(d,-v);
    dbox2(uv-vec2(-.48*a,.462), .5*vec2(.005, .005), v);
    d = max(d,-v);
    col = mix(col, vec3(0.92,0.89,0.84), smoothstep(ry,-ry,d));
    
    // Battery Block
    dbox2((uv-vec2(.45*a,.475))*c.xz, vec2(.035, .018), d);
    dbox2((uv-vec2(.47*a,.475))*c.xz, vec2(.008, .01), v);
    d = min(d,v);
    col = mix(col, vec3(0.92,0.89,0.84), smoothstep(ry,-ry,d));
    dbox2((uv-vec2(.439*a,.475))*c.xz, .9*vec2(.015, .018), d);
    col = mix(col, vec3(0.76,0.20,0.25), smoothstep(ry,-ry,d));
    
    // Network information
    dbox2((uv-vec2(.39*a,.466))*c.xz, vec2(.0055, .009), d);
    dbox2((uv-vec2(.398*a,.469))*c.xz, vec2(.0055, .012), v);
	d = min(d,v);
    dbox2((uv-vec2(.406*a,.472))*c.xz, vec2(.0055, .015), v);
	d = min(d,v);
    dbox2((uv-vec2(.414*a,.475))*c.xz, vec2(.0055, .018), v);
	d = min(d,v);
    col = mix(col, vec3(0.92,0.89,0.84), smoothstep(ry,-ry,d));
    
    // Wifi
    d = length(uv-vec2(.36*a,.46))-.0025;
    d = min(d, abs(length(uv-vec2(.36*a,.46))-.01)-.003);
    d = min(d, abs(length(uv-vec2(.36*a,.46))-.02)-.003);
    d = min(d, abs(length(uv-vec2(.36*a,.46))-.03)-.003);
    mat2 m = mat2(cos(pi/4.),-sin(pi/4.), sin(pi/4.), cos(pi/4.));
    dbox2(m*(uv-vec2(.36*a,.5)), 2.*vec2(.015), v);
    d = max(d,v);
    col = mix(col, vec3(0.92,0.89,0.84), smoothstep(ry,-ry,d));
    
    // Playback bar
    dbox2(uv-.41*c.yx*c.xz, vec2(.47*a,.003), d);
    col = mix(col, c.xyy, smoothstep(ry,-ry, d));
    dbox2(uv-.41*c.yx*c.xz-.4*c.xy, vec2(.47*a-.4,.003), d);
    col = mix(col, vec3(0.92,0.89,0.84), smoothstep(ry,-ry, d));
    
    // Play symbol
    dtriangle2(((uv-vec2(-.44*a,-.455))).yx*c.xz, .025, d);
    col = mix(col, vec3(0.92,0.89,0.84), smoothstep(ry,-ry, -d));
    
    // Next symbol
    dtriangle2(((uv-vec2(-.4*a,-.455))).yx*c.xz, .025, d);
    dbox2(uv-vec2(-.385*a,-.455), vec2(.003, .024), v);
    d = min(-d,v);
    col = mix(col, vec3(0.92,0.89,0.84), smoothstep(ry,-ry, d));
    
    // Mute symbol
    dtriangle2(((uv-vec2(-.35*a,-.455))).yx, .025, d);
    dbox2(uv-vec2(-.36*a,-.455), vec2(.007, .007), v);
    d = min(-d,v);
    d = min(d, length(uv-vec2(-.345*a,-.455))-.011);
    dbox2(uv-vec2(-.344*a,-.455), vec2(.003, .024), v);
    d = max(d,-v);
    col = mix(col, vec3(0.92,0.89,0.84), smoothstep(ry,-ry, d));
    
    // Settings
    dgear(uv-vec2(.4*a,-.455), vec2(.02,.025), 8., d);
    d = max(d, -length(uv-vec2(.4*a,-.455))+.01);
    col = mix(col, vec3(0.92,0.89,0.84), smoothstep(ry,-ry, d));
    
    // Full Screen
    vec2 y = mod(uv, .03)-.015;
    dbox2(y, vec2(.01), d);
    d = abs(d)-.003;
    dbox2(uv-vec2(.44*a-.0025,-.45), vec2(.02), v);
	d = max(d,v);
    col = mix(col, vec3(0.92,0.89,0.84), smoothstep(ry,-ry, d));
    
    fragColor = vec4(clamp(col,0.0,1.0),1.0);
}

void main()
{
    mainImage(gl_FragColor, gl_FragCoord.xy);
}
