varying vec2 texCoords;

uniform sampler2D text;
uniform vec3 textColor;

void main()
{   
    vec4 sampled = vec4(1.0, 1.0, 1.0, texture2D(text, texCoords).a);
	// For some reason lower alpha values render the background transparant
	//if(sampled.a > 0.0)
	//	sampled.a = 1.0;


    gl_FragColor = vec4(textColor, 1.0) * sampled;
	//gl_FragColor = vec4(0.8,0.0,0.0,1.0);
}