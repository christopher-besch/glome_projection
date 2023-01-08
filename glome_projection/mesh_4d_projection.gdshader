shader_type spatial;
render_mode skip_vertex_transform;

// before world transformation
varying vec3 model_vertex;

void vertex() {
    // don't care about normal
    NORMAL = (MODELVIEW_MATRIX * vec4(VERTEX, 0.0)).xyz;

    model_vertex = VERTEX;
    VERTEX = (WORLD_MATRIX * vec4(VERTEX, 1.0)).xyz;

    float r = 1.0;
    VERTEX = vec3(
        r * sin(VERTEX.y) * cos(VERTEX.x),
        r * sin(VERTEX.y) * sin(VERTEX.x),
        r * cos(VERTEX.y)
    );

    VERTEX = (INV_CAMERA_MATRIX * vec4(VERTEX, 1.0)).xyz;
}

void fragment() {
    ALBEDO = vec3(model_vertex.y, model_vertex.x , 0.2);
}
