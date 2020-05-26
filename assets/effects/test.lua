-- Dumb effect for tests
{
    vertex = {
        uniformBuffers = {
            matrices = {
                wvp = "mat4"
            },

            variables = {
                f = "float",
                v2 = "vec2",
                v3 = "vec3",
                v4 = "vec4"
            }
        },

        inputs = {
            sl_Position = "vec4"
        },

        code = [[
            void main()
            {
                gl_Position = #matrices:wvp# * sl_Position;
                gl_Position.x = #variables:f# * #variables:v2#.x * #variables:v3#.x * #variables:v4#.x;
                SL_FIX_Y#gl_Position#;
            }
        ]]
    },

    fragment = {
        uniformBuffers = {
            variables = {
                color = "vec4",
                tex = "sampler2D"
            }
        },

        outputs = {
            fragColor = { type = "vec4", target = 0 }
        },

        code = [[
            void main()
            {
                fragColor = #variables:color# * texture(#variables:tex#, vec2(0, 0));
            }
        ]]
    }
}
