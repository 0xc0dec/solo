/*
    Copyright (c) Aleksey Fedotov
    MIT license
*/

#include "SoloLuaScriptRuntime.h"
#include "SoloDevice.h"
#include "SoloLuaCommon.h"

using namespace solo;

void registerEnums(CppBindModule<LuaBinding> &module);
void registerMathApi(CppBindModule<LuaBinding> &module);
void registerNodeAndComponentApi(CppBindModule<LuaBinding> &module);
void registerTransformApi(CppBindModule<LuaBinding> &module);
void registerCameraApi(CppBindModule<LuaBinding> &module);
void registerTextureApi(CppBindModule<LuaBinding> &module);
void registerMaterialApi(CppBindModule<LuaBinding> &module);
void registerMiscApi(CppBindModule<LuaBinding> &module);
void registerDeviceApi(CppBindModule<LuaBinding> &module);
void registerPhysicsApi(CppBindModule<LuaBinding> &module);
void registerMeshApi(CppBindModule<LuaBinding> &module);
void registerFontApi(CppBindModule<LuaBinding> &module);

static void registerApi(CppBindModule<LuaBinding> &module)
{
    registerEnums(module);
    registerMathApi(module);
    registerNodeAndComponentApi(module);
    registerTransformApi(module);
    registerCameraApi(module);
    registerTextureApi(module);
    registerMaterialApi(module);
    registerMiscApi(module);
    registerDeviceApi(module);
    registerPhysicsApi(module);
    registerMeshApi(module);
    registerFontApi(module);
}

static void registerLibrary(LuaState &state)
{
    state.doString(R"(
        sl.getCmpId = function(name)
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
            shape.typeId = sl.getCmpId(id)
            return shape
        end

		sl.generateEffectSource = function(desc)
			local vulkan = sl.device:getMode() == sl.DeviceMode.Vulkan

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

				raw = string.gsub(raw, "FIX_Y#([_0-9a-zA-Z]+)#", function(varName)
					return vulkan and string.format("%s.y = -%s.y", varName, varName) or ""
				end)

				return string.gsub(raw, "FIX_UV#([_0-9a-zA-Z]+)#", function(varName)
					return vulkan and string.format("%s.y = 1 - %s.y", varName, varName) or ""
				end)
			end

			local versionAttr = vulkan and "#version 450" or "#version 330"
        
			local vsUniformBuffers, vsUniformBufferCount = generateBuffers(desc.vertex.uniformBuffers, 0)
			local vsInputs = generateAttributes(desc.vertex.inputs, 0, "in")
			local vsOutputs = generateAttributes(desc.vertex.outputs, 0, "out")
			local vsEntry = generateEntry(desc.vertex.entry)

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
				versionAttr, vsUniformBuffers, vsInputs, vsOutputs, vsEntry,
				versionAttr, fsUniformBuffers, fsSamplers, fsInputs, fsOutputs, fsEntry
			)
		end
    )");
}

LuaScriptRuntime::LuaScriptRuntime()
{
    lua = LuaState::newState();
    lua.openLibs();

    auto module = LuaBinding(lua).beginModule("sl");
    registerApi(module);
    registerLibrary(lua);
    module.endModule();
}

LuaScriptRuntime::LuaScriptRuntime(Device *device):
    LuaScriptRuntime()
{
    auto module = LuaBinding(lua).beginModule("sl");
    module.addConstant("device", device);
    module.endModule();
}

LuaScriptRuntime::~LuaScriptRuntime()
{
    lua.close();
}

void LuaScriptRuntime::executeFile(const str& path)
{
    if (lua.loadFile(path.c_str()))
    {
	    const auto msg = lua.getString(-1);
        panic(SL_FMT("Script failed to load: ", msg));
    }

    // TODO use FileSystem to read the file
    lua.doFile(path.c_str());
}

auto LuaScriptRuntime::eval(const str& code) -> str
{
	return lua.eval<str>(code.c_str());
}

auto LuaScriptRuntime::getString(const str& name) -> str
{
    return LuaRef(lua, name.c_str()).toValue<str>();
}

auto LuaScriptRuntime::getDeviceSetup(const str &name) -> DeviceSetup
{
    return LuaRef(lua, name.c_str()).toValue<DeviceSetup>();
}
