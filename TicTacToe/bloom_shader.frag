// bloom_shader.frag
uniform sampler2D texture;
uniform float bloom_intensity;

void main()
{
    // Retrieve the texture coordinates
    vec2 texCoord = gl_TexCoord[0].xy;

    // Calculate the color for the current fragment
    vec3 color = texture2D(texture, texCoord).rgb;

    // Perform a simple horizontal blur
    color += texture2D(texture, texCoord + vec2(0.001, 0)).rgb;
    color += texture2D(texture, texCoord + vec2(0.002, 0)).rgb;
    // ... Add more blur steps as needed

    // Scale the blurred color to achieve bloom effect
    color *= bloom_intensity;

    // Output the final color
    gl_FragColor = vec4(color, 1.0);
}