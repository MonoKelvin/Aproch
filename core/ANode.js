import { getUUID } from './utilities.js';

export class ANode {
    /**
     * 创建一个新的节点
     * @param {object} canvas 要把节点放入的画布场景
     * @param {string} idName 指定html中节点的唯一id
     * @param {string} titleName 节点标题名
     * @param {string} titleColor 节点头部颜色
     * @param {number} x 节点的 x 坐标
     * @param {number} y 节点的 y 坐标
     */
    constructor(canvas, idName, titleName, titleColor = '#3f3f3f', x = 0, y = 0) {
        /** UUID，标识每一个节点 */
        this.uuid = getUUID();

        /** 节点的 x 坐标 */
        this.x = x;

        /** 节点的 y 坐标 */
        this.y = y;

        /** 节点控件元素 */
        this.nodeWidget = document.createElement('div');

        /** 节点头部元素 */
        this.nodeTitle = document.createElement('div');

        /** 节点内容，放置所有接口的容器 */
        this.nodeContent = document.createElement('div');

        /* 初始化节点控件 */

        this.nodeWidget.setAttribute('class', 'node-widget');
        this.nodeWidget.id = idName;
        this.nodeTitle.setAttribute('class', 'node-title');
        this.nodeContent.setAttribute('class', 'node-content');

        this.nodeTitle.innerHTML = titleName;
        this.nodeTitle.style.background = titleColor;

        this.nodeWidget.append(nodeTitle);
        this.nodeWidget.append(nodeContent);
        this.nodeWidget.append('<span class="node-resize-indicator"></span>');

        canvas.append(this.nodeWidget);
    }

    /**
     * 设置节点头部颜色
     * @param {string} color 节点头部的颜色，
     * @example
     *  setTitleColor("#123456");
     *  setTitleColor("rgb(255,255,255)")
     * @warning 尽量不要设置透明alpha通道
     */
    setTitleColor(color) {
        this.nodeTitle.style.background = color;
    }
}