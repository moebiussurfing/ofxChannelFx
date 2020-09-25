
#include "EchoTrace.h"


ofx::dotfrag::EchoTrace::EchoTrace() {
    name("ECHOTRACE");
    pingpong(); // activate ping pong buffer
    uniform( gain.set("GAIN", 0.7f, 0.0f, 2.0f), "u_gain" );
    uniform( threshold.set("THRESHOLD", 0.55f, 0.0f, 1.0f), "u_threshold");
    uniform( invert.set("INVERT", false), "u_invert" );
    uniform( hardCutoff.set("HARDCUTOFF", false), "u_hardcutoff" );
    source( code );
}


const std::string ofx::dotfrag::EchoTrace::code = OFXDOTFRAGSOURCE(

    //#ifdef GL_ES
    //precision mediump float;
    //#endif

    uniform vec2 u_resolution;
    uniform float u_time;
    
    uniform sampler2D u_tex0;
    uniform sampler2D u_tex1;

    uniform float u_gain;
    uniform float u_threshold;
    uniform float u_invert;
    uniform float u_hardcutoff;

    void main (void) {
        vec2 st = gl_FragCoord.xy/u_resolution;
        
        vec4 freshPixel = texture2D( u_tex0, st );
        vec4 stalePixel = texture2D( u_tex1, st );
        float brightLevel = 0.299*freshPixel.r +  0.587*freshPixel.g + 0.114*freshPixel.b;
        
        // invert
        brightLevel = mix( brightLevel, 1.0 - brightLevel, u_invert );
        
        brightLevel = brightLevel * u_gain;
        
        // hard cutof
        brightLevel = mix( brightLevel, step( u_threshold, brightLevel ), u_hardcutoff );
    
        gl_FragColor = mix( freshPixel, stalePixel, brightLevel);
        
    }

); //OFXDOTFRAGSOURCE end
