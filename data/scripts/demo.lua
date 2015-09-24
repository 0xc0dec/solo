local vsBasic = [[
	#version 330 core

	layout (location = 0) in vec4 position;
	layout (location = 1) in vec3 normal;
	layout (location = 2) in vec2 uv;

	uniform mat4 worldViewProjMatrix;
	out vec2 uv0;
	out vec3 n;

	void main()
	{
		gl_Position = worldViewProjMatrix * position;
		uv0 = uv;
	}
]]

local fsColor = [[
	#version 330 core

	uniform vec4 color;

	in vec2 uv0;
	out vec4 fragColor;

	void main()
	{
		fragColor = color;
	}
]]

local fsTexture =[[
	#version 330 core

	uniform sampler2D mainTex;

	in vec2 uv0;
	out vec4 fragColor;

	void main()
	{
		fragColor = texture(mainTex, uv0);
	}
]]

local fsChecker = [[
	#version 330 core

	in vec2 uv0;
	uniform vec4 color;
	out vec4 fragColor;

	void main()
	{
		float xfloor = floor(uv0.x / 0.2);
		float yfloor = floor(uv0.y / 0.2) + 1;
		if (mod(xfloor, 2) == 0 && mod(yfloor, 2) == 0)
			fragColor = vec4(0, 0, 0, 1);
		else if (mod(xfloor, 2) > 0 && mod(yfloor, 2) > 0)
			fragColor = vec4(0, 0, 0, 1);
		else
			fragColor = color;
	}
]]

local fsBasicLighting = [[
	#version 330 core

	layout (location = 0) in vec4 position;
	layout (location = 1) in vec3 normal;
	layout (location = 2) in vec2 uv;

	uniform mat4 worldViewProjMatrix;
	uniform mat4 normalMatrix;
	out vec2 uv0;
	out vec3 n;

	void main()
	{
		gl_Position = worldViewProjMatrix * position;
		uv0 = uv;
		n = normalize((normalMatrix * vec4(normal, 1)).xyz);
	}
]]

local fsTextureWithLighting = [[
	#version 330 core

	uniform sampler2D mainTex;

	in vec2 uv0;
	in vec3 n;
	out vec4 fragColor;

	void main()
	{
		vec4 color = texture(mainTex, uv0);
		fragColor = color * dot(vec3(1, 1, 1), n) / (length(vec3(1, 1, 1)) * length(n));
	}
]]
--
callback =
{
	onDeviceCloseRequested = function()
		print("Close requested")
		return true
	end,

	onEngineStarted = function()
		device = engine:getDevice()
		scene = engine:getScene()
		local _, err = pcall(init)
		if err then
			print(err)
		end
	end
}

function print_r ( t )
    local print_r_cache={}
    local function sub_print_r(t,indent)
        if (print_r_cache[tostring(t)]) then
            print(indent.."*"..tostring(t))
        else
            print_r_cache[tostring(t)]=true
            if (type(t)=="table") then
                for pos,val in pairs(t) do
                    if (type(val)=="table") then
                        print(indent.."["..pos.."] => "..tostring(t).." {")
                        sub_print_r(val,indent..string.rep(" ",string.len(pos)+8))
                        print(indent..string.rep(" ",string.len(pos)+6).."}")
                    elseif (type(val)=="string") then
                        print(indent.."["..pos..'] => "'..val..'"')
                    else
                        print(indent.."["..pos.."] => "..tostring(val))
                    end
                end
            else
                print(indent..tostring(t))
            end
        end
    end
    if (type(t)=="table") then
        print(tostring(t).." {")
        sub_print_r(t,"  ")
        print("}")
    else
        sub_print_r(t,"  ")
    end
    print()
end

local escapeWatcher =
{
	typeId = "EscapeWatcher",

	update = function()
		if device:isKeyPressed(solo.KeyCode_Escape, true) then
			device:requestShutdown()
		end
	end
}

local spectator =
{
	typeId = "Spectator",

	init = function(self)
		-- local transform = self.node:findComponent("Transform")
		-- local t = self.node:test()
		-- print(transform:getTypeId())
		-- print(t:getTypeId())
		-- print(t:getWorldPosition().x)
	end
}

function init()
	local auxNode = scene:createNode() -- TODO local
	auxNode:addComponent(escapeWatcher)
	auxNode:addComponent(spectator)
end

engine = solo.Engine.create(solo.EngineCreationArgs(solo.EngineMode_OpenGL, 800, 600))
engine:setCallback(callback)
engine:run()
