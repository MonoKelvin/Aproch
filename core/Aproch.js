import { getUUID } from './utilities';

var EPortType = {
    INPUT: 0,
    OUTPUT: 1
};

class AFlowView extends HTMLElement {
    /** 创建一个指定name的节点编辑视图控件
     * @param {string} name 该视图的名称
     */
    constructor(name) {
        super();

        this.name = this.getAttribute('name') ? this.getAttribute('name') : name;

        /** 场景中所有的节点 */
        this.nodes = [];

        /** 场景中选择的节点 */
        this.selectedNodes = [];

        /** 注册事件 */
        this.onmousedown = function(evt) {
            let oldX = parseInt($(this).css('left'));
            let oldY = parseInt($(this).css('top'));

            // 按下shift键就多选
            if (evt.shiftKey == 1) {
                var selList = [];
                var fileNodes = document.getElementsByTagName('div');
                for (let i = 0; i < fileNodes.length; i++) {
                    if (fileNodes[i].className.indexOf('node-widget') != -1) {
                        fileNodes[i].className = 'node-widget';
                        selList.push(fileNodes[i]);
                    }
                }

                var isSelect = true;
                // var evt = window.event || arguments[0];
                var startX = evt.offsetX; //.clientX;
                var startY = evt.offsetY; //.clientY;
                console.log('evt :', evt);
                var selDiv = document.createElement('div');

                selDiv.style.cssText =
                    'position:absolute;width:0px;height:0px;font-size:0px;margin:0px;padding:0px;border:1px dashed #AAA;background-color:#333;z-index:1000;filter:alpha(opacity:60);opacity:0.6;display:none;';

                this.append(selDiv);
                selDiv.style.left = startX + 'px';
                selDiv.style.top = startY + 'px';

                var _x = null;
                var _y = null;
                clearEventBubble(evt);

                this.onmousemove = function(evt) {
                    // evt = window.event || arguments[0];

                    if (isSelect) {
                        if (selDiv.style.display == 'none') {
                            selDiv.style.display = '';
                        }
                        _x = evt.offsetX;
                        _y = evt.offsetY;
                        selDiv.style.left = Math.min(_x, startX) + 'px';
                        selDiv.style.top = Math.min(_y, startY) + 'px';
                        selDiv.style.width = Math.abs(_x - startX) + 'px';
                        selDiv.style.height = Math.abs(_y - startY) + 'px';

                        // ---------------- 关键算法 ---------------------

                        let _l = selDiv.offsetLeft,
                            _t = selDiv.offsetTop;

                        let _w = selDiv.offsetWidth,
                            _h = selDiv.offsetHeight;

                        for (let i = 0; i < selList.length; i++) {
                            let sl = selList[i].offsetWidth + selList[i].offsetLeft;
                            let st = selList[i].offsetHeight + selList[i].offsetTop;
                            if (
                                sl > _l &&
                                st > _t &&
                                selList[i].offsetLeft < _l + _w &&
                                selList[i].offsetTop < _t + _h
                            ) {
                                if (selList[i].className.indexOf('seled') == -1) {
                                    selList[i].className = selList[i].className + ' seled';
                                }
                            } else {
                                if (selList[i].className.indexOf('seled') != -1) {
                                    selList[i].className = 'node-widget';
                                }
                            }
                        }
                    }
                    clearEventBubble(evt);
                };
            } else {
                this.style.cursor = 'grabbing';
                this.onmousemove = function(em) {
                    em.preventDefault();
                    let x = em.clientX - evt.clientX;
                    let y = em.clientY - evt.clientY;
                    this.style.left = String(oldX + x) + 'px';
                    this.style.top = String(oldY + y) + 'px';
                    return false;
                };
            }

            this.onmouseup = function() {
                this.style.cursor = 'grab';
                this.onmousemove = null;

                isSelect = false;

                if (selDiv) {
                    selDiv.remove();
                    let count = 0;
                    for (let i = 0; i < selList.length; i++) {
                        if (selList[i].className.indexOf('seled') != -1) {
                            count++;
                        }
                    }
                    console.log(selList instanceof ANode);
                }

                selList = null;
                _x = null;
                _y = null;
                selDiv = null;
                startX = null;
                startY = null;
                evt = null;
            };

            return false;
        };
    }

    /**
     * 添加一个节点
     * @param {ANode} node 要加入场景的节点
     */
    addNode(node) {
        if (node === null || !(node instanceof ANode)) {
            throw '节点无效！';
        }

        this.append(node.nodeWidget);
    }

    /**
     * 删除一个节点
     * @param {ANode} node 要删除的节点
     */
    deleteNode(node) {
        if (node === null || !(node instanceof ANode)) {
            console.log('删除了一个无效的节点');
            return;
        }
        node.remove();
        node = null;
    }
}

class ANode extends HTMLElement {
    /**
     * 创建一个新的节点
     * @param {AFlowView} flowView 要把节点放入的视图
     * @param {string} idName 指定html中节点的唯一id
     * @param {string} titleName 节点标题名
     * @param {number} x 节点的 x 坐标
     * @param {number} y 节点的 y 坐标
     * @param {string} titleColor 节点头部颜色
     */
    constructor(flowView, idName, titleName, x = 0, y = 0, titleColor = '#3f3f3f') {
        super();

        flowView = document.querySelector('#' + this.getAttribute('view'));

        if (flowView === null || !(flowView instanceof AFlowView)) {
            throw 'flowView无效！：\n' + flowView;
        }

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
        this.nodeWidget.append(this.nodeTitle);
        this.nodeWidget.append(this.nodeContent);
        this.nodeWidget.innerHTML += '<span class="node-resize-indicator"></span>';

        /** 添加到视图 */
        flowView.addNode(this);

        /* 设置位置 */
        this.nodeWidget.style.left = x + 'px';
        this.nodeWidget.style.top = y + 'px';

        /* 注册为可移动节点 */
        $('#' + idName).addMoveComponent();
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

/** <aproch-interface
 *      in-port="true"
 *      out-port=""
 *      widget="input"
 *      input-type="int"
 *      max-value="100"
 *      min-value="10"
 *      d-value="5"
 * />
 *
 * @example
 * in-port 是否有输入端口： true | 声明(true) | 不声明(false);
 * out-port 是否有输出端口： true | 声明(true) | 不声明(false);
 * widget 嵌入的控件： input   |   selection   |   check    |  label   |    vector2 |
 *               vector3     |   matrix2     |  matrix3   |  matrix4 |   ;
 * input-type 输入类型，只有当widget是input时才有效： int | float | double | number | string | bool;
 * min-value 最小值，只有当widget是input时才有效：类型随input-type
 * max-value 最大值，只有当widget是input时才有效：类型随input-type
 * d-value 默认值，类型随input-type
 */
class AInterface extends HTMLElement {
    constructor() {
        console.log('this.getAttribute("in-port") :', this.getAttribute('in-port'));
        if (this.getAttribute('in-port')) {
            this.inPort = new APort(EPortType.INPUT);
            this.append(this.inPort);
        }
        if (this.getAttribute('out-port')) {
            this.outPort = new APort(EPortType.OUTPUT);
            this.append(this.outPort);
        }
        this.setAttribute('class', 'node-interface interface-out');
    }

    /** 获得端口
     * @param {EPortType} type 端口类型
     * @returns {APort} port 返回端口，若无端口则返回空null
     */
    getPort(type) {
        if (type === EPortType.INPUT && this.inPort !== undefined) {
            return this.inPort;
        }
        if (type === EPortType.OUTPUT && this.outPort !== undefined) {
            return this.outPort;
        }
        return null;
    }
}

class APort extends HTMLElement {
    /**
     * 创建一个端口
     * @param {EPortType} type 端口类型
     */
    constructor(type) {
        /** 单口类型，只有输入和输出，其具体可接纳的数据类型由接口 @see `AInterface` 控制 */
        this.portType = type;

        if (this.portType == EPortType.INPUT) {
            this.setAttribute('class', 'node-port-in');
        } else {
            this.setAttribute('class', 'node-port-out');
        }
    }

    getInterface() {
        var i = this.parentElement();
        if (i === null || !(i instanceof AInterface)) {
            throw '接口为空！此时父元素：' + i;
        }
        return i;
    }
}

customElements.define('aproch-flow-view', AFlowView);
customElements.define('aproch-node', ANode);
customElements.define('aproch-interface', AInterface);
customElements.define('aproch-port', APort);