"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
exports.getElementsByClassName = exports.findParentFromChild = exports.addResizeComponent = exports.addMoveComponent = exports.getClassName = exports.clearEventBubble = exports.rgbToHex = exports.hexToRGB = exports.getUUID = void 0;
function getUUID() {
    var s = [];
    var hexDigits = '0123456789abcdef';
    for (var i = 0; i < 36; i++) {
        s[i] = hexDigits.substr(Math.floor(Math.random() * 0x10), 1);
    }
    s[14] = '4';
    s[19] = hexDigits.substr((s[19] & 0x3) | 0x8, 1);
    s[8] = s[13] = s[18] = s[23] = '-';
    var uuid = s.join('');
    return uuid;
}
exports.getUUID = getUUID;
function hexToRGB(hex, alpha) {
    var r = parseInt(hex.slice(1, 3), 16), g = parseInt(hex.slice(3, 5), 16), b = parseInt(hex.slice(5, 7), 16);
    return [r, g, b];
}
exports.hexToRGB = hexToRGB;
function rgbToHex(r, g, b) {
    if (r === undefined) {
        return false;
    }
    return '#' + ((1 << 24) + (r << 16) + (g << 8) + b).toString(16).slice(1).toUpperCase();
}
exports.rgbToHex = rgbToHex;
function clearEventBubble(evt) {
    if (evt.stopPropagation) {
        evt.stopPropagation();
    }
    else {
        evt.cancelBubble = true;
    }
    if (evt.preventDefault) {
        evt.preventDefault();
    }
    else {
        evt.returnValue = false;
    }
}
exports.clearEventBubble = clearEventBubble;
function getClassName(clsName) {
    if (typeof clsName == 'string') {
        return clsName;
    }
    var s = clsName.toString();
    if (s.indexOf('function') == -1) {
        return null;
    }
    else {
        s = s.replace('function', '');
        var idx = s.indexOf('(');
        s = s.substring(0, idx);
        s = s.replace(' ', '');
    }
    return s;
}
exports.getClassName = getClassName;
function addMoveComponent(movableElement, areaElement, onMoving) {
    if (onMoving === void 0) { onMoving = null; }
    document.onmousedown = function (evt) {
        if (!findParentFromChild(movableElement, evt.target)) {
            return;
        }
        var startEvtX = evt.pageX;
        var startEvtY = evt.pageY;
        var $mvEle = $(movableElement);
        var left = parseInt($mvEle.css('left'));
        var top = parseInt($mvEle.css('top'));
        $(document).on('mousemove', function (e) {
            var newLeft = left + e.pageX - startEvtX;
            var newTop = top + e.pageY - startEvtY;
            if (areaElement) {
                var $aEle = $(areaElement);
                var l = parseInt($aEle.css('left'));
                var t = parseInt($aEle.css('top'));
                l = isNaN(l) ? 0 : l;
                t = isNaN(t) ? 0 : t;
                var bRL = newLeft + $mvEle.innerWidth() > l + $aEle.innerWidth();
                var bBL = newTop + $mvEle.innerHeight() > t + $aEle.innerHeight();
                if (newLeft < l || bRL) {
                    if (newTop < t || bBL) {
                        return;
                    }
                    else {
                        $mvEle.css('top', newTop + 'px');
                    }
                    return;
                }
                if (newTop < t || bBL) {
                    if (newLeft < l || bRL) {
                        return;
                    }
                    else {
                        $mvEle.css('left', newLeft + 'px');
                    }
                    return;
                }
            }
            $mvEle.css('top', newTop + 'px');
            $mvEle.css('left', newLeft + 'px');
            if (onMoving) {
                onMoving();
            }
        });
        $(document).on('mouseup', function () {
            $(document).off('mousemove');
            $(document).off('mousedown');
            $(document).off('mouseup');
        });
        return;
    };
}
exports.addMoveComponent = addMoveComponent;
function addResizeComponent(ele, onResize, wEffect, hEffect) {
    if (onResize === void 0) { onResize = null; }
    if (wEffect === void 0) { wEffect = true; }
    if (hEffect === void 0) { hEffect = false; }
    if (!wEffect && !hEffect) {
        return;
    }
    var t = $(ele);
    if (t.length == 0 || t.children('.resize-indicator').length > 0) {
        return;
    }
    var rs = $('<span class="resize-indicator"></span>');
    t.append(rs);
    rs.on('mousedown', function (e) {
        var old_width = t.width();
        var old_height = t.height();
        var old_x = e.clientX;
        var old_y = e.clientY;
        $(document).on('mousemove', function (e) {
            if (wEffect) {
                t.width(e.clientX - old_x + old_width);
            }
            if (hEffect) {
                t.height(e.clientY - old_y + old_height);
            }
            if (onResize) {
                onResize();
            }
            return false;
        });
        $(document).on('mouseup', function () {
            $(document).off('mousemove');
            $(document).off('mouseup');
        });
        return false;
    });
}
exports.addResizeComponent = addResizeComponent;
function findParentFromChild(parent, child) {
    var curElement = child;
    while (curElement != document) {
        if (curElement == parent) {
            return true;
        }
        curElement = curElement.parentNode;
    }
    return false;
}
exports.findParentFromChild = findParentFromChild;
function getElementsByClassName(element, className) {
    var s = this.getElementsByClassName(className);
    var result;
    for (var i = 0; i < s.length; i++) {
        result.push(s[i]);
    }
    return result;
}
exports.getElementsByClassName = getElementsByClassName;
