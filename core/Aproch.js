// import { getUUID } from './utilities';

var NodeIDGenerator = 0;

var EPortType = {
    INPUT: 0,
    OUTPUT: 1
};

var EWidgetType = {
    Label: 'label',
    Input: 'input',
    Check: 'check',
    Vector: 'vector',
    Matrix: 'matrix',
    Image: 'image',
    File: 'file'
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
                var startX = evt.clientX;
                var startY = evt.clientY;
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
                        _x = evt.clientX;
                        _y = evt.clientY;
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
                    // this.nodes.forEach(function(node) {
                    //     node.setPosition(oldX + x, oldY + y);
                    // });
                    return false;
                };
            }

            this.onmouseup = function() {
                this.style.cursor = 'grab';
                this.onmousemove = null;

                isSelect = false;

                if (selDiv) {
                    selDiv.remove();
                    // for (let i = 0; i < selList.length; i++) {
                    //     if (selList[i].className.indexOf('seled') != -1) {
                    //         // count++;
                    //     }
                    // }
                }

                selList = null;
                _x = null;
                _y = null;
                selDiv = null;
                startX = null;
                startY = null;
                evt = null;
            };

            this.onmouseleave = this.onmouseup;

            return false;
        };

        /** 添加画布，主要是为了绘制连线 */
        // this.canvas = document.createElement('canvas');
        // this.append(this.canvas);
    }

    /**
     * 添加一个节点
     * @param {ANode} node 要加入场景的节点
     * @param {number} x 加入时的x坐标
     * @param {number} y 加入时的y坐标
     */
    addNode(node, x = 0, y = 0) {
        if (node === null || !(node instanceof ANode)) {
            throw '节点无效！';
        }

        this.append(node);

        node.setPosition(x, y);

        this.nodes.push(node);
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

/** 节点
 * <aproch-node name="my node" title-color="#123465"></aproch-node>
 */
class ANode extends HTMLElement {
    constructor() {
        super();

        /** UUID，标识每一个节点 */
        // this.uuid = getUUID();

        /** 节点的 x 坐标 */
        this.x = 0;

        /** 节点的 y 坐标 */
        this.y = 0;

        /** 节点头部元素 */
        this.nodeTitle = null;

        /** 节点内容，放置所有接口的容器 */
        this.nodeContent = null;

        /** 包含的所有接口 */
        this.interfaces = [];

        /** 管理的所有连线 */
        // this.connections = [];

        /** 数据模型 */
        // this.dataModel = null;
    }

    /**
     * 元素被插入文档中被调用
     */
    connectedCallback() {
        /** 节点头部元素 */
        this.nodeTitle = document.createElement('div');

        /** 节点内容，放置所有接口的容器 */
        this.nodeContent = document.createElement('div');

        /* 初始化节点控件 */
        this.setAttribute('class', 'node-widget');
        this.nodeTitle.setAttribute('class', 'node-title');
        this.nodeContent.setAttribute('class', 'node-content');

        /* 添加称为子组件 */
        this.append(this.nodeTitle);
        this.append(this.nodeContent);

        /* 设置位置 */
        this.setPosition(0, 0);

        /* 注册为可移动节点*/
        $(this).addMoveComponent();
    }

    /**
     * 元素被移除时调用
     */
    disconnectedCallback() {
        this.interfaces.length = 0;
        this.interfaces = null;
        delete this.interfaces;
    }

    /**
     * 设置节点的名称
     * @param {string} name 节点的名称，这个名称会显示在标题
     */
    setName(name) {
        this.nodeTitle.innerHTML = name;
    }

    /**
     * 获得节点的名称，也就是节点标题显示的文字
     */
    getName() {
        return this.nodeTitle.innerHTML;
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
            x: this.style.left,
            y: this.style.top
        };
    }

    /**
     * 设置节点在视图中的位置
     * @param {number} x x坐标
     * @param {number} y y坐标
     */
    setPosition(x, y) {
        /* 设置位置 */
        this.style.left = x + 'px';
        this.style.top = y + 'px';
    }

    /**
     * 增加节点在视图中的位置
     * @param {number} dx x增量
     * @param {number} dy y增量
     * @note 等价于 position = position + (dx, dy)
     */
    addPosition(dx, dy) {
        this.style.left = parseInt($(this).css('left')) + dx + 'px';
        this.style.top = parseInt($(this).css('top')) + dy + 'px';
    }

    /**
     * 添加接口
     * @param {AInterface} itf 要添加的接口
     */
    addInterface(itf) {
        if (this.interfaces.indexOf(itf) < 0) {
            this.interfaces.push(itf);
            if (this.nodeContent) {
                this.nodeContent.appendChild(itf);
            }
        } else {
            console.log('接口已经存在，无法重复添加。interface:', itf);
        }
    }

    /**
     * 获得接口
     */
    // getInterfaces() {
    //     var result = [];
    //     this.children().forEach(element => {
    //         if (element instanceof AInterface) {
    //             result.push(element);
    //         }
    //     });
    //     return result;
    // }
}

/** 序列化选项
 * <aproch-interface in-port="true" out-port="" ></aproch-interface>
 */
class AInterface extends HTMLElement {
    constructor() {
        super();

        /** 输入端口（左侧） */
        this.inPort = undefined;

        /** 输出端口（右侧） */
        this.outPort = undefined;
    }

    connectedCallback() {
        this.setAttribute('class', 'node-interface interface-out');
    }

    disconnectedCallback() {
        removePort(true, true);
    }

    /**
     * 移除输入端口，没有输入端口则什么都不做
     * @param {boolean} isInPort 是否要移除输入端口（左侧）
     * @param {boolean} isOutPort 是否要移除输出端口（右侧）
     */
    removePort(isInPort, isOutPort) {
        if (isInPort && this.inPort) {
            this.inPort.remove();
            this.inPort = null;
            delete this.inPort;
        }
        if (isOutPort && this.outPort) {
            this.outPort = null;
            delete this.outPort;
        }
    }

    /**
     * 设置输出输出端口
     * @param {boolean} isInPort 是否添加输入端口
     * @param {boolean} isOutPort 是否添加输出端口
     * @note 当存在输入或输出端口时，可以设置为false以移除端口
     * @see removePort();
     */
    setPort(isInPort, isOutPort) {
        if (isInPort) {
            this.inPort = new APort(EPortType.INPUT);
            this.append(this.inPort);
        }
        if (isOutPort) {
            this.outPort = new APort(EPortType.OUTPUT);
            this.append(this.outPort);
        }

        this.removePort(!isInPort, !isOutPort);
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

    /** 添加一个控件
     * @param {Element} widget 要添加widget
     */
    addWidget(widget) {
        this.append(widget);
    }

    _inputWidgetBuild() {}
}

class APort extends HTMLElement {
    /**
     * 创建一个端口
     * @param {EPortType} type 端口类型
     */
    constructor(type) {
        super();

        /** 端口类型，只有输入和输出，其具体可接纳的数据类型由接口 @see `AInterface` 控制 */
        this.portType = type;

        if (this.portType == EPortType.INPUT) {
            this.setAttribute('class', 'node-port-in');
        } else {
            this.setAttribute('class', 'node-port-out');
        }

        this.onmousedown = function() {
            AFlowView.createPath();
        };
    }

    getInterface() {
        var i = this.parentElement();
        if (i === null || !(i instanceof AInterface)) {
            throw '接口为空！此时父元素：' + i;
        }
        return i;
    }
}

/**
 * 连线类
 */
class AConnection {
    constructor(inPortID, outPortID) {
        this.inPortID = inPortID;

        this.outPortID = outPortID;

        // this.dataModel = null;

        this.path = '';
    }

    createPath() {
        let sp = { x: 0, y: 0 };
        let ep = { x: 200, y: 100 };

        // let sd = { x: sp.x + 15, y: sp.y };
        // let ed = { x: ep.x - 15, y: ep.y };

        this.path = '<g><polyline points="';
        this.path += sp.x + ',' + sp.y + ' ';
        // this.path += sd.x + ',' + sd.y + ' ';
        // this.path += ed.x + ',' + ed.y + ' ';
        this.path += ep.x + ',' + ep.y + ' ';
        this.path += '" style="fill:none;stroke:white;stroke-width:2"/></g>';

        return this.path;
    }

    moveEndPoint() {}
}

customElements.define('aproch-flow-view', AFlowView);
customElements.define('aproch-node', ANode);
customElements.define('aproch-interface', AInterface);
customElements.define('aproch-port', APort);

function addNodeTest(flowView) {
    flowView = document.querySelector(flowView);
    if (flowView === null || !(flowView instanceof AFlowView)) {
        alert('flowView不是有效的，flowView参数：' + flowView);
        return;
    }

    node = document.createElement('aproch-node');
    flowView.addNode(node);
    node.setName('My Node');

    itf = document.createElement('aproch-interface');
    itf.setPort(true, true);
    node.addInterface(itf);

    // input = document.createElement('aproch-input-number');
    // input.addEventListener('change', function() {});
    // itf.addWidget(input);

    label = new ALabelWidget('aproch-label-widget');
    itf.addWidget(label);
}