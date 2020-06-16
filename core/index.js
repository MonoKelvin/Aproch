import MonoList from './Component/MonoList.js';

$(document).ready(function () {
    // 防止按下刷新键
    window.onkeydown = function (e) {
        var code = e.keyCode || e.which;
        if (code == 116 || (code == 82 && (e.metaKey || e.ctrlKey))) {
            return false;
        }
    };

    /** 初始化组件 */
    new MonoList({
        ele: '#aproch-nav',
        currentIndex: 1,
    });
    new MonoList({
        ele: '#aproch-home-sidebar-proplist',
        currentIndex: 0,
    });

    /** 初始化特殊属性的元素 */
    $('[a-cloak]').removeAttr('a-cloak');
    $('[a-invisible]').removeAttr('a-invisible');
    $('[a-min-size]').each(function () {
        var t = $(this);
        t.css({ 'min-width': t.css('width'), 'min-height': t.css('height') });
    });

    // (function () {
    //     $('#aproch-nav');

    // })();

    /** 扩展方法 */
    String.prototype.realTextWidth = function (font) {
        var currentObj = $('<pre>').hide().appendTo(document.body);
        $(currentObj.html()).css('font', font);
        var width = currentObj.width();
        currentObj.remove();
        return width;
    };
    Array.prototype.remove = function (element) {
        var index = this.indexOf(element);
        if (index > -1) {
            this.splice(index, 1);
            return true;
        }
        return false;
    };
});
