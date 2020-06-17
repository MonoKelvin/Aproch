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
    new MonoList({
        ele: '#aproch-home-template',
    });

    $('.window-minimize').on('onclick', () => {
        console.log('window-minimize');
    });
    $('.window-restore').on('onclick', () => {
        console.log('window-restore');
    });
    $('.window-maximize').on('onclick', () => {
        console.log('window-maximize');
        window.close();
    });

    /** 初始化特殊属性的元素 */
    $('[a-cloak]').removeAttr('a-cloak');
    $('[a-invisible]').removeAttr('a-invisible');
    $('[a-min-size]').each(function () {
        var t = $(this);
        t.css({ 'min-width': t.css('width'), 'min-height': t.css('height') });
    });

    /** 扩展方法 */
    String.prototype.realTextWidth = function (font) {
        var currentObj = $('<pre>').hide().appendTo(document.body);
        $(currentObj.html()).css('font', font);
        var width = currentObj.width();
        currentObj.remove();
        return width;
    };
    String.prototype.compareWith = function (other, caseSensitive = true) {
        if (caseSensitive) {
            return this == other;
        }
        return this.toLowerCase() == other.toLowerCase();
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
