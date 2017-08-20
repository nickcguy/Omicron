---
--- Created by Guy.
--- DateTime: 19/08/2017 18:47
---


local seen={}

function dump(t,i)
    seen[t]=true
    local s={}
    local n=0
    for k in pairs(t) do
        n=n+1 s[n]=k
    end
    table.sort(s)
    for k,v in ipairs(s) do
        print(i,v)
        v=t[v]
        if type(v)=="table" and not seen[v] then
            dump(v,i.."\t")
        end
    end
end

local function SetLight(entity, state)
    local sys = System.new();
    if not state then return end
    local accessor = ComponentAccessor.new();
    local light = accessor:GetLight(entity);
    if light == nil then
        sys:PrintMessage("Cannot find light");
    else
        light:Toggle()
    end
end

local function SetFocused(entity, state)
    local sys = System.new();
    local accessor = ComponentAccessor.new();
    local light = accessor:GetLight(entity);

    if light == nil then return end

    for key,value in pairs(getmetatable(light)) do
        sys:PrintMessage(tostring(key)..": "..tostring(value))
    end

    if light == nil then
        sys:PrintMessage("Cannot find light");
    else
        sys:PrintMessage("Light: "..tostring(light))
        if state then
            light:set_cutoff(10.0)
            light:set_outerCutoff(15.0)
            light:set_intensity(300.0)
        else
            light:set_cutoff(40.0)
            light:set_outerCutoff(50.0)
            light:set_intensity(100.0)
        end
    end
end

local function APressed(delta, engine, entity, state)
    SetLight(entity, state)
end

local function BPressed(delta, engine, entity, state)
    SetFocused(entity, state)
end

Exports = {
    ToggleLight = APressed,
    ToggleFocused = BPressed
}