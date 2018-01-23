return {
    vertex = {
        uniformBuffers = {
            matrices = {
                wvp = "mat4"
            }
        },

        inputs = {
            position = "vec4",
            texCoord0 = "vec2"
        },

        outputs = {
            uv0 = "vec2"
        },

        entry = [[
            uv0 = texCoord0;
            gl_Position = #matrices:wvp# * position;
        ]]
    },

    fragment = {
        uniformBuffers = {
            variables = {
                color = "vec4"
            }
        },

        outputs = {
            fragColor = "vec4"
        },

        entry = [[
            float xfloor = floor(uv0.x / 0.2);
            float yfloor = floor(uv0.y / 0.2) + 1;
            float m = mod(xfloor, 2) + mod(yfloor, 2);
            if (m == 0 || m >= 2)
                fragColor = vec4(0, 0, 0, 1);
            else
                fragColor = #variables:color#;
        ]]
    }
}
