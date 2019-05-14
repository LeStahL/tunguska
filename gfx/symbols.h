//Generated with Symbolize (c) 2019 Alexander Kraus <nr4@z10.info>.
#ifndef SYMBOLIZE_H
#define SYMBOLIZE_H

int rand_handle, lfnoise_handle, mfnoise_handle, dbox_handle, dbox3_handle, dvoronoi_handle;
const int nsymbols = 6;
const char *rand_source = "#version 130\n"
"void main();"
"uniform float iTime;"
"uniform vec2 iResolution;"
"void rand(in vec2 x, out float n)"
"{"
"    x += 400.;"
"    n = fract(sin(dot(sign(x)*abs(x) ,vec2(12.9898,78.233)))*43758.5453);"
"}"
"";
const char *lfnoise_source = "#version 130\n"
"void main();"
"uniform float iTime;"
"uniform vec2 iResolution;"
"const vec3 c = vec3(1.,0.,-1.);"
"void rand(in vec2 x, out float d);"
"void lfnoise(in vec2 t, out float n)"
"{"
"    vec2 i = floor(t);"
"    t = fract(t);"
"    t = smoothstep(c.yy, c.xx, t);"
"    vec2 v1, v2;"
"    rand(i, v1.x);"
"    rand(i+c.xy, v1.y);"
"    rand(i+c.yx, v2.x);"
"    rand(i+c.xx, v2.y);"
"    v1 = c.zz+2.*mix(v1, v2, t.y);"
"    n = mix(v1.x, v1.y, t.x);"
"}"
"";
const char *mfnoise_source = "#version 130\n"
"void main();"
"uniform float iTime;"
"uniform vec2 iResolution;"
"const vec3 c = vec3(1.,0.,-1.);"
"void lfnoise(in vec2 x, out float d);"
"void mfnoise(in vec2 x, in float d, in float b, in float c, out float n)"
"{"
"    n = 0.;"
"    float a = 1., nf = 0., buf;"
"    for(float f = d; f<b; f *= 2.)"
"    {"
"        lfnoise(f*x, buf);"
"        n += a*buf;"
"        a *= c;"
"        nf += 1.;"
"    }"
"    n *= (1.-c)/(1.-pow(c, nf));"
"}"
"";
const char *dbox_source = "#version 130\n"
"const vec3 c = vec3(1.,0.,-1.);"
"void main();"
"uniform float iTime;"
"uniform vec2 iResolution;"
"void dbox(in vec2 x, in vec2 b, out float d)"
"{"
"    vec2 da = abs(x)-b;"
"    d = length(max(da,c.yy)) + min(max(da.x,da.y),0.0);"
"}"
"";
const char *dbox3_source = "#version 130\n"
"void main();"
"uniform float iTime;"
"uniform vec2 iResolution;"
"void dbox3(in vec3 x, in vec3 b, out float d)"
"{"
"  vec3 da = abs(x) - b;"
"  d = length(max(da,0.0))"
"         + min(max(da.x,max(da.y,da.z)),0.0);"
"}"
"";
const char *dvoronoi_source = "#version 130\n"
"void main();"
"uniform float iTime;"
"uniform vec2 iResolution;"
"const vec3 c = vec3(1.,0.,-1.);"
"void rand(in vec2 x, out float d);"
"void dvoronoi(in vec2 x, out float d, out vec2 z)"
"{"
"    vec2 y = floor(x);"
"       float ret = 1.;"
"    vec2 pf=c.yy, p;"
"    float df=10.;"
"    "
"    for(int i=-1; i<=1; i+=1)"
"        for(int j=-1; j<=1; j+=1)"
"        {"
"            p = y + vec2(float(i), float(j));"
"            float pa;"
"            rand(p, pa);"
"            p += pa;"
"            "
"            d = length(x-p);"
"            "
"            if(d < df)"
"            {"
"                df = d;"
"                pf = p;"
"            }"
"        }"
"    for(int i=-1; i<=1; i+=1)"
"        for(int j=-1; j<=1; j+=1)"
"        {"
"            p = y + vec2(float(i), float(j));"
"            float pa;"
"            rand(p, pa);"
"            p += pa;"
"            "
"            vec2 o = p - pf;"
"            d = length(.5*o-dot(x-pf, o)/dot(o,o)*o);"
"            ret = min(ret, d);"
"        }"
"    "
"    d = ret;"
"    z = pf;"
"}"
"";
const char *decayingfactory_source = "/* Endeavor by Team210 - 64k intro by Team210 at Revision 2k19"
"* Copyright (C) 2018  Alexander Kraus <nr4@z10.info>"
"*"
"* This program is free software: you can redistribute it and/or modify"
"* it under the terms of the GNU General Public License as published by"
"* the Free Software Foundation, either version 3 of the License, or"
"* (at your option) any later version."
"*"
"* This program is distributed in the hope that it will be useful,"
"* but WITHOUT ANY WARRANTY; without even the implied warranty of"
"* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the"
"* GNU General Public License for more details."
"*"
"* You should have received a copy of the GNU General Public License"
"* along with this program.  If not, see <https://www.gnu.org/licenses/>."
"*/"
""
"#version 130\n\n"
""
"uniform float iTime;"
"uniform vec2 iResolution;"
""
"// Global constants"
"const float pi = acos(-1.);"
"const vec3 c = vec3(1.0, 0.0, -1.0);"
"float a = 1.0;"
""
"// Hash function"
"void rand(in vec2 x, out float num);"
"void lfnoise(in vec2 t, out float num);"
"void mfnoise(in vec2 x, in float fmin, in float fmax, in float alpha, out float num);"
"void dbox(in vec2 x, in vec2 b, out float d);"
"void dbox3(in vec3 x, in vec3 b, out float d);"
"void dvoronoi(in vec2 x, out float d, out vec2 ind);"
""
"// Stroke"
"void stroke(in float d0, in float s, out float d)"
"{"
"    d = abs(d0)-s;"
"}"
""
"void colorize_wall_concrete(in vec2 x, out vec3 col)"
"{"
"    float w = .4,"
"        d;"
"    float n, n2, n3, n4;"
"    mfnoise(x, 8.,8.e2, .68, n);"
"    lfnoise(2.*x, n2);"
"    col = mix(vec3(0.85,0.84,0.82), vec3(0.07,0.05,0.04), .5+.5*n);"
"    "
"     // white"
"    col = mix(col, mix(vec3(0.85,0.84,0.82), vec3(0.86,0.83,0.79), .8+.2*n), smoothstep(.0,.3,n*n2));"
"    "
"    // Red paint"
"    mfnoise(x, .4,2.e2, .48, n3);"
"    "
"    //gray"
"    col = mix(col, c.yyy, smoothstep(-.2,.4, .3+n3-n));"
"    float na;"
"    lfnoise(iTime*c.xx, na);"
"    col = mix(col, c.yyy, (.5+.5*na)*smoothstep(.1,-.1,abs(.3+n3-n)-.1));"
"    col = mix(col, mix(vec3(0.45,0.06,0.01), vec3(0.69,0.14,0.04), (.1+.9*n)*smoothstep(-.1,.4,.3+1.4*n3+.7*n)), smoothstep(.3,.7,.3+1.4*n3-.5*n));"
"}"
""
"void colorize_tiles(in vec2 x, out vec3 col)"
"{"
"    //x.x += .2*iTime;"
"    "
"    float w = .4,"
"        d;"
"    vec2 y = mod(x, w)-.5*w;"
"    float n;"
"    mfnoise(x, 4.,4.e2, .48, n);"
"    "
"    dbox(y, .47*w*c.xx, d);"
"    d += .005*n;"
"    col = mix(vec3(0.90,0.84,0.80),1.2*vec3(1.00,0.95,0.86), .5+.5*n);"
"    "
"    // Dirt/rust"
"    float nr;"
"    mfnoise(x*vec2(12.,1.), 1.,1.e1, .85, nr);"
"    col = mix(col, 1.4*vec3(0.80,0.58,0.22), clamp(.1+.9*nr,0.,1.));"
"    "
"    // cracks"
"    float v;"
"    vec2 vi;"
"    dvoronoi((x.xy-.1*n)/w,v, vi); "
"    col = mix(col, vec3(0.54,0.48,0.45), smoothstep(1.5/iResolution.y, -1.5/iResolution.y, abs(v)-.01));"
"   "
"    // joint"
"    col = mix(vec3(0.22,0.22,0.22), col, smoothstep(1.5/iResolution.y, -1.5/iResolution.y, d));"
"    "
"    // holes"
"    float na;"
"    mfnoise(x, 4.,4.e2, .98, na);"
"    col = mix(col, mix(vec3(0.54,0.48,0.45), .0*c.xxx, .5+.5*na), smoothstep(1.5/iResolution.y, -1.5/iResolution.y, .3+n));"
"}"
""
"void colorize_wall(in vec2 x, out vec3 col)"
"{"
"    if(x.y > 0.) colorize_wall_concrete(x, col);"
"    else colorize_tiles(x, col);"
"}"
""
"void colorize_pipes(in vec2 x, out vec3 col)"
"{"
"    float n, nlo;"
"    "
"    mfnoise(x,4.8e1, 4.8e3, .65, n);"
"    lfnoise(6.*x, nlo);"
"    col = mix(vec3(0.37,0.07,0.00), vec3(0.62,0.63,0.66), .5+.5*n);"
"    "
"    vec3 c1 = mix(vec3(1.00,0.84,0.70), vec3(0.37,0.07,0.00), .5+.5*n);"
"    col = mix(col, c1, smoothstep(n-.1,n+.1,nlo));"
"    col = mix(col, vec3(0.64,0.39,0.32), (.5+.5*n)*smoothstep(n-.1,n+.1,nlo));"
"    "
"    float nb;"
"    stroke(n, .1, nb);"
"    col = mix(col, vec3(0.58,0.33,0.26), smoothstep(1.5/iResolution.y, -1.5/iResolution.y, nb));"
"}"
""
"void wall_tiles(in vec3 x, out float d)"
"{"
"    //x.x += .2*iTime;"
"    "
"    float n;"
"    mfnoise(x.xy, 4.,4.e2, .45, n);"
""
"    float w = .4;"
"    vec2 y = mod(x.xy, w)-.5*w;"
"    dbox(y, .47*w*c.xx, d);"
"    "
"    // cracks"
"    float v;"
"    vec2 vi;"
"    dvoronoi((x.xy-.1*n)/w,v, vi); "
"    "
"    v = mix(v, 1., smoothstep(1.5/iResolution.y, -1.5/iResolution.y, .3+n));"
"    "
"    d = x.z"
"        + .008*n"
"        + .2*smoothstep(1.5/iResolution.y, -1.5/iResolution.y, abs(v)-.01)"
"        +.3*smoothstep(2.5/iResolution.y, -2.5/iResolution.y, d+.005*n)*smoothstep(-1.5/iResolution.y,1.5/iResolution.y,.3+n)"
"        -.2*(.5*n+.5)*smoothstep(1.5/iResolution.y,-1.5/iResolution.y,.3+n);"
"}"
""
"void wall_concrete(in vec3 x, out float d)"
"{"
"    float n;"
"    mfnoise(x.xy, 8.,8.e2, .68, n);"
"    "
"    d = x.z"
"        +.015*(.5+.5*n);"
"}"
""
"void pipes(in vec3 x, out float d)"
"{"
"    float n, nlo, na;"
"    mfnoise(x.xy,4.8e1, 4.8e3, .65, na);"
"    mfnoise(x.xy,1.8e1, 4.8e3, .55, n);"
"    lfnoise(6.*x.xy, nlo);"
"    "
"    d = x.z+.015*n-.015*smoothstep(na-.1,na+.1,nlo);"
"}"
""
"void pipes_normal(in vec3 x, out vec3 n)"
"{"
"    const float dx = 5.e-4;"
"    float s;"
"    "
"    pipes(x,s);"
"    pipes(x+dx*c.xyy, n.x);"
"    pipes(x+dx*c.yxy, n.y);"
"    pipes(x+dx*c.yyx, n.z);"
"    n = normalize(n-s);"
"}"
""
"void wall_tiles_normal(in vec3 x, out vec3 n)"
"{"
"    const float dx = 5.e-4;"
"    float s;"
"    "
"    wall_tiles(x,s);"
"    wall_tiles(x+dx*c.xyy, n.x);"
"    wall_tiles(x+dx*c.yxy, n.y);"
"    wall_tiles(x+dx*c.yyx, n.z);"
"    n = normalize(n-s);"
"}"
""
"void wall_concrete_normal(in vec3 x, out vec3 n)"
"{"
"    const float dx = 5.e-4;"
"    float s;"
"    "
"    wall_concrete(x,s);"
"    wall_concrete(x+dx*c.xyy, n.x);"
"    wall_concrete(x+dx*c.yxy, n.y);"
"    wall_concrete(x+dx*c.yyx, n.z);"
"    n = normalize(n-s);"
"}"
""
"void add(in vec2 sda, in vec2 sdb, out vec2 sdf)"
"{"
"    sdf = mix(sda, sdb, step(sdb.x, sda.x));"
"}"
""
"void scene(in vec3 x, out vec2 sdf)"
"{"
"    float d;"
"    "
"    dbox3(x, 112.*c.xxx, sdf.x);"
"    "
"    // Remove corridor"
"    float w = mix(.4,.38, smoothstep(1.5/iResolution.y,-1.5/iResolution.y,x.y));"
"    dbox3(x, vec3(w,1.2*w,56.), d);"
"    sdf.x = max(sdf.x, -d);"
"    dbox3(vec3(x.xy, abs(mod(x.z,2.)))-.8*c.yyx, vec3(2.,1.2*w,w), d);"
"    sdf.x = max(sdf.x, -d);"
"    sdf.y = 1.;"
"    "
"    // Add pipes"
"    if(abs(x.x) < .4)"
"    {"
"        float wa = .1;"
"        vec2 sda = vec2(length(vec2(mod(x.x,wa)-.5*wa, x.y-1.2*w))-.4*wa, 2.);"
"        add(sdf, sda, sdf);"
"    }"
"    "
"    // Add Water"
"    float n, n2;"
"    lfnoise(12.*x.xz-iTime*c.yx+.1*iTime*c.xy, n);"
"    lfnoise(34.5*x.xz-3.141*iTime*c.yx-.1*iTime*c.xy, n2);"
"    vec2 sdb = vec2(x.y+.38-.002*(.7*n+.3*n2),3.);"
"    add(sdf, sdb, sdf);"
"}"
""
"void normal(in vec3 x, out vec3 n)"
"{"
"    const float dx = 5.e-4;"
"    vec2 s, na;"
"    "
"    scene(x,s);"
"    scene(x+dx*c.xyy, na);"
"    n.x = na.x;"
"    scene(x+dx*c.yxy, na);"
"    n.y = na.x;"
"    scene(x+dx*c.yyx, na);"
"    n.z = na.x;"
"    n = normalize(n-s.x);"
"}"
""
"void colorize(in vec3 x, in vec2 s, inout vec3 n, out vec3 col)"
"{"
"    if(s.y == 1.)"
"        {"
"            // March volumetric texture"
"            if(abs(dot(n,c.xyy))+abs(dot(n,c.yxy)) < 1.e-4)"
"            {"
"                //Front wall"
"                colorize_wall(6.*x.xy, col);"
"                if(x.y < 0.) wall_tiles_normal(6.*x*c.xxy, n);"
"                else wall_concrete_normal(6.*x*c.xxy, n);"
"            }"
"            else if(abs(dot(n,c.yxy))+abs(dot(n,c.yyx)) < 1.e-4)"
"            {"
"                //Side Wall"
"                colorize_wall(6.*x.zy, col);"
"                if(x.y < 0.) wall_tiles_normal(6.*x.zyx, n);"
"                else wall_concrete_normal(6.*x.zyx, n);"
"            }"
"            else "
"            {"
"                // Floor"
"                if(x.y < 0.)"
"                {"
"                    colorize_wall(6.*x.zx-23., col);"
"                    wall_tiles_normal(6.*x.zxy-23.*c.xxy, n);"
"                }"
"                else"
"                {"
"                    colorize_wall(6.*x.zx+23., col);"
"                    wall_concrete_normal(6.*x.zxy+23.*c.xxy, n);"
"                }"
"                    //col = .2*c.xxx;"
"            }"
"        }"
"        else if(s.y == 2.)"
"        {"
"            colorize_pipes(6.*x.zx-23., col);"
"            pipes_normal(6.*x.zxy-23.*c.xxy, n);"
"        }"
"}"
""
"void mainImage( out vec4 fragColor, in vec2 fragCoord )"
"{"
"    a = iResolution.x/iResolution.y;"
"    vec2 uv = fragCoord/iResolution.yy-0.5*vec2(a, 1.0);"
"    vec3 col = c.yyy;"
"    "
"    vec3 t = vec3(uv, 0.)"
"            -mix(.3,.7, 0.*smoothstep(0.,1.,sin(pi*iTime)))*iTime*c.yyx // forward"
"            +.01*abs(cos(2.*pi*iTime))*c.yxy // up/down"
"            + .005*sin(2.*pi*iTime)*c.xyy, // right/left"
"        o = c.yyx"
"            -.3*iTime*c.yyx"
"            +.01*abs(sin(2.*pi*iTime))*c.yxy"
"            + .005*sin(2.*pi*iTime)*c.xyy,"
"        dir = normalize(t-o),"
"        x, n;"
"    float d = 0.;"
"    vec2 s;"
"    int N = 300, i;"
"    "
"    for(i=0; i<N; ++i)"
"    {"
"        x = o + d * dir;"
"        scene(x, s);"
"        if(s.x < 1.e-4) break;"
"        d += s.x;"
"    }"
"      "
"    //if(i<N)"
"    {"
"        normal(x, n);"
"        float d0 = d;"
"        if(s.y == 3.)"
"        {"
"            o = x;"
"            d = .002;"
"            dir = reflect(dir, n);"
"            "
"            for(i=0; i<N; ++i)"
"            {"
"                x = o + d * dir;"
"                scene(x, s);"
"                if(s.x < 1.e-4) break;"
"                d += s.x;"
"            }"
"            "
"            //if(i<N)"
"                normal(x, n);"
"            d = abs(d)+ abs(d0);"
"        }"
"        colorize(x, s, n, col);"
"        "
"        vec3 l = -abs(normalize(o)/max(d*d,1.));"
"        col = .1*col/min(d,4.)"
"            + .6*col*abs(dot(l,n))"
"            + .3*col*abs(pow(dot(reflect(-l,n),dir),4.));"
"    }"
"    "
"    col = clamp(col, 0., 1.);"
"    "
"    float na;"
"    lfnoise(iTime*c.xx, na);"
"    vec3 gs = length(col)*c.xxx;"
"    col = mix(col, gs, .5+.5*na);"
"    "
"    fragColor = vec4(col,1.0);"
"}"
""
"void main()"
"{"
"    mainImage(gl_FragColor, gl_FragCoord.xy);"
"}"
"";
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
    glAttachShader(decayingfactory_program,rand_handle);
    glAttachShader(decayingfactory_program,lfnoise_handle);
    glAttachShader(decayingfactory_program,mfnoise_handle);
    glAttachShader(decayingfactory_program,dbox_handle);
    glAttachShader(decayingfactory_program,dbox3_handle);
    glAttachShader(decayingfactory_program,dvoronoi_handle);
    glAttachShader(decayingfactory_program,decayingfactory_handle);
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
