#version 330 core
in vec2 uv;
out vec4 FragColor;

uniform vec4 color;      // base color of the glow
uniform float thickness; // how thick the ring core is (e.g. 0.01)
uniform float glow;      // how far the glow fades outward (e.g. 0.02)

void main() {
    float dist = distance(uv, vec2(0.5));
    float edgeInner = 0.5 - thickness;
    float edgeOuter = 0.5;

    // Add a glow band outside the edge
    float outerFadeStart = edgeOuter;
    float outerFadeEnd = edgeOuter + glow;

    // Fade from full opacity at edgeOuter to zero at edgeOuter + glow
    float alpha = 1.0 - smoothstep(outerFadeStart, outerFadeEnd, dist);

    // If you want to also fade in on the *inner* edge:
    float innerFadeStart = edgeInner - glow;
    float innerFadeEnd = edgeInner;

    alpha *= smoothstep(innerFadeStart, innerFadeEnd, dist);

    if(thickness == 0 && dist < 0.5f) alpha = 1.0f;

    if(alpha == 0) discard;

    // Final output with faded alpha
    FragColor = vec4(color.rgb, color.a * alpha);
}