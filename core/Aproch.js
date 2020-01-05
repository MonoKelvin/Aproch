// import { getUUID } from './utilities';

var NodeIDGenerator = 0;
var InterfaceIDGenerator = 0;
var PortIDGenerator = 0;
var ConnectionIDGenerator = 0;

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

        /** 场景中选择的项目 */
        this.selectedItems = [];

        /** 场景中的连线 */
        this.connections = [];

        this.onwheel = function (evt) {
            var t = $(this);                      // 视图的jquery对象

            // 按下ctrl放大缩小视图
            if (evt.ctrlKey == 1) {
                evt.preventDefault();
            }
        }

        /** 注册事件 */
        this.onmousedown = function (evt) {
            var t = $(this);                      // 视图的jquery对象
            var tdom = t[0];                      // 视图的dom对象
            let oldX = parseInt(t.css('left'));   // 视图起始x点
            let oldY = parseInt(t.css('top'));    // 视图起始y点
            let startEvtX = evt.clientX;          // 鼠标点击时起始x点
            let startEvtY = evt.clientY;          // 鼠标点击时起始y点
            let sfX = null;                       // selection-frame 选择框起始x点
            let sfY = null;                       // selection-frame 选择框起始y点
            var sfDiv = null;                     // selection-frame 选择框div标签

            // 按下ctrl加选
            if (evt.ctrlKey == 1) {
                console.log('ctrl被按下 :');
                return;
            }

            // 清空选择集
            tdom.selectedItems.length = 0;
            tdom.selectedItems = [];

            sfDiv = document.createElement('div');

            // 视图移动事件
            $(document).on('mousemove', function (em) {
                
                // 按下shift键就多选
                if (evt.shiftKey == 1) {
                    // tdom.selectedItems.length = 0;
                    // tdom.selectedItems = [];

                    sfDiv.style.cssText =
                        'position:absolute;width:0px;height:0px;font-size:0px;margin:0px;padding:0px;border:1px dashed #AAA;background-color:#333;z-index:1000;filter:alpha(opacity:60);opacity:0.6;display:none;';

                    t.append(sfDiv);
                    sfDiv.style.left = startEvtX + 'px';
                    sfDiv.style.top = startEvtY + 'px';

                    clearEventBubble(evt);

                    if (sfDiv.style.display == 'none') {
                        sfDiv.style.display = '';
                    }
                    sfX = em.clientX;
                    sfY = em.clientY;
                    sfDiv.style.left = (Math.min(sfX, startEvtX) - oldX) + 'px';
                    sfDiv.style.top = (Math.min(sfY, startEvtY) - oldY) + 'px';
                    sfDiv.style.width = Math.abs(sfX - startEvtX) + 'px';
                    sfDiv.style.height = Math.abs(sfY - startEvtY) + 'px';

                    let _l = sfDiv.offsetLeft,
                        _t = sfDiv.offsetTop;

                    let _w = sfDiv.offsetWidth,
                        _h = sfDiv.offsetHeight;

                    for (var i = 0; i < tdom.selectedItems.length; i++) {
                        let sl = tdom.selectedItems[i].offsetWidth + tdom.selectedItems[i].offsetLeft;
                        let st = tdom.selectedItems[i].offsetHeight + tdom.selectedItems[i].offsetTop;
                        if (
                            sl > _l &&
                            st > _t &&
                            tdom.selectedItems[i].offsetLeft < _l + _w &&
                            tdom.selectedItems[i].offsetTop < _t + _h
                        ) {
                            if (tdom.selectedItems[i].className.indexOf('seled') == -1) {
                                tdom.selectedItems[i].className = tdom.selectedItems[i].className + ' seled';
                            }
                        } else {
                            if (tdom.selectedItems[i].className.indexOf('seled') != -1) {
                                tdom.selectedItems[i].className = 'node-widget';
                            }
                        }
                    }
                } else {
                    t.css('cursor', 'grabbing');
                    let x = em.clientX - evt.clientX;
                    let y = em.clientY - evt.clientY;
                    t.css('left', String(oldX + x) + 'px');
                    t.css('top', String(oldY + y) + 'px');
                }
                return false;
            });

            $(document).on('mouseup', function () {
                t.css('cursor', 'grab');

                if (sfDiv) {
                    sfDiv.remove();
                }

                oldX = null;
                oldY = null;
                startEvtX = null;
                startEvtY = null;
                sfX = null;
                sfY = null;
                sfDiv = null;

                $(document).off('mousemove');
                $(document).off('mouseup');
            });

            return false;
        };
    }

    /**
     * 清空选择集
     */
    clearSelectedItems() {
        this.selectedItems.length = 0;
        this.selectedItems = [];
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
        this.attachTransform(node);
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

    addLinkingConnection(sourcePortID) {
        var conn = new AConnection();
        
        // 设置起始地固定点
        let p = document.querySelector('#' + sourcePortID).getPositionInView();
        conn.path.r.l = p.x + parseInt($(this).css('left'));
        conn.path.r.t = p.y + parseInt($(this).css('top'));

        $(this).prepend(conn);

        return conn;
    }

    /**
     * 为节点或者其他对象附加变换
     * @param {any} obj 要添加到该场景中的对象
     * @param {any} options 相关配置选项
     */
    attachTransform(
        obj,
        options = {
            /** 可以移动部分的元素名称，用class名 */
            moveable: 'node-title',
            /** 是否可以改变宽度 */
            isWidthCanChange: true,
            /** 是否可以改变高度 */
            isHeightCanChange: false
        }
    ) {
        var t = $(obj);
        var m = t.find('.' + options.moveable).first();

        let iwc = options.isWidthCanChange;
        let ihc = options.isHeightCanChange;
        let box_sizing = t.css('box-sizing');
        let top = t.css('top') - $(this).scrollTop();
        let left = t.css('left') - $(this).scrollLeft();
        let height = t.outerHeight();
        let width = t.outerWidth();
        let w_width = $(this).width();
        let w_height = $(this).height();
        let c_width = 0;
        let c_height = 0;

        if (box_sizing != 'border-box') {
            c_width = width - t.width();
            c_height = height - t.height();
        }
        t.css({
            'max-height': w_height - c_height,
            'max-width': w_width - c_width
        });
        $(this).resize(function () {
            w_width = $(this).width();
            w_height = $(this).height();
            if (box_sizing != 'border-box') {
                c_width = width - t.width();
                c_height = height - t.height();
            } else {
                c_width = 0;
                c_height = 0;
            }
            if (t.width() > w_width - c_width) {
                t.width(w_width - c_width);
            }
            if (t.height() > w_height - c_height) {
                t.height(w_height - c_height);
            }
            t.css({ 'max-height': w_height - c_height, 'max-width': w_width - c_width });
            height = t.outerHeight();
            width = t.outerWidth();
            if (width + left >= w_width) {
                left = w_width - width;
                if (parseInt(t.css('left')) < 0) {
                    t.css('left', -width);
                } else if (parseInt(t.css('left')) > w_width - width) {
                    t.css('left', w_width);
                } else {
                    t.css('left', left);
                }
            }
            if (height + top >= w_height) {
                top = w_height - height;
                if (parseInt(t.css('top')) < 0) {
                    t.css('top', -height);
                } else {
                    t.css('top', top);
                }
            }
        });

        // 移动部分
        t.on('mousedown', e => {
            e.stopPropagation();
        });
        m.on('mousedown', function (ed) {
            left = parseInt(t.css('left'));
            top = parseInt(t.css('top'));
            height = t.outerHeight();
            width = t.outerWidth();
            $(document).on('mousemove', function (e) {
                let left2 = left + e.pageX - ed.pageX;
                let top2 = top + e.pageY - ed.pageY;
                t.css({ top: top2, left: left2 });
                return false;
            });
            $(document).on('mouseup', function (e) {
                top = t.offset().top - $(this).scrollTop();
                left = t.offset().left - $(this).scrollLeft();
                $(document).off('mousemove');
                $(document).off('mouseup');
            });
            return false;
        });

        // 调整尺寸部分
        if (!iwc && !ihc) {
            return;
        }
        var rs = document.createElement('span');
        rs.setAttribute('class', 'resize-indicator');
        t.append(rs);
        t.css({
            'min-height': t.height(),
            'min-width': t.width()
        });
        rs.onmousedown = function (e) {
            let old_width = t.width();
            let old_height = t.height();
            let old_size_x = e.pageX;
            let old_size_y = e.pageY;
            $(document).on('mousemove', function (e) {
                if (ihc) {
                    let new_height = e.pageY - old_size_y + old_height;
                    t.height(new_height);
                    if (t.outerHeight() + top >= w_height) {
                        t.height(w_height - top - c_height);
                    }
                }
                if (iwc) {
                    let new_width = e.pageX - old_size_x + old_width;
                    t.width(new_width);
                    if (t.outerWidth() + left >= w_width) {
                        t.width(w_width - left - c_width);
                    }
                }
                return false;
            });
            $(document).on('mouseup', function () {
                width = t.outerWidth();
                height = t.outerHeight();
                $(document).off('mousemove');
                $(document).off('mouseup');
            });
            return false;
        };
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
        this.connections = { inputs: [], outputs: [] };

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

        this.id = 'node_' + NodeIDGenerator++;

        PortIDGenerator = 0;
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
        this.id = 'itf_' + NodeIDGenerator + '_' + InterfaceIDGenerator++;
        this.setAttribute('class', 'node-interface');
    }

    disconnectedCallback() {
        this.removePort(true, true);
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

        if (isInPort && !isOutPort) {
            $(this).addClass('interface-in');
        } else if (isOutPort && !isInPort) {
            $(this).addClass('interface-out');
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

    _inputWidgetBuild() { }
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

        this.id = 'port_' + NodeIDGenerator + '_' + PortIDGenerator++;

        if (this.portType == EPortType.INPUT) {
            this.setAttribute('class', 'node-port-in');
        } else {
            this.setAttribute('class', 'node-port-out');
        }

        // 点击时创建连线
        $(this).on('mousedown', function(ed){
            var conn = CurrentFV.addLinkingConnection(this.id);
            $(document).css('cursor', 'crosshair');
            
            $(document).on('mousemove', function(em){
                conn.moveLinkingPoint({x:ed.clientX, y:ed.clientY}, {x:em.clientX, y: em.clientY});
            });
            
            $(document).on('mouseup', function(evt){
                $(document).css('cursor', 'default');
                
                conn.remove();
                
                $(document).off('mousemove');
                $(document).off('mouseup');
            });
        });

        $(this).on('mousemove', function(em){
            if(em.target === this) {
                console.log('it is me');
            }
            return false;
        });
    }

    /**
     * 获得父端口
     * @returns {AInterface} 父端口
     */
    getInterface() {
        var i = this.parentElement();
        if (i === null || !(i instanceof AInterface)) {
            throw '接口为空！此时父元素：' + i;
        }
        return i;
    }

    getPositionInView() {
        var t = $(this);
        return { x: t.offset().left + t.width() / 2, y: t.offset().top + t.height() / 2 };
    }
}

/**
 * 连线类
 */
class AConnection extends HTMLElement {
    constructor(inPortID, outPortID) {
        super();

        /** 输入端口ID，对应节点的输出端口 */
        this.inPortID = inPortID;

        /** 输出端口ID，对应节点的输入端口 */
        this.outPortID = outPortID;

        // this.dataModel = null;

        /** 连线标签 */
        this.path = {
            id: "conn_" + ConnectionIDGenerator++,
            p1y: 0,
            p2y: 0,
            r: { l:0, t:0, w:0, h:0 },
            color: 'white',
            toString: function () {
                return '<polyline class="conn-path" style="stroke:' + this.color + '" id="' + this.id + '" points="0,' + this.p1y + ' ' + this.r.w + ',' + this.p2y + '"/>';
            }
        };
    }

    connectedCallback() {
        this.setAttribute('class', 'aproch-conn');
    }

    disconnectedCallback() {
        //todo: 从节点移除

        this.inPortID = null;
        this.outPortID = null;
        this.path = null;
    }

    _update() {
    }

    // updatePosition() {
    //     var i = document.querySelector('#' + this.inPortID);
    //     var o = document.querySelector('#' + this.outPortID);
    //     this.path.points.p1 = i.getPositionInView();
    //     this.path.points.p2 = o.getPositionInView();
    // }

    moveLinkingPoint(fixed, move) {
        this.path.r.w = Math.abs(fixed.x - move.x);
        this.path.r.h = Math.abs(fixed.y - move.y);
        
        if(fixed.x < move.x) {
            this.path.r.l = fixed.x;
        } else {
            this.path.r.l = move.x;
        }
        if(fixed.y < move.y) {
            this.path.r.t = fixed.y;
            if(fixed.x > move.x) {
                this.path.p1y = this.path.r.h;
                this.path.p2y = 0;
            } else {
                this.path.p1y = 0;
                this.path.p2y = this.path.r.h;
            }
        } else {
            this.path.r.t = move.y;
            if(fixed.x < move.x) {
                this.path.p1y = this.path.r.h;
                this.path.p2y = 0;
            } else {
                this.path.p1y = 0;
                this.path.p2y = this.path.r.h;
            }
        }
        
        this.innerHTML = '<svg xmlns="http://www.w3.org/2000/svg" version="1.1" width="'+this.path.r.w+'" height="'+this.path.r.h+'">' + this.path.toString() + '</svg>';
        this.style.left = this.path.r.l + 'px';
        this.style.top = this.path.r.t + 'px';
    }
}

customElements.define('aproch-flow-view', AFlowView);
customElements.define('aproch-node', ANode);
customElements.define('aproch-interface', AInterface);
customElements.define('aproch-port', APort);
customElements.define('aproch-connection', AConnection);

function addNodeTest(flowView) {
    flowView = document.querySelector(flowView);
    if (flowView === null || !(flowView instanceof AFlowView)) {
        alert('flowView不是有效的，flowView参数：' + flowView);
        return;
    }

    let node = document.createElement('aproch-node');
    flowView.addNode(node);
    node.setName('My Node');
    node.setPosition(100, 100);

    let itf = document.createElement('aproch-interface');
    node.addInterface(itf);
    itf.setPort(true, true);

    let label = new ALabelWidget('aproch-label');
    itf.addWidget(label);

    let itf2 = document.createElement('aproch-interface');
    node.addInterface(itf2);
    itf2.setPort(true, true);

    let input = document.createElement('aproch-input-number');
    itf2.addWidget(input);
    // input.addEventListener('change', function() {});
}