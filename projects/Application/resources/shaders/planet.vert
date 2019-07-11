uniform mat4 mvp;

attribute vec3 aVerPos;
attribute vec3 aColPos; 
attribute float aScale;

varying vec3 color;

void main() {
    gl_Position = mvp * vec4(aVerPos.xyz, 1);
	gl_PointSize = aScale / gl_Position.z;
    color = aColPos;
}