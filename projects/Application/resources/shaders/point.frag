varying vec4 color;

void main() 
{
	if(length(gl_PointCoord - vec2(0.5)) > 0.5)
		discard;

	gl_FragColor = color;
}