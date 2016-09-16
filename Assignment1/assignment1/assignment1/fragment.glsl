varying vec2 varyingTexCoord;
uniform sampler2D texture;
uniform vec2 mousePos;

void main() {
	gl_FragColor = texture2D(texture, varyingTexCoord + mousePos);
}