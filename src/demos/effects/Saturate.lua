return {
    vertex = {
        inputs = {
            position = "vec4",
            texCoord = "vec2"
        },

        outputs = {
            uv = "vec2"
        },

        entry = [[
            gl_Position = position;
            uv = texCoord;
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
        }

        outputs = {
            fragColor = "vec4"
        },

        entry = [[
            vec4 color = texture(mainTex, uv);
            if (uv.x >= #variables:leftSeparator# && uv.x <= #variables:rightSeparator#)
                color *= 2;
            fragColor = color;
        ]]
    }
}
