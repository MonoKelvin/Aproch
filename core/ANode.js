class ANode {
    /**
     * 创建一个新的节点
     * @param {object} canvasElement 要把节点放入的画布场景元素
     * @param {string} idName 指定html中节点的唯一id
     * @param {string} titleName 节点标题名
     * @param {number} x 节点的 x 坐标
     * @param {number} y 节点的 y 坐标
     * @param {string} titleColor 节点头部颜色
     */
    constructor(canvasElement, idName, titleName, x = 0, y = 0, titleColor = '#3f3f3f') {
        if (canvasElement === undefined) {
            throw 'canvasElement不允许为空!';
        }

        /** UUID，标识每一个节点 */
        this.uuid = getUUID();
        console.log(this.uuid);

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
        this.nodeWidget.append(this.nodeTitle);
        this.nodeWidget.append(this.nodeContent);
        this.nodeWidget.innerHTML += '<span class="node-resize-indicator"></span>';
        canvasElement.appendChild(this.nodeWidget);

        /* 设置位置 */
        this.nodeWidget.style.left = x + 'px';
        this.nodeWidget.style.top = y + 'px';

        /* 注册为可移动节点 */
        $('#' + idName).bg_move({
            move: '.node-title'
        });
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

    /** 获得节点在场景中的位置
     * @returns {array} {x,y}
     */
    getPosition() {
        return {
            x: this.nodeWidget.style.left,
            y: this.nodeWidget.style.top
        };
    }
}

function addNode() {
    let node = new ANode(
        document.querySelector('#flow-view'),
        '_' + parseInt(Math.random() * 10000000),
        'NEW NODE',
        300,
        150
    );
}