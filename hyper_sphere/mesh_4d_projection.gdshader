shader_type spatial;
render_mode skip_vertex_transform;

// before world transformation
varying vec3 model_vertex;

void vertex() {
    // don't care about normal
    NORMAL = (MODELVIEW_MATRIX * vec4(VERTEX, 0.0)).xyz;

    model_vertex = VERTEX;
    VERTEX = (WORLD_MATRIX * vec4(VERTEX, 1.0)).xyz;
    
    // now play with world coordinates
    VERTEX.x = VERTEX.x + 1.0;
    VERTEX.y = VERTEX.y + 1.0;

    VERTEX = (INV_CAMERA_MATRIX * vec4(VERTEX, 1.0)).xyz;
}

void fragment() {
    ALBEDO = vec3(0.0, model_vertex.x , 0.8);
}
