shader_type spatial;
render_mode skip_vertex_transform;
render_mode unshaded;

uniform vec3 u_cam_pos;

vec3 stereographic_project(vec3 vert) {
    float d = dot(u_cam_pos, u_cam_pos);
    float r = d/(dot(vert, u_cam_pos));
    return r * vert - u_cam_pos;
}

void vertex() {
    VERTEX = stereographic_project(VERTEX);
    VERTEX = (INV_CAMERA_MATRIX * vec4(VERTEX, 1.0)).xyz;
}

void fragment() {
    ALBEDO = vec3(0.0f, 0.0f, 1.0f);
}
