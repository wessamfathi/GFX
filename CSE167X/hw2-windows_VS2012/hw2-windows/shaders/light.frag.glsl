# version 130 

/* This is the fragment shader for reading in a scene description, including 
   lighting.  Uniform lights are specified from the main program, and used in 
   the shader.  As well as the material parameters of the object.  */

// Mine is an old machine.  For version 130 or higher, do 
// in vec4 color;  
// in vec3 mynormal; 
// in vec4 myvertex;
// That is certainly more modern

in vec4 color;
in vec3 mynormal;
in vec4 myvertex;

const int numLights = 10; 
uniform bool enablelighting; // are we lighting at all (global).
uniform vec4 lightposn[numLights]; // positions of lights 
uniform vec4 lightcolor[numLights]; // colors of lights
uniform int numused;               // number of lights used

// Now, set the material parameters.  These could be varying and/or bound to 
// a buffer.  But for now, I'll just make them uniform.  
// I use ambient, diffuse, specular, shininess as in OpenGL.  
// But, the ambient is just additive and doesn't multiply the lights.  

uniform vec4 ambient; 
uniform vec4 diffuse; 
uniform vec4 specular; 
uniform vec4 emission; 
uniform float shininess; 

vec4 ComputeLight (const in vec3 direction, const in vec4 lightcolor, const in vec3 normal, const in vec3 halfvec, const in vec4 mydiffuse, const in vec4 myspecular, const in float myshininess)
{
	float nDotL = dot(normal, direction);
	vec4 lambert = mydiffuse * lightcolor * max (nDotL, 0.0); 

	float nDotH = dot(normal, halfvec);
	vec4 phong = myspecular * lightcolor * pow (max(nDotH, 0.0), myshininess);

	vec4 retval = lambert + phong;
	return retval;
}       

void main (void) 
{       
    if (enablelighting)
	{
		const vec3 eyepos = vec3(0,0,0);
		vec4 _mypos = gl_ModelViewMatrix * myvertex;
		vec3 mypos = _mypos.xyz / _mypos.w;	// Dehomogenize current location 
		vec3 eyedirn = normalize(eyepos - mypos);

		// Compute normal, needed for shading. 
		vec3 normal = normalize(gl_NormalMatrix * mynormal);

		vec4 finalColor = vec4(0, 0, 0, 1);

		for (int i = 0; i < numLights; ++i)
		{
			if (lightposn[i].w == 0)
			{
				// Directional Light
				vec3 direction0 = normalize (lightposn[i].xyz) ; 
				vec3 half0 = normalize (direction0 + eyedirn) ; 
				vec4 result = ComputeLight(direction0, lightcolor[i], normal, half0, diffuse, specular, shininess) ;
				finalColor += result;
			}
			else
			{
				// Point Light
				vec3 position = lightposn[i].xyz / lightposn[i].w ; 
				vec3 direction1 = normalize (position - mypos) ; // no attenuation 
				vec3 half1 = normalize (direction1 + eyedirn) ;  
				vec4 result = ComputeLight(direction1, lightcolor[i], normal, half1, diffuse, specular, shininess) ;
				finalColor += result;
			}
		}

		gl_FragColor = finalColor;
	}
	else
	{
		gl_FragColor = color; 
    }
}
