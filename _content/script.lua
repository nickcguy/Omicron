
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

function Event_BeginPlay(delta, world, entity)
    local sys = System.new();
    sys:PrintMessage(entity:GetTagString())

    local engine = Engine.new("CoreEngine");

    local hands = engine:GetTaggedCount("ovrHand");
    sys:PrintMessage("ovrHand: "..tostring(hands));

    local compAcc = ComponentAccessor.new(entity);
    local mtl = compAcc:GetMaterial();
    if mtl then
        sys:PrintMessage(mtl:Get())
    else
        sys:PrintMessage("No material in entity");
    end

end

function Event_Tick(delta, world, entity)

end


Exports = {
    Event_BeginPlay = Event_BeginPlay,
    Event_Tick = Event_Tick
};