uniform mat4 trans;
uniform mat4 proj;

attribute vec4 coord;
attribute vec4 color;

varying vec4 vcolor;

void main(void) {
     vcolor = color;
     gl_Position = proj*trans*coord;
}
