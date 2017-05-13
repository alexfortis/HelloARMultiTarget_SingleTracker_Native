uniform mat4 trans;
uniform mat4 proj;

attribute vec4 coord;
attribute vec2 tex_coord;

varying vec2 vtex_coord;

void main(void) {
     vtex_coord = tex_coord;
     gl_Position = coord;
}
