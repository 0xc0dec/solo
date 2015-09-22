print("Unit tests")

function printTable(t)
	for k, v in pairs(t) do
		print(k, v)
	end
end

printTable(solo.Engine.___class)

local engine = solo.engine
print(engine.test)

print("Done")
