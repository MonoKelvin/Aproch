import ANodeModelRegistry from './NodeEditor/ANodeModelRegistry.js';
import MonoLabel from './Component/MonoLabel.js';
import MonoInputWidget from './Component/MonoInputWidget.js';

$(document).ready(function () {
    // 防止按下(F5、Ctrl+R、Ctrl+Shift+R)刷新
    window.onkeydown = function (e) {
        var ev = window.event || e;
        var code = ev.keyCode || ev.which;
        if (code == 116 || (code == 82 && (ev.metaKey || ev.ctrlKey))) {
            return false;
        }
    };

    /** 初始化属性设置 */

    // 删除所有元素会因刷新导致闪烁的属性
    $('[a-cloak]').removeAttr('a-cloak');
    $('[a-invisible]').removeAttr('a-invisible');

    $('[a-min-size]').each(function () {
        let t = $(this);
        t.css({ 'min-width': t.css('width'), 'min-height': t.css('height') });
    });

    /** 注册元素 */
    customElements.define('mono-label', MonoLabel);
    customElements.define('mono-input-number', MonoInputWidget);
    customElements.define('a-flow-view', AFlowView);
    customElements.define('a-node', ANode);
    customElements.define('a-interface', AInterface);
    customElements.define('a-port', APort);
    customElements.define('a-connection', AConnection);

    /** 扩展方法 */

    // 获取文本实际像素宽度（返回结果不包含'px'）
    String.prototype.realTextWidth = function (font) {
        let currentObj = $('<pre>').hide().appendTo(document.body);
        $(currentObj).html(this).css('font', font);
        const width = currentObj.width();
        currentObj.remove();
        return width;
    };

    // 移除数组中的指定元素，移除失败返回false，否则返回true
    Array.prototype.remove = function (element) {
        const index = this.indexOf(element);
        if (index > -1) {
            this.splice(index, 1);
            return true;
        }

        return false;
    };

    // 创建系统默认的节点表
    AFlowView.NodeTable = new ANodeModelRegistry();
    AFlowView.NodeTable.registryDataModel('OutDataModel', 'MyCategory');
    AFlowView.NodeTable.registryDataModel('InDataModel', 'MyCategory');
});
