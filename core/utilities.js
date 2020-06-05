export function getUUID() {
    let s = [];
    let hexDigits = '0123456789abcdef';
    for (let i = 0; i < 36; i++) {
        s[i] = hexDigits.substr(Math.floor(Math.random() * 0x10), 1);
    }
    s[14] = '4'; // bits 12-15 of the time_hi_and_version field to 0010
    s[19] = hexDigits.substr((s[19] & 0x3) | 0x8, 1); // bits 6-7 of the clock_seq_hi_and_reserved to 01
    s[8] = s[13] = s[18] = s[23] = '-';

    let uuid = s.join('');
    return uuid;
}

export function hexToRGB(hex, alpha) {
    var r = parseInt(hex.slice(1, 3), 16),
        g = parseInt(hex.slice(3, 5), 16),
        b = parseInt(hex.slice(5, 7), 16);
    return [r, g, b];
}

export function rgbToHex(r, g, b) {
    if (r === undefined) {
        return false;
    }
    return '#' + ((1 << 24) + (r << 16) + (g << 8) + b).toString(16).slice(1).toUpperCase();
}

export function clearEventBubble(evt) {
    if (evt.stopPropagation) {
        evt.stopPropagation();
    } else {
        evt.cancelBubble = true;
    }

    if (evt.preventDefault) {
        evt.preventDefault();
    } else {
        evt.returnValue = false;
    }
}

/**
 * 通过给定的类（不是字符串，直接写类名）解析类名（给定类名则直接返回）
 * @param  {class} class对象
 * @return 类名
 */
export function getClassName(clsName) {
    if (typeof clsName == 'string') {
        return clsName;
    }
    var s = clsName.toString();
    if (s.indexOf('function') == -1) {
        return null;
    } else {
        s = s.replace('function', '');
        var idx = s.indexOf('(');
        s = s.substring(0, idx);
        s = s.replace(' ', '');
    }
    return s;
}

/**
 * 为元素添加可移动组件
 * @param {Element} movableElement 可移动的组件js对象
 * @param {Element} areaElement 限制区域元素。指定该值后，移动元素不会超出这个区域内
 */
export function attachMoveComponent(movableElement, areaElement) {
    document.onmousedown = function (evt) {
        if (!findParentFromChild(movableElement, evt.target)) {
            return;
        }
        const startEvtX = evt.clientX; // 鼠标点击时起始x点
        const startEvtY = evt.clientY; // 鼠标点击时起始y点
        const $mvEle = $(movableElement);
        const left = parseInt($mvEle.css('left')); // 元素起始x
        const top = parseInt($mvEle.css('top')); // 元素起始y

        $(document).on('mousemove', (e) => {
            const newLeft = left + e.pageX - startEvtX;
            const newTop = top + e.pageY - startEvtY;

            if (areaElement) {
                const $aEle = $(areaElement);
                let l = parseInt($aEle.css('left'));
                let t = parseInt($aEle.css('top'));
                l = isNaN(l) ? 0 : l;
                t = isNaN(t) ? 0 : t;

                const bRL = newLeft + $mvEle.innerWidth() > l + $aEle.innerWidth(); // right limit
                const bBL = newTop + $mvEle.innerHeight() > t + $aEle.innerHeight(); // bottom limit

                // 紧贴左右两侧，可以上下移动
                if (newLeft < l || bRL) {
                    if (newTop < t || bBL) {
                        return;
                    } else {
                        $mvEle.css('top', newTop + 'px');
                    }
                    return;
                }

                // 紧贴上下两侧，可以左右移动
                if (newTop < t || bBL) {
                    if (newLeft < l || bRL) {
                        return;
                    } else {
                        $mvEle.css('left', newLeft + 'px');
                    }
                    return;
                }
            }
            $mvEle.css('top', newTop + 'px');
            $mvEle.css('left', newLeft + 'px');
        });
        $(document).on('mouseup', () => {
            $(document).off('mousemove');
            $(document).off('mousedown');
            $(document).off('mouseup');
        });
        return;
    };
}

/**
 * 从子元素找到父元素
 * @param {Element} parent 父元素
 * @param {Element} child 子元素
 * @returns {Boolean} 如果找到就返回true，否则返回false
 */
export function findParentFromChild(parent, child) {
    let curElement = child;
    while (curElement != document) {
        if (curElement == parent) {
            return true;
        }
        curElement = curElement.parentNode;
    }

    return false;
}
