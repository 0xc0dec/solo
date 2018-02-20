{
    vertex = {
        inputs = {
            sl_Position = "vec4",
            slTexCoord = "vec2"
        },

        outputs = {
            uv = "vec2"
        },

        entry = [[
            gl_Position = sl_Position;
            uv = slTexCoord;
            SL_FIX_UV#uv#;
            SL_FIX_Y#gl_Position#;
        ]]
    },

    fragment = {
        uniformBuffers = {
            variables = {
                stitchCount = "vec2",
                resolution = "vec2"
            }
        },

        samplers = {
            mainTex = "sampler2D",
            stitchTex = "sampler2D"
        },

        outputs = {
            fragColor = "vec4"
        },

        entry = [[
            vec2 colorBlock = floor(uv * #variables:resolution#);
            vec4 color = texture(mainTex, uv);
            vec4 prevColor = texture(mainTex, vec2(uv.x, uv.y + 1.0 / #variables:resolution#.y));

            float stitchUvX = fract(#variables:stitchCount#.x * (uv.x + pow(colorBlock.y, 2.0) / #variables:resolution#.x * 2.0));
            float stitchUvY = fract(#variables:stitchCount#.y * uv.y);
            float nextStitchUvX = fract(#variables:stitchCount#.x * (uv.x + pow(colorBlock.y + 1.0, 2.0) / #variables:resolution#.x * 2.0));
            if (stitchUvY > 0.5)
            {
                vec2 stitchUV = vec2(stitchUvX, stitchUvY);
                fragColor = texture(stitchTex, stitchUV) * color;
                vec2 topStitchUV = vec2(nextStitchUvX, fract(stitchUvY - 0.5));
                vec4 topStitchColor = texture(stitchTex, topStitchUV) * prevColor;
                if (topStitchColor.a > 0.05)
                    fragColor = mix(fragColor, topStitchColor, 1.0 - fragColor.a);
            }
            else
            {
                vec2 stitchUV = vec2(nextStitchUvX, stitchUvY);
                fragColor = texture(stitchTex, stitchUV) * prevColor;
                vec2 bottomStitchUV = vec2(stitchUvX, fract(stitchUvY + 0.5));
                vec4 bottomStitchColor = texture(stitchTex, bottomStitchUV) * color;
                if (bottomStitchColor.a > 0.7)
                    fragColor = mix(bottomStitchColor, fragColor, 1.0 - prevColor.a);
            }

            fragColor = mix(vec4(0, 0, 0, 1), fragColor, fragColor.a);
        ]]
    }
}
