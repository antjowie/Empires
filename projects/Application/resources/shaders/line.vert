attribute vec3 aVerPos;
attribute vec3 aColPos;

uniform mat4 mvp;

varying vec3 color;

void main()
{
	gl_Position = mvp * vec4(aVerPos.xyz,1);
	color = aColPos;
}