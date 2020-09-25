
#include "MirrorAxis.h"

ofx::dotfrag::MirrorAxis::MirrorAxis() {
    name("mirror axis");
    uniform( horizontal.set("horizontal", false), "u_horizontal" );
    uniform( vertical.set("vertical", false), "u_vertical");

    source( code );
}

const std::string ofx::dotfrag::MirrorAxis::code = OFXDOTFRAGSOURCE(

    //#ifdef GL_ES
    //precision mediump float;
    //#endif

    uniform vec2 u_resolution;
    uniform float u_time;
    uniform sampler2D u_tex0;

    uniform float u_vertical;
    uniform float u_horizontal;

    void main (void) {
        vec2 st = gl_FragCoord.xy/u_resolution.xy;
        
        float invx = step( 0.5, st.x ) * u_horizontal;
        st.x = mix( st.x, 1.0 - st.x, invx );
        
        float invy = step( 0.5, st.y ) * u_vertical;
        st.y = mix( st.y, 1.0 - st.y, invy );
        
        vec4 source = texture2D( u_tex0, st );

        gl_FragColor = source;
    }
    
); //OFXDOTFRAGSOURCE end
