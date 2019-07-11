varying vec2 texPos;

uniform sampler2D sampler;
uniform vec3 color;

void main()
{
	gl_FragColor = texture2D(sampler, texPos) * vec4(color.xyz,1);
}