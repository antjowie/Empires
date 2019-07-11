attribute vec3 aVerPos;
attribute vec2 aTexPos;

uniform mat4 mvp;
varying vec2 texPos;

void main()
{	
	gl_Position = mvp * vec4(aVerPos,1);
	texPos = aTexPos;
}