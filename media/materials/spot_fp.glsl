varying vec3 normal;
uniform vec4 spotParams; // object space
uniform vec4 ldr; // object space
varying vec3 ldir;

void main() {
  vec3 nldir = normalize(ldir);
  float spotFactor = 0;
  float rho = clamp(dot(-nldir.xyz,ldr.xyz ), 0, 1);
  spotFactor = pow(clamp(rho - spotParams.y, 0, 1) / (spotParams.x - spotParams.y), spotParams.z);
  spotFactor = clamp(spotFactor, 0, 1);
  gl_FragColor = dot(nldir, normal) * spotFactor * vec4(1, 0.3, 0.3, 1.0);
}

