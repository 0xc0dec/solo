/*
 * Copyright (c) Aleksey Fedotov
 * MIT license
 */

#include "SoloLuaCommon.h"

void registerLibrary(LuaState &state) {
    state.doString(R"(
        sl.includeDir = function(path)
            package.path = path .. "/?.lua;" .. package.path
        end

        sl.cmpId = function(name)
            sl.__nextCmpId = sl.__nextCmpId and sl.__nextCmpId or 1
            sl.__cmpIds = sl.__cmpIds and sl.__cmpIds or {}
    
            local id = sl.__cmpIds[name]
            if not id then
                id = sl.__nextCmpId
                sl.__cmpIds[name] = id
                sl.__nextCmpId = sl.__nextCmpId + 1
            end

            return id
        end

        sl.createComponent = function(id, shape)
            shape.typeId = sl.cmpId(id)
            return shape
        end

        sl.generateEffectSource = function(desc)
            local vulkan = sl.device:mode() == sl.DeviceMode.Vulkan -- otherwise 330

            function generateAttributes(desc, typeStr)
                local all = {}
                local location = 0
                for name, type in pairs(desc or {}) do
                    local s = vulkan
                        and string.format("layout (location = %d) %s %s %s;", location, typeStr, type, name)
                        or string.format("%s %s %s;", typeStr, type, name)
                    all[#all + 1] = s
                    location = location + 1
                end
                return table.concat(all, "\n")
            end

            function generateFsOutputs(desc)
                local all = {}
                for name, info in pairs(desc or {}) do
                    local t = info.type
                    local loc = info.target
                    local s = string.format("layout (location = %d) out %s %s;", loc, t, name)
                    all[#all + 1] = s
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
        
            function generateCode(raw)
                raw = string.gsub(raw, "#([_0-9a-zA-Z]+):([_0-9a-zA-Z]+)#", function(buffer, uniform)
                    return vulkan
                        and string.format("%s.%s", buffer, uniform)
                        or string.format("%s_%s", buffer, uniform)
                end)

                raw = string.gsub(raw, "SL_FIX_Y#([_0-9a-zA-Z]+)#", function(varName)
                    return vulkan and string.format("%s.y = -%s.y", varName, varName) or ""
                end)

                raw = string.gsub(raw, "SL_SHADOW_BIAS_MAT", function(varName)
                    return vulkan and [[
                        mat4(
                            0.5, 0.0, 0.0, 0.0,
                            0.0, 0.5, 0.0, 0.0,
                            0.0, 0.0, 1.0, 0.0,
                            0.5, 0.5, 0.0, 1.0)
                    ]] or [[
                        mat4(
                            0.5, 0.0, 0.0, 0.0,
                            0.0, 0.5, 0.0, 0.0,
                            0.0, 0.0, 0.5, 0.0,
                            0.5, 0.5, 0.5, 1.0)
                    ]]
                end)

                return string.gsub(raw, "SL_FIX_UV#([_0-9a-zA-Z]+)#", function(varName)
                    return vulkan and string.format("%s.y = 1 - %s.y", varName, varName) or ""
                end)
            end

            local versionAttr = vulkan and "#version 450" or "#version 330"
        
            local vsUniformBuffers, vsUniformBufferCount = generateBuffers(desc.vertex.uniformBuffers, 0)
            local vsInputs = generateAttributes(desc.vertex.inputs, "in")
            local vsOutputs = generateAttributes(desc.vertex.outputs, "out")
            local vsCode = generateCode(desc.vertex.code)

            local fsUniformBuffers, fsUniformBufferCount = generateBuffers(desc.fragment.uniformBuffers, vsUniformBufferCount)
            local fsSamplers = generateSamplers(desc.fragment.samplers, vsUniformBufferCount + fsUniformBufferCount)
            local fsInputs = generateAttributes(desc.vertex.outputs, "in")
            local fsOutputs = generateFsOutputs(desc.fragment.outputs)
            local fsCode = generateCode(desc.fragment.code)

            local result = string.format([[
                // VERTEX
                %s
                %s
                %s
                %s
                %s

                // FRAGMENT
                %s
                %s
                %s
                %s
                %s
                %s
            ]],
                versionAttr, vsUniformBuffers, vsInputs, vsOutputs, vsCode,
                versionAttr, fsUniformBuffers, fsSamplers, fsInputs, fsOutputs, fsCode
            )

            return result
        end
    )");
}