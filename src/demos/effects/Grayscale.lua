{
    vertex = {
        inputs = {
            slPosition = "vec4",
            slTexCoord = "vec2"
        },

        outputs = {
            uv = "vec2"
        },

        entry = [[
            gl_Position = slPosition;
            uv = slTexCoord;
            FIX_Y#gl_Position#;
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

        entry = [[
            vec4 color = texture(mainTex, uv);
            if (uv.x >= #variables:leftSeparator# && uv.x <= #variables:rightSeparator#)
            {
                float gray = dot(color.rgb, vec3(0.299, 0.587, 0.114));
                color = vec4(gray, gray, gray, 1);
            }
            fragColor = color;
        ]]
    }
}
