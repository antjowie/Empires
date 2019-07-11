varying vec2 texPos;
uniform sampler2D sampler;

void main()
{
	vec4 color = texture2D(sampler, texPos);
	gl_FragColor = color;
}