uniform sampler2D screenFramebuffer;
varying vec2 varyingTexCoord;

void main(){
	gl_FragColor = texture2D(screenFramebuffer, varyingTexCoord);
}