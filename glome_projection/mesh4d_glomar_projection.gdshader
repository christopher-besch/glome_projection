shader_type spatial;
render_mode skip_vertex_transform;
render_mode unshaded;

// before world transformation
varying vec3 model_vertex;
uniform sampler2D texture;

void vertex() {
    // don't care about normal
    NORMAL = (MODELVIEW_MATRIX * vec4(VERTEX, 0.0)).xyz;

    model_vertex = VERTEX;
    VERTEX = (WORLD_MATRIX * vec4(VERTEX, 1.0)).xyz;

    float r = 1.0;
    VERTEX = vec3(
        r * sin(VERTEX.x) * cos(VERTEX.y),
        r * sin(VERTEX.x) * sin(VERTEX.y),
        r * cos(VERTEX.x)
    );

    VERTEX = (INV_CAMERA_MATRIX * vec4(VERTEX, 1.0)).xyz;
}

void fragment() {
    vec2 pos = model_vertex.xz / 6.28 + 0.5;
    ALBEDO = texture(texture, pos).rgb;
}

