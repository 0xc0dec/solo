{
    vertex = {
        uniformBuffers = {
            matrices = {
                worldView = "mat4",
                proj = "mat4"
            }
        },

        inputs = {
            sl_Position = "vec4"
        },

        outputs = {
            eyeDir = "vec3"
        },

        code = [[
            void main()
            {
                vec4 pos = sl_Position;
                SL_FIX_Y#pos#;

                mat4 invProjMatrix = inverse(#matrices:proj#);
                mat3 invModelViewMatrix = inverse(mat3(#matrices:worldView#));
                vec3 unprojected = (invProjMatrix * pos).xyz;
                eyeDir = invModelViewMatrix * unprojected;
                gl_Position = sl_Position;
                SL_FIX_Y#eyeDir#;
            }
        ]]
    },

    fragment = {
        samplers = {
            colorMap = "samplerCube"
        },

        outputs = {
            fragColor = { type = "vec4", target = 0 }
        },

        code = [[
            void main()
            {
                fragColor = texture(colorMap, eyeDir);
            }
        ]]
    }
}