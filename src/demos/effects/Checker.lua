{
    vertex = {
        uniformBuffers = {
            matrices = {
                wvp = "mat4"
            }
        },

        inputs = {
            sl_Position = "vec4",
            slTexCoord = "vec2"
        },

        outputs = {
            uv = "vec2"
        },

        entry = [[
            uv = slTexCoord;
            gl_Position = #matrices:wvp# * sl_Position;
            SL_FIX_Y#gl_Position#;
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
            float xfloor = floor(uv.x / 0.2);
            float yfloor = floor(uv.y / 0.2) + 1;
            float m = mod(xfloor, 2) + mod(yfloor, 2);
            if (m == 0 || m >= 2)
                fragColor = vec4(0, 0, 0, 1);
            else
                fragColor = #variables:color#;
        ]]
    }
}
