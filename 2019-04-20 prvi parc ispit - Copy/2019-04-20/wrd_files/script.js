var elementi= document.getElementsByClassName('VilaKolonaOkvir');

for(var i=0; i<elementi.length; i++) {

    elementi[i].onmouseover = hoveruj;
    elementi[i].onmouseout  = iskljuciHover;
}

function hoveruj() {
   this.style.border = '3px solid yellow';
}

function iskljuciHover() {
    this.style.border = 'none';
}
$("#IzbornikDugme").on('click', function() {
    $("#Izbornik").toggleClass('vidljivo');
});
