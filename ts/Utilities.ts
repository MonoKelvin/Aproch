export function getUUID() {
    let s: string[] = [];
    let hexDigits = '0123456789abcdef';
    for (let i = 0; i < 36; i++) {
        s[i] = hexDigits.substr(Math.floor(Math.random() * 0x10), 1);
    }
    s[14] = '4'; // bits 12-15 of the time_hi_and_version field to 0010
    s[19] = hexDigits.substr((parseInt(s[19]) & 0x3) | 0x8, 1); // bits 6-7 of the clock_seq_hi_and_reserved to 01
    s[8] = s[13] = s[18] = s[23] = '-';

    let uuid = s.join('');
    return uuid;
}

export function hexToRGB(hex: string, alpha: string) {
    var r = parseInt(hex.slice(1, 3), 16),
        g = parseInt(hex.slice(3, 5), 16),
        b = parseInt(hex.slice(5, 7), 16);
    return [r, g, b];
}

export function rgbToHex(r: number, g: number, b: number) {
    return '#' + ((1 << 24) + (r << 16) + (g << 8) + b).toString(16).slice(1).toUpperCase();
}

export function clearEventBubble(evt: Event) {
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
 * @param  {class} className
 * @return 类名
 */
export function getClassName(className: Object) {
    if (typeof className == 'string') {
        return className;
    }
    var s = className.toString();
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
 * @param {Function} onMoving 当移动时的回调函数
 */
export function addMoveComponent(movableElement: Element, areaElement?: Element, onMoving?: Function) {
    document.onmousedown = function (evt: MouseEvent) {
        if (!isParentChild(movableElement, evt.target as Element)) {
            return;
        }
        const startEvtX = evt.pageX; // 鼠标点击时起始x点
        const startEvtY = evt.pageY; // 鼠标点击时起始y点
        const $mvEle = $(movableElement);
        const left = parseInt($mvEle.css('left')); // 元素起始x
        const top = parseInt($mvEle.css('top')); // 元素起始y

        $(document).on('mousemove', (e: MouseEvent) => {
            const newLeft = left + e.pageX - startEvtX;
            const newTop = top + e.pageY - startEvtY;

            if (areaElement) {
                const $aEle = $(areaElement);
                let l = parseInt($aEle.css('left'));
                let t = parseInt($aEle.css('top'));
                l = isNaN(l) ? 0 : l;
                t = isNaN(t) ? 0 : t;

                const bRL = newLeft + ($mvEle.innerWidth() as number) > l + ($aEle.innerWidth() as number); // right limit
                const bBL = newTop + ($mvEle.innerHeight() as number) > t + ($aEle.innerHeight() as number); // bottom limit

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
            if (onMoving) {
                onMoving();
            }
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
 * 为元素附加尺寸变换组件。重置尺寸指示器默认在元素的右下角，暂无法更改其他位置。
 * @param {Element} ele 顶层元素
 * @param {Function | null} onResize 当尺寸改变时的回调函数
 * @param {Boolean} wEffect 宽度影响，是否可以改变宽度
 * @param {Boolean} hEffect 高度影响，是否可以改变高度
 */
export function addResizeComponent(ele: Element, onResize?: Function, wEffect = true, hEffect = false) {
    // 调整尺寸部分
    if (!wEffect && !hEffect) {
        return;
    }

    const t = $(ele);
    if (t.length == 0 || t.children('.resize-indicator').length > 0) {
        return;
    }

    const rs = $('<span class="resize-indicator"></span>');
    t.append(rs);
    rs.on('mousedown', function (e: MouseEvent) {
        const old_width = t.width() as number;
        const old_height = t.height() as number;
        const old_x = e.clientX;
        const old_y = e.clientY;
        $(document).on('mousemove', function (e: MouseEvent) {
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

/**
 * 是否是父子关系
 * @param {Element} parent 父元素
 * @param {Element} child 子元素
 * @returns {Boolean} 如果找到就返回true，否则返回false
 */
export function isParentChild(parent: Element, child: Element | null) {
    let curElement = child;
    while ((curElement as any) != document) {
        if (curElement == parent) {
            return true;
        }
        curElement = curElement?.parentNode as Element;
    }

    return false;
}

/**
 * 获取子元素指定css类名的所有元素数组
 * @param parentElement 父元素
 * @param className 要获取的子元素的类型
 */
export function getElementsByClassName<T>(parentElement: Element, className: string) {
    const s = document.getElementsByClassName(className);
    let result: T[] = [];
    for (let i = 0; i < s.length; i++) {
        result.push((s[i] as any) as T);
    }
    return result;
}

/**
 * 设置文本溢出时的提示内容
 * @param container 外层容器
 * @param text 文本
 * @note 当未溢出时就不会设置外层容器的title属性。
 */
export function setOverflowTooltip(container: HTMLElement, text: HTMLElement): void {
    // 容器真实可用宽度
    const ctWidth = container.offsetWidth - text.offsetLeft - parseInt(container.style.paddingRight);

    // 如果溢出
    text.offsetWidth >= ctWidth && container.setAttribute('title', text.innerText);
}
