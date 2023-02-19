shader_type spatial;
render_mode skip_vertex_transform;
render_mode unshaded;

uniform float u_radius;
uniform vec3 u_globe_rotation_inv_x;
uniform vec3 u_globe_rotation_inv_y;
uniform vec3 u_globe_rotation_inv_z;

vec2 stereographic_project(vec3 vert) {
    mat3 globe_rotation_inv = mat3(u_globe_rotation_inv_x, u_globe_rotation_inv_y, u_globe_rotation_inv_z);
    // remove rotation
    vert = globe_rotation_inv * vert;

    vec3 cam_pos = vec3(0, 0, u_radius);

    // project on tangential plane
    float d = dot(cam_pos, cam_pos);
    float r = d/(dot(vert, cam_pos));
    vert = r * vert;

    // translate back to origin -> z = 0
    return vert.xy;
}

void vertex() {
    vec2 projected_vertex = stereographic_project(VERTEX);
    VERTEX = vec3(projected_vertex, 0);
    VERTEX = (INV_CAMERA_MATRIX * vec4(VERTEX, 1.0)).xyz;
}

void fragment() {
    ALBEDO = COLOR.xyz;
}
