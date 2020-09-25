
#include "Delay.h"


ofx::dotfrag::Delay::Delay() {
    name("DELAY");
    framebuffer( 6 );
    uniform( feedback.set("FEEDBACK", 0.0f, 0.0f, 1.0f), "u_feedback" );
    uniform( blendmode.set("BLENDMODE", 0, 0, 4), "u_blendmode" );
    source( code );

	//blendmode_name.set("NAME", "UNKNOWN");
	//blendmode_name.setSerializable(false);
}


const std::string ofx::dotfrag::Delay::code = OFXDOTFRAGSOURCE(

    //#ifdef GL_ES
    //precision mediump float;
    //#endif

    uniform vec2 u_resolution;
    uniform float u_time;
    
    uniform sampler2D u_tex0;
    uniform sampler2D u_tex1;
    
    uniform float u_feedback;
    uniform int u_blendmode;

    void main (void) {
        
        vec2 st = gl_FragCoord.xy/u_resolution;
        
        vec4 now = texture2D(u_tex0, st);
        vec4 delay = texture2D(u_tex1, st);
        
        float alpha = max( now.a, delay.a );
        
        if( u_blendmode==0 ){
			//blendmode_name = "SCREEN";
            // screen
            vec3 screen = 1.0-((1.0-now.xyz)*(1.0-delay.xyz));
            gl_FragColor = vec4(now.xyz*(1.0-u_feedback) + screen*u_feedback, alpha);
        }else if( u_blendmode==1 ){
			//blendmode_name = "SUM";
            // sum
            vec3 sum = min(now.xyz + delay.xyz*u_feedback, vec3(1.0));
            gl_FragColor = vec4( sum, alpha );
        }else if( u_blendmode==2 ){
			//blendmode_name = "LERP";
            // lerp
            gl_FragColor = vec4(now.xyz*(1.0-u_feedback) + delay.xyz*u_feedback, alpha);
        }else if( u_blendmode==3 ){
			//blendmode_name = "BLEND MAX";
            // blend max
            gl_FragColor = vec4( max(now.xyz, delay.xyz*u_feedback), alpha );
        }else{
			//blendmode_name = "BLEND MIN";
            // blend min
            gl_FragColor = vec4( min(now.xyz, delay.xyz*u_feedback), alpha );
        }
    }

); //OFXDOTFRAGSOURCE end
