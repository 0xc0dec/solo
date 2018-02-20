{
    vertex = {
        inputs = {
            sl_Position = "vec4",
            slTexCoord = "vec2"
        },

        outputs = {
            uv = "vec2"
        },

        code = [[
            void main()
            {
                gl_Position = sl_Position;
                uv = slTexCoord;
                SL_FIX_Y#gl_Position#;
            }
        ]]
    },

    fragment = {
        uniformBuffers = {
            variables = {
                leftSeparator = "float",
                rightSeparator = "float"
            }
        },

        samplers = {
            mainTex = "sampler2D"
        },

        outputs = {
            fragColor = "vec4"
        },

        code = [[
            void main()
            {
                vec4 color = texture(mainTex, uv);
                if (uv.x >= #variables:leftSeparator# && uv.x <= #variables:rightSeparator#)
                    color *= 2;
                fragColor = color;
            }
        ]]
    }
}
