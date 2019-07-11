attribute vec3 aVer;
attribute vec2 aTex;

varying vec2 texCoords;

uniform mat4 projection;

void main()
{
    gl_Position = projection * vec4(aVer, 1);
    //gl_Position = vec4(aVer, 1);
    texCoords = aTex;
}  