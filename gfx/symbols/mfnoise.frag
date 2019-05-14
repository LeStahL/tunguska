#version 130
const vec3 c = vec3(1.,0.,-1.);
void lfnoise(in vec2 x, out float d);
void mfnoise(in vec2 x, in float a, in float b, in float c, out float n)
{
    n = 0.;
    float a = 1., nf = 0., buf;
    for(float f = a; f<b; f *= 2.)
    {
        lfnoise(f*x, buf);
        n += a*buf;
        a *= c;
        nf += 1.;
    }
    n *= (1.-c)/(1.-pow(c, nf));
}
