attribute vec3 aVerPos;
attribute vec2 aTexPos;

varying vec2 texPos;

uniform mat4 mvp;

void main()
{	
	gl_Position = mvp * vec4(aVerPos,1);
	texPos = aTexPos;
}