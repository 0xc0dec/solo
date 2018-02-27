{
    vertex = {
        uniformBuffers = {
            matrices = {
                wvp = "mat4"
            }
        },

        inputs = {
            sl_Position = "vec3"
        },

        outputs = {
        },

        code = [[
            void main()
            {
                gl_Position = #matrices:wvp# * vec4(sl_Position, 1);
                SL_FIX_Y#gl_Position#;
            }
        ]]
    },

    fragment = {
        outputs = {
            fragColor = { type = "vec4", target = 0 }
        },

        code = [[
            void main()
            {
                fragColor = vec4(1, 0, 0, 1);
            }
        ]]
    }
}