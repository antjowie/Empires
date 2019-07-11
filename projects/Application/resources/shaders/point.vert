uniform mat4 mvp;

attribute vec3 aVerPos;
attribute vec4 aTexPos; 
attribute float aScale;

varying vec4 color;

void main() {
	gl_PointSize = aScale;
    gl_Position = mvp * vec4(aVerPos.xy,0, 1);
    color = aTexPos;
}