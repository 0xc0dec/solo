--
-- Copyright (c) Aleksey Fedotov
-- MIT license
-- 

function runTest(fileName)
    local test = assert(loadfile("../../src/tests/" .. fileName))
    local _, err = select(1, pcall(test))
    if err then
        print(fileName .. ": failed\n" .. err)
    else
        print(fileName .. ": success")
    end
end

runTest("smoke/DegreeRadian.lua")
runTest("smoke/Enums.lua")
runTest("smoke/Vector2.lua")
runTest("smoke/Vector3.lua")
runTest("smoke/Vector4.lua")
runTest("smoke/Quaternion.lua")
runTest("smoke/Matrix.lua")
runTest("smoke/Ray.lua")
runTest("smoke/Device.lua")
runTest("smoke/DeviceSetup.lua")
runTest("smoke/Logger.lua")
runTest("smoke/Scene.lua")
runTest("smoke/Node.lua")
runTest("smoke/Component.lua")
runTest("smoke/Transform.lua")
runTest("smoke/Camera.lua")
runTest("smoke/FrameBuffer.lua")
runTest("smoke/Texture.lua")
runTest("smoke/MeshRenderer.lua")
runTest("smoke/Effect.lua")
runTest("smoke/Material.lua")
runTest("smoke/Physics.lua")
runTest("smoke/FileSystem.lua")
runTest("smoke/Renderer.lua")
runTest("smoke/Mesh.lua")
runTest("smoke/Spectator.lua")
runTest("smoke/SkyboxRenderer.lua")
runTest("smoke/Font.lua")
runTest("smoke/FontRenderer.lua")
runTest("smoke/Lib.lua")

print("All tests finished")

-- // VERTEX

-- #version 450

-- layout (binding = 0) uniform Matrices
-- {
--     mat4 wvp;
-- } matrices;

-- layout (location = 0) in vec3 position;
-- layout (location = 1) in vec2 texCoord0;

-- layout (location = 0) out vec2 outTexCood;

-- void main()
-- {
--     outTexCood = texCoord0;
--     outTexCood.y = 1 - outTexCood.y;
-- 	gl_Position = matrices.wvp * vec4(position.xyz, 1.0);
--     gl_Position.y = -gl_Position.y;
-- }

-- // FRAGMENT

-- #version 450

-- layout (binding = 1) uniform sampler2D mainTex;

-- layout (location = 0) in vec2 inTexCoord;

-- layout (location = 0) out vec4 outFragColor;

-- void main()
-- {
--     outFragColor = texture(mainTex, inTexCoord);
-- }

local effect = {
    vertex = {
        uniformBuffers = {
            ["matrices"] = {
                wvp = "mat4"
            }
        },

        inputs = {
            position = "vec3",
            texCoord0 = "vec2"
        },

        outputs = {
            outTexCood = "vec2"
        },

        entry = [[
            outTexCood = texCoord0;
            outTexCood.y = 1 - outTexCood.y;
            gl_Position = #matrices:wvp# * vec4(position.xyz, 1.0);
        ]]
    },

    fragment = {
        uniformBuffers = {
            ["vars"] = {
                lala = "vec3"
            }
        },

        samplers = {
            mainTex = "sampler2D"
        },

        outputs = {
            outFragColor = "vec4"
        },

        entry = [[
            outFragColor = texture(mainTex, inTexCoord);
        ]]
    }
}

function generateAttributes(vulkan, desc, location, typeStr)
    local all = {}
    for name, type in pairs(desc) do
        local s = vulkan
            and string.format("layout (location = %d) %s %s %s;", location, typeStr, type, name)
            or string.format("%s %s %s;", typeStr, type, name)
        all[#all + 1] = s
        location = location + 1
    end
    return table.concat(all, "\n")
end

function generateSamplers(vulkan, desc, binding)
    local all = {}
    for name, type in pairs(desc) do
        local s = vulkan
            and string.format("layout (binding = %d) uniform %s %s;", binding, type, name)
            or string.format("uniform %s %s;", type, name)
        all[#all + 1] = s
        binding = binding + 1
    end
    return table.concat(all, "\n")
end

function generateBuffer(vulkan, name, desc, binding)
    local result = vulkan and string.format("layout (binding = %d) uniform __%s {\n", binding, name) or ""

    for varName, varType in pairs(desc) do
        local prefix = (not vulkan) and "uniform " or ""
        local newVarName = vulkan and varName or string.format("_%s_%s", name, varName)
        local varStr = string.format("%s%s %s;\n", prefix, varType, newVarName)
        result = result .. varStr
    end

    if vulkan then
        result = string.format("%s} _%s;", result, name)
    end

    return result .. "\n"
end

function generateBuffers(vulkan, desc, binding)
    local all = {}
    local count = 0
    for name, desc in pairs(desc) do
        all[#all + 1] = generateBuffer(vulkan, name, desc, binding)
        binding = binding + 1
        count = count + 1
    end
    return table.concat(all, "\n"), count
end

function generateEntry(vulkan, raw)
    return string.gsub(raw, "#(.+):(.+)#", function(buffer, uniform)
        return vulkan
            and string.format("_%s.%s", buffer, uniform)
            or string.format("_%s_%s", buffer, uniform)
    end)
end

function generateEffectCode(device, desc)
    local vulkan = false -- device:getMode() ~= sl.DeviceMode.Vulkan
    local version = vulkan and "#version 450" or "#version 330"
    
    local vsUniformBuffers, vsUniformBufferCount = generateBuffers(vulkan, desc.vertex.uniformBuffers, 0)
    local vsInputs = generateAttributes(vulkan, desc.vertex.inputs, 0, "in")
    local vsOutputs = generateAttributes(vulkan, desc.vertex.outputs, 0, "out")
    local vsEntry = generateEntry(vulkan, desc.vertex.entry)
    local vsInvertY = vulkan and "gl_Position.y = -gl_Position.y;" or ""

    local fsUniformBuffers, fsUniformBufferCount = generateBuffers(vulkan, desc.fragment.uniformBuffers, vsUniformBufferCount)
    local fsSamplers = generateSamplers(vulkan, desc.fragment.samplers, vsUniformBufferCount + fsUniformBufferCount)
    local fsInputs = generateAttributes(vulkan, desc.vertex.outputs, 0, "in")
    local fsOutputs = generateAttributes(vulkan, desc.fragment.outputs, 0, "out")
    local fsEntry = generateEntry(vulkan, desc.fragment.entry)

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
        version, vsUniformBuffers, vsInputs, vsOutputs, vsEntry, vsInvertY,
        version, fsUniformBuffers, fsSamplers, fsInputs, fsOutputs, fsEntry
    )
end

print(generateEffectCode(sl.device, effect))