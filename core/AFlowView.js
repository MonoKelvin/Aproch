let flowView = document.querySelector('#flow-view');
flowView.onmousedown = function() {
    flowView.style.cursor = 'grabbing';
};
flowView.onmouseup = function() {
    flowView.style.cursor = 'grab';
};