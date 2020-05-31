{
    vertex = {
        uniformBuffers = {
            uniforms = {
                wvp = "mat4",
                world = "mat4",
                lightVp = "mat4",
                lightPos = "vec3"
            }
        },

        inputs = {
            sl_Position = "vec3",
            sl_TexCoord = "vec2",
            sl_Normal = "vec3",
            sl_Tangent = "vec3"
        },

        outputs = {
            uv = "vec2",
            shadowCoord = "vec4",
            worldNormal = "vec3",
            worldDirToLight = "vec3",
            tbn = "mat3"
        },

        code = [[
            void main()
            {
                const mat4 biasMat = SL_SHADOW_BIAS_MAT;

                uv = sl_TexCoord;
                SL_FIX_UV#uv#;

                gl_Position = #uniforms:wvp# * vec4(sl_Position, 1);
                SL_FIX_Y#gl_Position#;

                vec4 lightProjectedPos = (#uniforms:lightVp# * #uniforms:world#) * vec4(sl_Position, 1.0);
                SL_FIX_Y#lightProjectedPos#;
                shadowCoord = biasMat * lightProjectedPos;

                worldNormal = mat3(#uniforms:world#) * sl_Normal;
                worldDirToLight = normalize(#uniforms:lightPos# - sl_Position);

                vec3 worldTangent = normalize(vec3(#uniforms:world# * vec4(sl_Tangent, 0.0)));
                vec3 worldBinormal = cross(worldNormal, worldTangent);
                tbn = mat3(worldTangent, worldBinormal, worldNormal);
            }
        ]]
    },

    fragment = {
        uniformBuffers = {
            variables = {
                highlighted = "float"
            }
        },

        samplers = {
            colorMap = "sampler2D",
            normalMap = "sampler2D",
            shadowMap = "sampler2D"
        },

        outputs = {
            fragColor = { type = "vec4", target = 0 }
        },

        code = [[
            const float ambient = 0.1;

            float sampleShadow(vec4 coords, vec2 offset)
            {
                float shadow = 1.0;
                float dist = texture(shadowMap, coords.st + offset).r;
                if (dist < coords.z - 0.00002)
                    shadow = ambient;
                return shadow;
            }

            float samplePCF(vec4 coords)
            {
                ivec2 texDim = textureSize(shadowMap, 0);
                float scale = 1.5;
                float dx = scale * 1.0 / float(texDim.x);
                float dy = scale * 1.0 / float(texDim.y);

                float shadowFactor = 0.0;
                int count = 0;
                int range = 1;
                
                for (int x = -range; x <= range; x++)
                {
                    for (int y = -range; y <= range; y++)
                    {
                        shadowFactor += sampleShadow(coords, vec2(dx * x, dy * y));
                        count++;
                    }
                
                }
                return shadowFactor / count;
            }

            void main()
            {
                vec3 n = normalize(tbn * (texture(normalMap, uv).rgb * 2 - 1));
                vec3 l = normalize(worldDirToLight);
                
                float diffuse = max(dot(n, l), ambient);
                float shadow = samplePCF(shadowCoord / shadowCoord.w);

                fragColor = texture(colorMap, uv) * min(diffuse, shadow);

                if (#variables:highlighted# > 0)
                    fragColor.r *= 2;
            }
        ]]
    }
}