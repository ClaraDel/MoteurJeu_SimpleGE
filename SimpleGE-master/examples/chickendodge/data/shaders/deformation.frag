/* Rendu du jeu */
uniform sampler2D uSampler;

/* Texture de déformation en rouge et vert */
uniform sampler2D uDeformation;

/* Texture pour contrôler l'intensité de la déformation */
uniform sampler2D uIntensity;

/* Interval de temps multiplié par la vitesse depuis l'activation du composant */
uniform float uTime;

/* Échelle de la déformation */
uniform float uScale;

/* Coordonnées UV du fragment */
varying vec2 vTextureCoord;

vec4 intensite;
vec4 deformation;

void main(void) {
    
    intensite = texture2D(uIntensity, vec2(uTime, 0.5)) * uScale ;
    deformation = texture2D(uDeformation,vTextureCoord + vec2(50*sin(0.05*uTime), 50*cos(0.05*uTime)));
    
    deformation *= intensite[0];
    deformation[0] *= 0.4*sin(10 * uTime);
    deformation[1] *= 0.4*cos(10 *uTime);

    gl_FragColor = texture2D(uSampler,vTextureCoord + vec2(deformation[0], deformation[1]));
    
}
