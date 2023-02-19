shader_type spatial;
render_mode skip_vertex_transform;
render_mode unshaded;

uniform float u_radius;
uniform vec4 u_globe_rotation_inv_0;
uniform vec4 u_globe_rotation_inv_1;
uniform vec4 u_globe_rotation_inv_2;
uniform vec4 u_globe_rotation_inv_3;

vec3 stereographic_project(vec4 vert) {
    mat4 globe_rotation_inv = mat4(u_globe_rotation_inv_0, u_globe_rotation_inv_1, u_globe_rotation_inv_2, u_globe_rotation_inv_3);
    // remove rotation
    vert = globe_rotation_inv * vert;

    vec4 cam_pos = vec4(0, 0, 0, u_radius);

    // project on tangential plane
    float d = dot(cam_pos, cam_pos);
    float r = d/(dot(vert, cam_pos));
    vert = r * vert;

    // translate back to origin -> w = 0
    return vert.xyz;
}

void vertex() {
    VERTEX = stereographic_project(vec4(VERTEX, UV.x));
    VERTEX = (INV_CAMERA_MATRIX * vec4(VERTEX, 1.0)).xyz;
}

void fragment() {
    ALBEDO = COLOR.xyz;
}
