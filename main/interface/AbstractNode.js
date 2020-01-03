'use strict';

function AbstructNode(title = "New Node") {
    this.title = title;
    this.titleColor = null;
};

// AbstructNode.prototype.setTitle = function(title) {
//     this.title = title;
// };

AbstructNode.prototype.createNode = function(title) {
    this.title = title;
    let html = `<div class="draggable-widget">
                    <div class="dw-title">` + title + `</div>
                    <div class="dw-content">
                        <span class="dw-resize-indicator"></span>
                    </div>
                </div>`;
};

/**
 * set title color
 * @param color title color
 * ```javascript
 * var node = new AbstructNode('New Node');
 * node.setTitleColor('#ff0000');
 * // or use fllowing code.
 * node.setTitleColor('rgb(255, 0, 0)');
 * ```
 */
AbstructNode.prototype.setTitleColoe = function(color) {
    this.titleColor = color;
    let eleTitle = document.querySelector("dw-title");
    eleTitle.setAttribute('background-color', color);
};