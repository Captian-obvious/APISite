local downloader = {}
--[[local rbxfile = require('github.com/robloxapi/rbxfile')
local bin = require('github.com/robloxapi/rbxfile/bin')
local json = require('github.com/robloxapi/rbxfile/json')
--]]

function downloader:DownloadAsset(id)
    local url = 'https://www.roblox.com/asset/?id='..id
end

return downloader
