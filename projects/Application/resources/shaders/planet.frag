varying vec3 color;

void main() 
{
	float d = length(gl_PointCoord - vec2(0.5));
	if(d > 0.5)
		discard;

	gl_FragColor = vec4(color,1.);
}