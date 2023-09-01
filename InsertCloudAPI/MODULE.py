asseturl='https://assetdelivery.roblox.com/v1/asset/?id='

class cloud:
    def loadAsset(id):
        theasset = asseturl+id
        if (theasset!=None):
            return encode(theasset)
        ##endif
    ##end
##}
