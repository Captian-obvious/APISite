local https = game:GetService("HttpService")
local base_url = 'https://api.sdd2.io/InsertCloudAPI/assets/v1/'
cloud = {}

function cloud:JSON(t,data)
    if t:lower()=='encode' then
        return https:JSONEncode(data)
    elseif t:lower()=='decode'
        return https:JSONDecode(data)
    end
end

function cloud:Decompile(url)
    
end

function cloud:CompileAsset(asset,parent)
    if asset~=nil then
        local model = Instance.new('Model',parent)
        model.Name=''
        asset.Parent = model
    end
end

function cloud:LoadAsset(id)
    local pre_asset = cloud:Decompile()
    local final_asset = nil
    if pre_asset~=nil then
        final_asset = cloud:CompileAsset(pre_asset)
    else
        
    end
    if final_asset~=nil then
        return final_asset
    end
end

return cloud
