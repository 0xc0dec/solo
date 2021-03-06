{
    vertex = {
        uniformBuffers = {
            uniforms = {
                wvp = "mat4",
                world = "mat4",
                lightVp = "mat4",
                lightDir = "vec3",
                camPos = "vec3"
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
            tangentPos = "vec3",
            tangentCamPos = "vec3",
            tangentLightDir = "vec3"
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

                mat4 worldMat = #uniforms:world#;
                mat3 normalMat = transpose(inverse(mat3(worldMat)));
                vec3 n = normalize(normalMat * sl_Normal);
                vec3 t = normalize(normalMat * sl_Tangent);
                t = normalize(t - dot(t, n) * n);
                vec3 b = cross(n, t);
                mat3 tbn = transpose(mat3(t, b, n)); // transpose == inverse for orthogonal matrices

                tangentPos = tbn * vec3(worldMat * vec4(sl_Position, 1.0));
                tangentCamPos = tbn * #uniforms:camPos#;
                tangentLightDir = tbn * #uniforms:lightDir#;
            }
        ]]
    },

    fragment = {
        samplers = {
            colorMap = "sampler2D",
            normalMap = "sampler2D",
            shadowMap = "sampler2D"
        },

        outputs = {
            fragColor = { type = "vec4", target = 0 }
        },

        code = [[
            float sampleShadow(vec4 coords, vec2 offset)
            {
                float shadow = 1.0;
                float dist = texture(shadowMap, coords.st + offset).r;
                if (dist < coords.z - 0.00002)
                    shadow = 0;
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
                vec3 n = normalize(texture(normalMap, uv).rgb * 2 - 1);
                vec3 color = texture(colorMap, uv).rgb;
                
                // ambient
                vec3 ambient = 0.5 * color;

                // shadow
                float shadow = samplePCF(shadowCoord / shadowCoord.w);

                // diffuse
                vec3 lightDir = normalize(-tangentLightDir);
                vec3 diffuse = min(max(dot(lightDir, n), 0.0), shadow) * color * 1.2;

                // specular
                vec3 viewDir = normalize(tangentCamPos - tangentPos);
                vec3 reflectDir = reflect(-lightDir, n);
                vec3 halfwayDir = normalize(lightDir + viewDir);  
                vec3 specular = vec3(0.5) * min(pow(max(dot(n, halfwayDir), 0.0), 32.0), shadow);

                fragColor = vec4(ambient + diffuse + specular, 1);
            }
        ]]
    }
}