uniform float time; 
uniform vec2 resolution; 

uniform float val1 = 1.0;
uniform float val2 = 1.0;


uniform float brkVal = 2.0; 

uniform float mod1 = 5.0; 
uniform float mod2 = 0.755; 
uniform float panMod = 0.3; 



uniform float imagineMod = 2.0;

uniform int iterations; 

vec2 complex_square(vec2 num)
{
	float real = num.x * num.x - num.y * num.y;
	float imaginary = num.x * num.y * imagineMod; 
	return vec2(real, imaginary); 
}

vec3 hsv(float h, float s, float v){
    vec4 t = vec4(1.0, 2.0 / 3.0, 1.0 / 3.0, 3.0);
    vec3 p = abs(fract(vec3(h) + t.xyz) * 6.0 - vec3(t.w));
    return v * mix(vec3(t.x), clamp(p - vec3(t.x), 0.0, 1.0), s);
}

void main(void) 
{

	vec2 uv = gl_FragCoord.xy / resolution * 2.0 - 1.0;
	//( gl_FragCoord.xy / resolution ) * 2.0 - 1.0;
	vec2 c = vec2(val1, val2); // This can be tweaked 
	
	vec2 v = uv;

	//c.y *= -1.0;
	//c /= pow(val1, time - 92.0);
	//c.y += val2 * sin(time) /128.0;
	//c.x -= sqrt(val3 * val3 - val4 * val4);

	c.y *= -1; 
	c /= pow(1.5, mod1 - 3.0);
	c.y += 0.1;
	c.x -= sqrt(mod2 * mod2 - 0.1 * 0.1);
	//c.y += val2 * sin(time) /128.0;
	//c.x -= sqrt(val3 * val3 - val4 * val4);
	

	int count = 200;
	for (int i = 0; i < iterations; i++)
	{
		v = c + complex_square(v);
		if (dot(v, v) > brkVal) {
			count = i;
			break;
		}
	}
	

	vec3 color = vec3(count / float(iterations));
	color.x = color.x * 5.0 * sin(time);
	color.z = color.z * 20.0;

	gl_FragColor = vec4(color, 1.0 );

}