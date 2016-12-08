attribute vec4 position;

uniform mat4 modelViewMatrix;
uniform mat4 projectionMatrix;

varying vec3 varyingPosition;

void main(){
	gl_Position = projectionMatrix * modelViewMatrix * position;
	varyingPosition = position.xyz;
}