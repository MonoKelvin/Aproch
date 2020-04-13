import { IDataModel, OutDataModel } from '../core/DataModel.js';
import { ALabelWidget, AInputNumberWidget } from '../core/AWidget.js';
import { AFlowView, ANode, AConnection, AInterface, APort } from '../core/Aproch.js';
import ANodeModelRegistry from '../core/ANodeModelRegistry.js';

$(document).ready(function () {
    // 防止按下(F5、Ctrl+R、Ctrl+Shift+R)刷新
    window.onkeydown = function (e) {
        var ev = window.event || e;
        var code = ev.keyCode || ev.which;
        if (code == 82 && (ev.metaKey || ev.ctrlKey)) {
            return false;
        }
    };

    customElements.define('a-label-widget', ALabelWidget);
    customElements.define('a-input-number', AInputNumberWidget);
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

    // 点的加法
    Object.prototype.pointAdd = function (point) {
        let tx = 0,
            ty = 0,
            px = 0,
            py = 0;

        if (point.left !== undefined) {
            px = point.left;
            py = point.top;
        } else {
            px = point.x;
            py = point.y;
        }
        if (this.left !== undefined) {
            tx = this.left;
            ty = this.top;
        } else {
            tx = this.x;
            ty = this.y;
        }

        return {
            x: tx + px,
            y: ty + py,
        };
    };

    // 点的减法
    Object.prototype.pointSub = function (point) {
        let tx = 0,
            ty = 0,
            px = 0,
            py = 0;

        if (point.left !== undefined) {
            px = point.left;
            py = point.top;
        } else {
            px = point.x;
            py = point.y;
        }
        if (this.left !== undefined) {
            tx = this.left;
            ty = this.top;
        } else {
            tx = this.x;
            ty = this.y;
        }

        return {
            x: tx - px,
            y: ty - py,
        };
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

    //var FVManager = [];
    const CurrentFV = document.querySelector('#fv_1');
    const NodesTable = new ANodeModelRegistry();

    NodesTable.registryDataModel('OutDataModel', 'MyCategory');
    NodesTable.registryDataModel('InDataModel', 'MyCategory');
    NodesTable.createNode('OutDataModel', 'MyCategory', CurrentFV, -200, -150);
    NodesTable.createNode('OutDataModel', 'MyCategory', CurrentFV, -200, 150);
    NodesTable.createNode('InDataModel', 'MyCategory', CurrentFV, 100, -150);
    NodesTable.createNode('InDataModel', 'MyCategory', CurrentFV, 100, 150);
});
