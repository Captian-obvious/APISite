import requests
import json
import luaparser
from luaparser import ast

class insertserver:
    def downloadAsset(id):
        url = 'https://assetdelivery.roblox.com/v1/asset/?id='+str(id)      
        REQUEST = requests.get(url)
        if (REQUEST.status_code<400):
            rawData = REQUEST.content
            asset = open(str(id)+".rbxm", "x")
            asset.write(str(rawData))
            posted = {'Status_Code': REQUEST.status_code}
            return asset.read()
        else:
            print('REQUEST_ERROR: '+str(REQUEST.status_code))
        ##endif
    ##end
    def downloadAudio(id):
        url = 'https://api.hyra.io/audio/'+str(id)
        REQUEST = requests.get(url)
        if (REQUEST.status_code<400):
            asset = REQUEST.content
            return asset
        ##endif
    ##end
##end
