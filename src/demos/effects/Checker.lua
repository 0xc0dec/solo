{
    vertex = {
        uniformBuffers = {
            matrices = {
                wvp = "mat4"
            }
        },

        inputs = {
            sl_Position = "vec4",
            sl_TexCoord = "vec2"
        },

        outputs = {
            uv = "vec2"
        },

        code = [[
            void main()
            {
                uv = sl_TexCoord;
                gl_Position = #matrices:wvp# * sl_Position;
                SL_FIX_Y#gl_Position#;
            }
        ]]
    },

    fragment = {
        uniformBuffers = {
            variables = {
                color = "vec4"
            }
        },

        outputs = {
            fragColor = { type = "vec4", target = 0 }
        },

        code = [[
            void main()
            {
                float xfloor = floor(uv.x / 0.2);
                float yfloor = floor(uv.y / 0.2) + 1;
                float m = mod(xfloor, 2) + mod(yfloor, 2);
                if (m == 0 || m >= 2)
                    fragColor = vec4(0, 0, 0, 1);
                else
                    fragColor = #variables:color#;
            }
        ]]
    }
}
