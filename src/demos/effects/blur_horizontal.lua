{
    vertex = {
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
                gl_Position = sl_Position;
                uv = sl_TexCoord;
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
            fragColor = { type = "vec4", target = 0 }
        },

        code = [[
            void main()
            {
                const float offset[5] = float[](0.0, 0.002, 0.004, 0.006, 0.008);
                const float weight[5] = float[](0.2270270270, 0.1945945946, 0.1216216216, 0.0540540541, 0.0162162162);

                if (uv.x >= #variables:leftSeparator# && uv.x <= #variables:rightSeparator#)
                {
                    fragColor = texture(mainTex, uv) * weight[0];
                    for (int i = 1; i < 5; i++)
                    {
                        fragColor += texture(mainTex, uv + vec2(offset[i], 0)) * weight[i];
                        fragColor += texture(mainTex, uv - vec2(offset[i], 0)) * weight[i];
                    }
                }
                else
                    fragColor = texture(mainTex, uv);
            }
        ]]
    }
}
