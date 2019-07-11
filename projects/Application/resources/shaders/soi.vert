attribute vec3 aVerPos;

uniform mat4 mvp;

void main()
{
	gl_Position = mvp * vec4(aVerPos,1);
}