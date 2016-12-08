varying vec3 varyingPosition;
uniform samplerCube environmentMap;

void main(){
	gl_FragColor = textureCube(environmentMap, normalize(varyingPosition));
}