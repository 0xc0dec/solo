--
-- Copyright (c) Aleksey Fedotov
-- MIT license
-- 

return {
    generateEffectSource = function(desc, deviceMode)
        local vulkan = deviceMode == sl.DeviceMode.Vulkan

        function generateAttributes(desc, location, typeStr)
            local all = {}
            for name, type in pairs(desc or {}) do
                local s = vulkan
                    and string.format("layout (location = %d) %s %s %s;", location, typeStr, type, name)
                    or string.format("%s %s %s;", typeStr, type, name)
                all[#all + 1] = s
                location = location + 1
            end
            return table.concat(all, "\n")
        end
        
        function generateSamplers(desc, binding)
            local all = {}
            for name, type in pairs(desc or {}) do
                local s = vulkan
                    and string.format("layout (binding = %d) uniform %s %s;", binding, type, name)
                    or string.format("uniform %s %s;", type, name)
                all[#all + 1] = s
                binding = binding + 1
            end
            return table.concat(all, "\n")
        end
        
        function generateBuffer(name, desc, binding)
            local result = vulkan and string.format("layout (binding = %d) uniform _%s {\n", binding, name) or ""
        
            for varName, varType in pairs(desc or {}) do
                local prefix = (not vulkan) and "uniform " or ""
                local newVarName = vulkan and varName or string.format("%s_%s", name, varName)
                local varStr = string.format("%s%s %s;\n", prefix, varType, newVarName)
                result = result .. varStr
            end
        
            if vulkan then
                result = string.format("%s} %s;", result, name)
            end
        
            return result .. "\n"
        end
        
        function generateBuffers(desc, binding)
            local all = {}
            local count = 0
            for name, desc in pairs(desc or {}) do
                all[#all + 1] = generateBuffer(name, desc, binding)
                binding = binding + 1
                count = count + 1
            end
            return table.concat(all, "\n"), count
        end
        
        function generateEntry(raw)
            raw = string.gsub(raw, "#([_0-9a-zA-Z]+):([_0-9a-zA-Z]+)#", function(buffer, uniform)
                return vulkan
                    and string.format("%s.%s", buffer, uniform)
                    or string.format("%s_%s", buffer, uniform)
            end)

            return string.gsub(raw, "SL_FIX_UV#([_0-9a-zA-Z]+)#", function(varName)
                return vulkan and string.format("%s.y = 1 - %s.y", varName, varName) or ""
            end)
        end

        local versionAttr = vulkan and "#version 450" or "#version 330"
        
        local vsUniformBuffers, vsUniformBufferCount = generateBuffers(desc.vertex.uniformBuffers, 0)
        local vsInputs = generateAttributes(desc.vertex.inputs, 0, "in")
        local vsOutputs = generateAttributes(desc.vertex.outputs, 0, "out")
        local vsEntry = generateEntry(desc.vertex.entry)
        local vsInvertY = vulkan and "gl_Position.y = -gl_Position.y;" or ""

        local fsUniformBuffers, fsUniformBufferCount = generateBuffers(desc.fragment.uniformBuffers, vsUniformBufferCount)
        local fsSamplers = generateSamplers(desc.fragment.samplers, vsUniformBufferCount + fsUniformBufferCount)
        local fsInputs = generateAttributes(desc.vertex.outputs, 0, "in")
        local fsOutputs = generateAttributes(desc.fragment.outputs, 0, "out")
        local fsEntry = generateEntry(desc.fragment.entry)

        return string.format([[
            // VERTEX
            %s
            %s
            %s
            %s
            void main() {
                %s
                %s
            }

            // FRAGMENT
            %s
            %s
            %s
            %s
            %s
            void main() {
                %s
            }        
        ]],
            versionAttr, vsUniformBuffers, vsInputs, vsOutputs, vsEntry, vsInvertY,
            versionAttr, fsUniformBuffers, fsSamplers, fsInputs, fsOutputs, fsEntry
        )
    end
}