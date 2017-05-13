#ifdef GL_ES
precision highp float;
#endif

uniform sampler2D tex_unit;

varying vec2 vtex_coord;

void main(void) {
     vec4 tex_color = texture2D(tex_unit, vtex_coord);
     gl_FragColor = tex_color;
}
