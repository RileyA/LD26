varying vec3 normal;
varying vec3 pos;
uniform vec4 eye_pos; // object space
uniform sampler2D diffuseMap;
varying vec2 uv;

void main() {
  float eye_dist = length(pos.xyz - eye_pos.xyz);
  float fog = clamp((eye_dist - 150.0) / 150.0,0.0,1.0);
  gl_FragColor = mix(texture2D(diffuseMap, uv), vec4(1.0,1.0,1.0,1.0), fog); 
}
