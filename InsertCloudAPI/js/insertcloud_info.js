var primaryImage = document.getElementById('normal');
var secondaryImage = document.getElementById('bigger');
primaryImage.addEventListener('click',function(){
    if (primaryImage.hidden != true) {
        primaryImage.hidden = true
        secondaryImage.hidden = false
    };
});
secondaryImage.addEventListener('click',function(){
    if (secondaryImage.hidden != true) {
        secondaryImage.hidden = true
        primaryImage.hidden = false
    };
});
