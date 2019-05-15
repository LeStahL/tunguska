//Generated with Symbolize (c) 2019 Alexander Kraus <nr4@z10.info>.
#ifndef SYMBOLIZE_H
#define SYMBOLIZE_H

int rand_handle, lfnoise_handle, mfnoise_handle, dbox_handle, dbox3_handle, dvoronoi_handle;
const int nsymbols = 6;
const char *rand_source = "#version 130\n\n"
"void rand(in vec2 x, out float n)\n"
"{\n"
"    x += 400.;\n"
"    n = fract(sin(dot(sign(x)*abs(x) ,vec2(12.9898,78.233)))*43758.5453);\n"
"}\n"
"\0";
const char *lfnoise_source = "#version 130\n\n"
"const vec3 c = vec3(1.,0.,-1.);\n"
"void rand(in vec2 x, out float d);\n"
"void lfnoise(in vec2 t, out float n)\n"
"{\n"
"    vec2 i = floor(t);\n"
"    t = fract(t);\n"
"    t = smoothstep(c.yy, c.xx, t);\n"
"    vec2 v1, v2;\n"
"    rand(i, v1.x);\n"
"    rand(i+c.xy, v1.y);\n"
"    rand(i+c.yx, v2.x);\n"
"    rand(i+c.xx, v2.y);\n"
"    v1 = c.zz+2.*mix(v1, v2, t.y);\n"
"    n = mix(v1.x, v1.y, t.x);\n"
"}\n"
"\0";
const char *mfnoise_source = "#version 130\n\n"
"// const vec3 c = vec3(1.,0.,-1.);\n"
"void lfnoise(in vec2 x, out float d);\n"
"void mfnoise(in vec2 x, in float d, in float b, in float e, out float n)\n"
"{\n"
"    n = 0.;\n"
"    float a = 1., nf = 0., buf;\n"
"    for(float f = d; f<b; f *= 2.)\n"
"    {\n"
"        lfnoise(f*x, buf);\n"
"        n += a*buf;\n"
"        a *= e;\n"
"        nf += 1.;\n"
"    }\n"
"    n *= (1.-e)/(1.-pow(e, nf));\n"
"}\n"
"\0";
const char *dbox_source = "#version 130\n\n"
"const vec3 c = vec3(1.,0.,-1.);\n"
"void dbox(in vec2 x, in vec2 b, out float d)\n"
"{\n"
"    vec2 da = abs(x)-b;\n"
"    d = length(max(da,c.yy)) + min(max(da.x,da.y),0.0);\n"
"}\n"
"\0";
const char *dbox3_source = "#version 130\n\n"
"void dbox3(in vec3 x, in vec3 b, out float d)\n"
"{\n"
"  vec3 da = abs(x) - b;\n"
"  d = length(max(da,0.0))\n"
"         + min(max(da.x,max(da.y,da.z)),0.0);\n"
"}\n"
"\0";
const char *dvoronoi_source = "#version 130\n\n"
"const vec3 c = vec3(1.,0.,-1.);\n"
"void rand(in vec2 x, out float d);\n"
"void dvoronoi(in vec2 x, out float d, out vec2 z)\n"
"{\n"
"    vec2 y = floor(x);\n"
"       float ret = 1.;\n"
"    vec2 pf=c.yy, p;\n"
"    float df=10.;\n"
"    \n"
"    for(int i=-1; i<=1; i+=1)\n"
"        for(int j=-1; j<=1; j+=1)\n"
"        {\n"
"            p = y + vec2(float(i), float(j));\n"
"            float pa;\n"
"            rand(p, pa);\n"
"            p += pa;\n"
"            \n"
"            d = length(x-p);\n"
"            \n"
"            if(d < df)\n"
"            {\n"
"                df = d;\n"
"                pf = p;\n"
"            }\n"
"        }\n"
"    for(int i=-1; i<=1; i+=1)\n"
"        for(int j=-1; j<=1; j+=1)\n"
"        {\n"
"            p = y + vec2(float(i), float(j));\n"
"            float pa;\n"
"            rand(p, pa);\n"
"            p += pa;\n"
"            \n"
"            vec2 o = p - pf;\n"
"            d = length(.5*o-dot(x-pf, o)/dot(o,o)*o);\n"
"            ret = min(ret, d);\n"
"        }\n"
"    \n"
"    d = ret;\n"
"    z = pf;\n"
"}\n"
"\0";
const char *decayingfactory_source = "/* Endeavor by Team210 - 64k intro by Team210 at Revision 2k19\n"
"* Copyright (C) 2018  Alexander Kraus <nr4@z10.info>\n"
"*\n"
"* This program is free software: you can redistribute it and/or modify\n"
"* it under the terms of the GNU General Public License as published by\n"
"* the Free Software Foundation, either version 3 of the License, or\n"
"* (at your option) any later version.\n"
"*\n"
"* This program is distributed in the hope that it will be useful,\n"
"* but WITHOUT ANY WARRANTY; without even the implied warranty of\n"
"* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the\n"
"* GNU General Public License for more details.\n"
"*\n"
"* You should have received a copy of the GNU General Public License\n"
"* along with this program.  If not, see <https://www.gnu.org/licenses/>.\n"
"*/\n"
"\n"
"#version 130\n\n"
"\n"
"uniform float iTime;\n"
"uniform vec2 iResolution;\n"
"\n"
"// Global constants\n"
"const float pi = acos(-1.);\n"
"const vec3 c = vec3(1.0, 0.0, -1.0);\n"
"float a = 1.0;\n"
"\n"
"// Hash function\n"
"void rand(in vec2 x, out float num);\n"
"void lfnoise(in vec2 t, out float num);\n"
"void mfnoise(in vec2 x, in float fmin, in float fmax, in float alpha, out float num);\n"
"void dbox(in vec2 x, in vec2 b, out float d);\n"
"void dbox3(in vec3 x, in vec3 b, out float d);\n"
"void dvoronoi(in vec2 x, out float d, out vec2 ind);\n"
"\n"
"// Stroke\n"
"void stroke(in float d0, in float s, out float d)\n"
"{\n"
"    d = abs(d0)-s;\n"
"}\n"
"\n"
"void colorize_wall_concrete(in vec2 x, out vec3 col)\n"
"{\n"
"    float w = .4,\n"
"        d;\n"
"    float n, n2, n3, n4;\n"
"    mfnoise(x, 8.,8.e2, .68, n);\n"
"    lfnoise(2.*x, n2);\n"
"    col = mix(vec3(0.85,0.84,0.82), vec3(0.07,0.05,0.04), .5+.5*n);\n"
"    \n"
"     // white\n"
"    col = mix(col, mix(vec3(0.85,0.84,0.82), vec3(0.86,0.83,0.79), .8+.2*n), smoothstep(.0,.3,n*n2));\n"
"    \n"
"    // Red paint\n"
"    mfnoise(x, .4,2.e2, .48, n3);\n"
"    \n"
"    //gray\n"
"    col = mix(col, c.yyy, smoothstep(-.2,.4, .3+n3-n));\n"
"    float na;\n"
"    lfnoise(iTime*c.xx, na);\n"
"    col = mix(col, c.yyy, (.5+.5*na)*smoothstep(.1,-.1,abs(.3+n3-n)-.1));\n"
"    col = mix(col, mix(vec3(0.45,0.06,0.01), vec3(0.69,0.14,0.04), (.1+.9*n)*smoothstep(-.1,.4,.3+1.4*n3+.7*n)), smoothstep(.3,.7,.3+1.4*n3-.5*n));\n"
"}\n"
"\n"
"void colorize_tiles(in vec2 x, out vec3 col)\n"
"{\n"
"    //x.x += .2*iTime;\n"
"    \n"
"    float w = .4,\n"
"        d;\n"
"    vec2 y = mod(x, w)-.5*w;\n"
"    float n;\n"
"    mfnoise(x, 4.,4.e2, .48, n);\n"
"    \n"
"    dbox(y, .47*w*c.xx, d);\n"
"    d += .005*n;\n"
"    col = mix(vec3(0.90,0.84,0.80),1.2*vec3(1.00,0.95,0.86), .5+.5*n);\n"
"    \n"
"    // Dirt/rust\n"
"    float nr;\n"
"    mfnoise(x*vec2(12.,1.), 1.,1.e1, .85, nr);\n"
"    col = mix(col, 1.4*vec3(0.80,0.58,0.22), clamp(.1+.9*nr,0.,1.));\n"
"    \n"
"    // cracks\n"
"    float v;\n"
"    vec2 vi;\n"
"    dvoronoi((x.xy-.1*n)/w,v, vi); \n"
"    col = mix(col, vec3(0.54,0.48,0.45), smoothstep(1.5/iResolution.y, -1.5/iResolution.y, abs(v)-.01));\n"
"   \n"
"    // joint\n"
"    col = mix(vec3(0.22,0.22,0.22), col, smoothstep(1.5/iResolution.y, -1.5/iResolution.y, d));\n"
"    \n"
"    // holes\n"
"    float na;\n"
"    mfnoise(x, 4.,4.e2, .98, na);\n"
"    col = mix(col, mix(vec3(0.54,0.48,0.45), .0*c.xxx, .5+.5*na), smoothstep(1.5/iResolution.y, -1.5/iResolution.y, .3+n));\n"
"}\n"
"\n"
"void colorize_wall(in vec2 x, out vec3 col)\n"
"{\n"
"    if(x.y > 0.) colorize_wall_concrete(x, col);\n"
"    else colorize_tiles(x, col);\n"
"}\n"
"\n"
"void colorize_pipes(in vec2 x, out vec3 col)\n"
"{\n"
"    float n, nlo;\n"
"    \n"
"    mfnoise(x,4.8e1, 4.8e3, .65, n);\n"
"    lfnoise(6.*x, nlo);\n"
"    col = mix(vec3(0.37,0.07,0.00), vec3(0.62,0.63,0.66), .5+.5*n);\n"
"    \n"
"    vec3 c1 = mix(vec3(1.00,0.84,0.70), vec3(0.37,0.07,0.00), .5+.5*n);\n"
"    col = mix(col, c1, smoothstep(n-.1,n+.1,nlo));\n"
"    col = mix(col, vec3(0.64,0.39,0.32), (.5+.5*n)*smoothstep(n-.1,n+.1,nlo));\n"
"    \n"
"    float nb;\n"
"    stroke(n, .1, nb);\n"
"    col = mix(col, vec3(0.58,0.33,0.26), smoothstep(1.5/iResolution.y, -1.5/iResolution.y, nb));\n"
"}\n"
"\n"
"void wall_tiles(in vec3 x, out float d)\n"
"{\n"
"    //x.x += .2*iTime;\n"
"    \n"
"    float n;\n"
"    mfnoise(x.xy, 4.,4.e2, .45, n);\n"
"\n"
"    float w = .4;\n"
"    vec2 y = mod(x.xy, w)-.5*w;\n"
"    dbox(y, .47*w*c.xx, d);\n"
"    \n"
"    // cracks\n"
"    float v;\n"
"    vec2 vi;\n"
"    dvoronoi((x.xy-.1*n)/w,v, vi); \n"
"    \n"
"    v = mix(v, 1., smoothstep(1.5/iResolution.y, -1.5/iResolution.y, .3+n));\n"
"    \n"
"    d = x.z\n"
"        + .008*n\n"
"        + .2*smoothstep(1.5/iResolution.y, -1.5/iResolution.y, abs(v)-.01)\n"
"        +.3*smoothstep(2.5/iResolution.y, -2.5/iResolution.y, d+.005*n)*smoothstep(-1.5/iResolution.y,1.5/iResolution.y,.3+n)\n"
"        -.2*(.5*n+.5)*smoothstep(1.5/iResolution.y,-1.5/iResolution.y,.3+n);\n"
"}\n"
"\n"
"void wall_concrete(in vec3 x, out float d)\n"
"{\n"
"    float n;\n"
"    mfnoise(x.xy, 8.,8.e2, .68, n);\n"
"    \n"
"    d = x.z\n"
"        +.015*(.5+.5*n);\n"
"}\n"
"\n"
"void pipes(in vec3 x, out float d)\n"
"{\n"
"    float n, nlo, na;\n"
"    mfnoise(x.xy,4.8e1, 4.8e3, .65, na);\n"
"    mfnoise(x.xy,1.8e1, 4.8e3, .55, n);\n"
"    lfnoise(6.*x.xy, nlo);\n"
"    \n"
"    d = x.z+.015*n-.015*smoothstep(na-.1,na+.1,nlo);\n"
"}\n"
"\n"
"void pipes_normal(in vec3 x, out vec3 n)\n"
"{\n"
"    const float dx = 5.e-4;\n"
"    float s;\n"
"    \n"
"    pipes(x,s);\n"
"    pipes(x+dx*c.xyy, n.x);\n"
"    pipes(x+dx*c.yxy, n.y);\n"
"    pipes(x+dx*c.yyx, n.z);\n"
"    n = normalize(n-s);\n"
"}\n"
"\n"
"void wall_tiles_normal(in vec3 x, out vec3 n)\n"
"{\n"
"    const float dx = 5.e-4;\n"
"    float s;\n"
"    \n"
"    wall_tiles(x,s);\n"
"    wall_tiles(x+dx*c.xyy, n.x);\n"
"    wall_tiles(x+dx*c.yxy, n.y);\n"
"    wall_tiles(x+dx*c.yyx, n.z);\n"
"    n = normalize(n-s);\n"
"}\n"
"\n"
"void wall_concrete_normal(in vec3 x, out vec3 n)\n"
"{\n"
"    const float dx = 5.e-4;\n"
"    float s;\n"
"    \n"
"    wall_concrete(x,s);\n"
"    wall_concrete(x+dx*c.xyy, n.x);\n"
"    wall_concrete(x+dx*c.yxy, n.y);\n"
"    wall_concrete(x+dx*c.yyx, n.z);\n"
"    n = normalize(n-s);\n"
"}\n"
"\n"
"void add(in vec2 sda, in vec2 sdb, out vec2 sdf)\n"
"{\n"
"    sdf = mix(sda, sdb, step(sdb.x, sda.x));\n"
"}\n"
"\n"
"void scene(in vec3 x, out vec2 sdf)\n"
"{\n"
"    float d;\n"
"    \n"
"    dbox3(x, 112.*c.xxx, sdf.x);\n"
"    \n"
"    // Remove corridor\n"
"    float w = mix(.4,.38, smoothstep(1.5/iResolution.y,-1.5/iResolution.y,x.y));\n"
"    dbox3(x, vec3(w,1.2*w,56.), d);\n"
"    sdf.x = max(sdf.x, -d);\n"
"    dbox3(vec3(x.xy, abs(mod(x.z,2.)))-.8*c.yyx, vec3(2.,1.2*w,w), d);\n"
"    sdf.x = max(sdf.x, -d);\n"
"    sdf.y = 1.;\n"
"    \n"
"    // Add pipes\n"
"    if(abs(x.x) < .4)\n"
"    {\n"
"        float wa = .1;\n"
"        vec2 sda = vec2(length(vec2(mod(x.x,wa)-.5*wa, x.y-1.2*w))-.4*wa, 2.);\n"
"        add(sdf, sda, sdf);\n"
"    }\n"
"    \n"
"    // Add Water\n"
"    float n, n2;\n"
"    lfnoise(12.*x.xz-iTime*c.yx+.1*iTime*c.xy, n);\n"
"    lfnoise(34.5*x.xz-3.141*iTime*c.yx-.1*iTime*c.xy, n2);\n"
"    vec2 sdb = vec2(x.y+.38-.002*(.7*n+.3*n2),3.);\n"
"    add(sdf, sdb, sdf);\n"
"}\n"
"\n"
"void normal(in vec3 x, out vec3 n)\n"
"{\n"
"    const float dx = 5.e-4;\n"
"    vec2 s, na;\n"
"    \n"
"    scene(x,s);\n"
"    scene(x+dx*c.xyy, na);\n"
"    n.x = na.x;\n"
"    scene(x+dx*c.yxy, na);\n"
"    n.y = na.x;\n"
"    scene(x+dx*c.yyx, na);\n"
"    n.z = na.x;\n"
"    n = normalize(n-s.x);\n"
"}\n"
"\n"
"void colorize(in vec3 x, in vec2 s, inout vec3 n, out vec3 col)\n"
"{\n"
"    if(s.y == 1.)\n"
"        {\n"
"            // March volumetric texture\n"
"            if(abs(dot(n,c.xyy))+abs(dot(n,c.yxy)) < 1.e-4)\n"
"            {\n"
"                //Front wall\n"
"                colorize_wall(6.*x.xy, col);\n"
"                if(x.y < 0.) wall_tiles_normal(6.*x*c.xxy, n);\n"
"                else wall_concrete_normal(6.*x*c.xxy, n);\n"
"            }\n"
"            else if(abs(dot(n,c.yxy))+abs(dot(n,c.yyx)) < 1.e-4)\n"
"            {\n"
"                //Side Wall\n"
"                colorize_wall(6.*x.zy, col);\n"
"                if(x.y < 0.) wall_tiles_normal(6.*x.zyx, n);\n"
"                else wall_concrete_normal(6.*x.zyx, n);\n"
"            }\n"
"            else \n"
"            {\n"
"                // Floor\n"
"                if(x.y < 0.)\n"
"                {\n"
"                    colorize_wall(6.*x.zx-23., col);\n"
"                    wall_tiles_normal(6.*x.zxy-23.*c.xxy, n);\n"
"                }\n"
"                else\n"
"                {\n"
"                    colorize_wall(6.*x.zx+23., col);\n"
"                    wall_concrete_normal(6.*x.zxy+23.*c.xxy, n);\n"
"                }\n"
"                    //col = .2*c.xxx;\n"
"            }\n"
"        }\n"
"        else if(s.y == 2.)\n"
"        {\n"
"            colorize_pipes(6.*x.zx-23., col);\n"
"            pipes_normal(6.*x.zxy-23.*c.xxy, n);\n"
"        }\n"
"}\n"
"\n"
"void mainImage( out vec4 fragColor, in vec2 fragCoord )\n"
"{\n"
"    a = iResolution.x/iResolution.y;\n"
"    vec2 uv = fragCoord/iResolution.yy-0.5*vec2(a, 1.0);\n"
"    vec3 col = c.yyy;\n"
"    \n"
"    vec3 t = vec3(uv, 0.)\n"
"            -mix(.3,.7, 0.*smoothstep(0.,1.,sin(pi*iTime)))*iTime*c.yyx // forward\n"
"            +.01*abs(cos(2.*pi*iTime))*c.yxy // up/down\n"
"            + .005*sin(2.*pi*iTime)*c.xyy, // right/left\n"
"        o = c.yyx\n"
"            -.3*iTime*c.yyx\n"
"            +.01*abs(sin(2.*pi*iTime))*c.yxy\n"
"            + .005*sin(2.*pi*iTime)*c.xyy,\n"
"        dir = normalize(t-o),\n"
"        x, n;\n"
"    float d = 0.;\n"
"    vec2 s;\n"
"    int N = 300, i;\n"
"    \n"
"    for(i=0; i<N; ++i)\n"
"    {\n"
"        x = o + d * dir;\n"
"        scene(x, s);\n"
"        if(s.x < 1.e-4) break;\n"
"        d += s.x;\n"
"    }\n"
"      \n"
"    //if(i<N)\n"
"    {\n"
"        normal(x, n);\n"
"        float d0 = d;\n"
"        if(s.y == 3.)\n"
"        {\n"
"            o = x;\n"
"            d = .002;\n"
"            dir = reflect(dir, n);\n"
"            \n"
"            for(i=0; i<N; ++i)\n"
"            {\n"
"                x = o + d * dir;\n"
"                scene(x, s);\n"
"                if(s.x < 1.e-4) break;\n"
"                d += s.x;\n"
"            }\n"
"            \n"
"            //if(i<N)\n"
"                normal(x, n);\n"
"            d = abs(d)+ abs(d0);\n"
"        }\n"
"        colorize(x, s, n, col);\n"
"        \n"
"        vec3 l = -abs(normalize(o)/max(d*d,1.));\n"
"        col = .1*col/min(d,4.)\n"
"            + .6*col*abs(dot(l,n))\n"
"            + .3*col*abs(pow(dot(reflect(-l,n),dir),4.));\n"
"    }\n"
"    \n"
"    col = clamp(col, 0., 1.);\n"
"    \n"
"    float na;\n"
"    lfnoise(iTime*c.xx, na);\n"
"    vec3 gs = length(col)*c.xxx;\n"
"    col = mix(col, gs, .5+.5*na);\n"
"    \n"
"    fragColor = vec4(col,1.0);\n"
"}\n"
"\n"
"void main()\n"
"{\n"
"    mainImage(gl_FragColor, gl_FragCoord.xy);\n"
"}\n"
"\0";
void Loadrand()
{
    int rand_size = strlen(rand_source);
    rand_handle = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(rand_handle, 1, (GLchar **)&rand_source, &rand_size);
    glCompileShader(rand_handle);
#ifdef DEBUG
    printf("---> rand Shader:\n");
    debug(rand_handle);
    printf(">>>>\n");
#endif
    progress += .2/(float)nsymbols;
}
void Loadlfnoise()
{
    int lfnoise_size = strlen(lfnoise_source);
    lfnoise_handle = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(lfnoise_handle, 1, (GLchar **)&lfnoise_source, &lfnoise_size);
    glCompileShader(lfnoise_handle);
#ifdef DEBUG
    printf("---> lfnoise Shader:\n");
    debug(lfnoise_handle);
    printf(">>>>\n");
#endif
    progress += .2/(float)nsymbols;
}
void Loadmfnoise()
{
    int mfnoise_size = strlen(mfnoise_source);
    mfnoise_handle = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(mfnoise_handle, 1, (GLchar **)&mfnoise_source, &mfnoise_size);
    glCompileShader(mfnoise_handle);
#ifdef DEBUG
    printf("---> mfnoise Shader:\n");
    debug(mfnoise_handle);
    printf(">>>>\n");
#endif
    progress += .2/(float)nsymbols;
}
void Loaddbox()
{
    int dbox_size = strlen(dbox_source);
    dbox_handle = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(dbox_handle, 1, (GLchar **)&dbox_source, &dbox_size);
    glCompileShader(dbox_handle);
#ifdef DEBUG
    printf("---> dbox Shader:\n");
    debug(dbox_handle);
    printf(">>>>\n");
#endif
    progress += .2/(float)nsymbols;
}
void Loaddbox3()
{
    int dbox3_size = strlen(dbox3_source);
    dbox3_handle = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(dbox3_handle, 1, (GLchar **)&dbox3_source, &dbox3_size);
    glCompileShader(dbox3_handle);
#ifdef DEBUG
    printf("---> dbox3 Shader:\n");
    debug(dbox3_handle);
    printf(">>>>\n");
#endif
    progress += .2/(float)nsymbols;
}
void Loaddvoronoi()
{
    int dvoronoi_size = strlen(dvoronoi_source);
    dvoronoi_handle = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(dvoronoi_handle, 1, (GLchar **)&dvoronoi_source, &dvoronoi_size);
    glCompileShader(dvoronoi_handle);
#ifdef DEBUG
    printf("---> dvoronoi Shader:\n");
    debug(dvoronoi_handle);
    printf(">>>>\n");
#endif
    progress += .2/(float)nsymbols;
}

void LoadSymbols()
{
    Loadrand();
    updateBar();
    Loadlfnoise();
    updateBar();
    Loadmfnoise();
    updateBar();
    Loaddbox();
    updateBar();
    Loaddbox3();
    updateBar();
    Loaddvoronoi();
    updateBar();
}
int decayingfactory_program, decayingfactory_handle;
int decayingfactory_iTime_location,decayingfactory_iResolution_location;
const int nprograms = 1;

void Loaddecayingfactory()
{
    int decayingfactory_size = strlen(decayingfactory_source);
    decayingfactory_handle = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(decayingfactory_handle, 1, (GLchar **)&decayingfactory_source, &decayingfactory_size);
    glCompileShader(decayingfactory_handle);
#ifdef DEBUG
    printf("---> decayingfactory Shader:\n");
    debug(decayingfactory_handle);
    printf(">>>>\n");
#endif
    decayingfactory_program = glCreateProgram();
    glAttachShader(decayingfactory_program,decayingfactory_handle);
    glAttachShader(decayingfactory_program,rand_handle);
    glAttachShader(decayingfactory_program,lfnoise_handle);
    glAttachShader(decayingfactory_program,mfnoise_handle);
    glAttachShader(decayingfactory_program,dbox_handle);
    glAttachShader(decayingfactory_program,dbox3_handle);
    glAttachShader(decayingfactory_program,dvoronoi_handle);
    glLinkProgram(decayingfactory_program);
#ifdef DEBUG
    printf("---> decayingfactory Program:\n");
    debugp(decayingfactory_program);
    printf(">>>>\n");
#endif
    glUseProgram(decayingfactory_program);
    decayingfactory_iTime_location = glGetUniformLocation(decayingfactory_program, "iTime");
    decayingfactory_iResolution_location = glGetUniformLocation(decayingfactory_program, "iResolution");
    progress += .2/(float)nprograms;
}

void LoadPrograms()
{
    Loaddecayingfactory();
    updateBar();
}
#endif
