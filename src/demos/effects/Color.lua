return {
    vertex = {
        uniformBuffers = {
            matrices = {
                wvp = "mat4"
            }
        },

        inputs = {
            position = "vec4"
        },

        entry = [[
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
            fragColor = "vec3"
        },

        entry = [[
            fragColor = #variables:color#.xyz;
        ]]
    }
}
