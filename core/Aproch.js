import { ITypeConverter, ABaseTypeConverter } from './TypeConverter.js';

var NodeIDGenerator = 0;
var InterfaceIDGenerator = 0;
var PortIDGenerator = 0;
var ConnectionIDGenerator = 0;

export const EPortType = {
    INPUT: 0,
    OUTPUT: 1
};

// export const PInterfaceOption = {
//     ui: null,
//     inPort: true,
//     outPort: true
// };

export class AFlowView extends HTMLElement {
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

        this.onwheel = function(evt) {
            var t = $(this); // 视图的jquery对象

            // 按下ctrl放大缩小视图
            if (evt.ctrlKey == 1) {
                evt.preventDefault();
            }
        };

        // 右键菜单事件
        this.oncontextmenu = function(e) {
            e.preventDefault();
            // todo: 弹出菜单
            return false;
        };

        /** 注册事件 */
        this.onmousedown = function(evt) {
            let t = $(this); // 视图的jquery对象
            let tdom = t[0]; // 视图的dom对象
            let oldX = parseInt(t.css('left')); // 视图起始x点
            let oldY = parseInt(t.css('top')); // 视图起始y点
            let startEvtX = evt.clientX; // 鼠标点击时起始x点
            let startEvtY = evt.clientY; // 鼠标点击时起始y点
            let sfX = null; // selection-frame 选择框起始x点
            let sfY = null; // selection-frame 选择框起始y点
            let sfDiv = null; // selection-frame 选择框div标签

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
            $(document).on('mousemove', function(em) {
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
                    sfDiv.style.left = Math.min(sfX, startEvtX) - oldX + 'px';
                    sfDiv.style.top = Math.min(sfY, startEvtY) - oldY + 'px';
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

            $(document).on('mouseup', function() {
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

        // 基本按键事件
        // let t = $(this);
        // $(this).on('keypress', function (k) {
        //     k.preventDefault();
        //     console.log('k :', k);
        // });
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
        if (node) {
            this.append(node);
            node.setPosition(x, y);
            this.attachTransform(node);
            this.nodes.push(node);
        }
    }

    /**
     * 删除一个节点
     * @param {ANode} node 要删除的节点
     */
    deleteNode(node) {
        if (node) {
            node.remove();
            node = null;
        }
        console.log('删除了一个无效的节点');
    }

    addLinkingConnection(sourcePortID) {
        let conn = new AConnection(this);

        // 设置起始地固定点
        let p = document.querySelector('#' + sourcePortID).getPositionInView();
        conn.path.r.l = p.x + parseInt($(this).css('left'));
        conn.path.r.t = p.y + parseInt($(this).css('top'));

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
        let t = $(obj);
        let m = t.find('.' + options.moveable).first();

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
        t.on('mousedown', e => {
            e.stopPropagation();
        });
        t.resize(function() {
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
        m.on('mousedown', function(ed) {
            left = parseInt(t.css('left'));
            top = parseInt(t.css('top'));
            height = t.outerHeight();
            width = t.outerWidth();
            $(document).on('mousemove', function(e) {
                let left2 = left + e.pageX - ed.pageX;
                let top2 = top + e.pageY - ed.pageY;
                t.css({ top: top2, left: left2 });
                return false;
            });
            $(document).on('mouseup', function(e) {
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
        let rs = document.createElement('span');
        rs.setAttribute('class', 'resize-indicator');
        t.append(rs);
        t.css({
            'min-height': t.height(),
            'min-width': t.width()
        });
        rs.onmousedown = function(e) {
            let old_width = t.width();
            let old_height = t.height();
            let old_size_x = e.pageX;
            let old_size_y = e.pageY;
            $(document).on('mousemove', function(e) {
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
            $(document).on('mouseup', function() {
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
export class ANode extends HTMLElement {
    constructor(flowView, dataModel, x = 0, y = 0) {
        super();

        /** UUID，标识每一个节点 */
        // this.uuid = getUUID();

        /** 节点的 x 坐标 */
        this.x = x;

        /** 节点的 y 坐标 */
        this.y = y;

        /** 节点头部元素 */
        this.nodeTitle = document.createElement('div');

        /** 节点内容，放置所有接口的容器 */
        this.nodeContent = document.createElement('div');

        /** 包含的所有接口 */
        this.interfaces = [];

        /** 管理的所有连线 */
        this.connections = { inputs: [], outputs: [] };

        /** 管理所有接口的数据模型映射
         * @example
         * [
         *      'itf_1_1': new IDataModel(),
         *      'itf_1_2': new MyDataModel(),
         *      'itf_1_3': new CustomDataModel(),
         * ]
         */
        this.dataModel = dataModel;

        /* 初始化节点控件 */
        this.setAttribute('class', 'node-widget');
        this.nodeTitle.setAttribute('class', 'node-title');
        this.nodeTitle.innerHTML = dataModel.name;
        this.nodeContent.setAttribute('class', 'node-content');

        /* 添加称为子组件 */
        this.append(this.nodeTitle);
        this.append(this.nodeContent);

        this._initDataModel();

        /* 添加到场景 */
        flowView.addNode(this);

        this.id = 'node_' + NodeIDGenerator++;
        this.setPosition(this.x, this.y);

        // 端口ID清零
        PortIDGenerator = 0;

        this.addEventListener('onmousemove', () => {
            this.connections.inputs.forEach(ic => {
                ic._update();
            });
            this.connections.outputs.forEach(oc => {
                oc._update();
            });
        });
    }

    attachConnection(conn, port) {
        if (port.type === EPortType.INPUT) {
            this.connections.inputs.push(conn);
        } else {
            this.connections.outputs.push(conn);
        }
    }

    _initDataModel() {
        // 创建接口
        for (let i = 0; i < 1024; i++) {
            var itfOption = this.dataModel.uiBuilder(i);
            if (!itfOption) {
                break;
            }
            new AInterface(this, i, itfOption);
        }
    }

    /**
     * 元素被移除时调用
     */
    disconnectedCallback() {
        _propagationData();

        this.interfaces.length = 0;
        delete this.interfaces;

        this.dataModel.length = 0;
        delete this.dataModel;
    }

    _propagationData() {
        this.dataModel.forEach(dm => {
            dm.dataModel.outputData();
        });
    }

    /**
     * 设置节点标题
     * @param {string} name 标题字符串
     */
    setTitle(name) {
        this.nodeTitle.innerHTML = name;
    }

    /**
     * 获得节点标题显示的文字
     */
    getTitle() {
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

    getFlowView() {
        let fv = null;
        $.each($(this).parents(), function(_, p) {
            if (p instanceof AFlowView) {
                fv = p;
                return;
            }
        });
        return fv;
    }

    /**
     * 获得接口
     */
    // getInterfaces() {
    //     let result = [];
    //     this.children().forEach(element => {
    //         if (element instanceof AInterface) {
    //             result.push(element);
    //         }
    //     });
    //     return result;
    // }
}

export class AInterface extends HTMLElement {
    constructor(node, index, options) {
        super();

        /** 输入端口（左侧） */
        this.inPort = null;

        /** 输出端口（右侧） */
        this.outPort = null;

        /** 接口索引 */
        this.index = index;

        if (node) {
            node.addInterface(this);
        }

        this.setPort(options.isInPort, options.isOutPort);

        this.id = 'itf_' + NodeIDGenerator + '_' + InterfaceIDGenerator++;
        this.setAttribute('class', 'node-interface');

        this.append(options.ui);
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
        if (isInPort && !this.inPort) {
            this.inPort = new APort(EPortType.INPUT);
            this.append(this.inPort);
        }
        if (isOutPort && !this.outPort) {
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
        if (type === EPortType.INPUT) {
            return this.inPort;
        }
        if (type === EPortType.OUTPUT) {
            return this.outPort;
        }
        return null;
    }

    /** 添加一个控件
     * @param {IWidget} widget 要添加widget
     */
    setWidget(widget) {
        if (!widget || $.inArray(widget, $(this).children()) > -1) {
            return;
        }

        // 如果该数据模型已经存在就移除
        let d = this.getDataModel();
        if (d) {
            d.remove();
            this.offsetParent.dataModel.widgets.remove(d);
        }

        this.offsetParent.dataModel.push({ id: this.id, dataModel: widget });
        this.append(widget.ui);
    }

    getWidget() {
        let result = null;
        this.offsetParent.dataModel.widgets.forEach(w => {
            if (w.id === this.id) {
                result = w;
            }
        });
        return result;
    }
}

export class APort extends HTMLElement {
    /**
     * 创建一个端口
     * @param {EPortType} type 端口类型
     */
    constructor(type) {
        super();

        /** 端口类型，只有输入和输出，其具体可接纳的数据类型由接口 @see `AInterface` 控制 */
        this.port = type;

        this.id = 'port_' + NodeIDGenerator + '_' + PortIDGenerator++;

        if (this.port == EPortType.INPUT) {
            this.setAttribute('class', 'node-port-in');
        } else {
            this.setAttribute('class', 'node-port-out');
        }

        // 点击时创建连线
        $(this).on('mousedown', function(ed) {
            let t = this;
            let f = { x: ed.clientX, y: ed.clientY }; //鼠标按下的固定点
            let canLink = false; // 是否可连

            // 创建连接线
            let conn = t
                .getNode()
                .getFlowView()
                .addLinkingConnection(this.id);

            $(document).on('mousemove', function(em) {
                conn._setLinkingPoint(f, { x: em.clientX, y: em.clientY });

                let tar = em.target;

                // 遇到另一个端口
                if (tar instanceof APort && tar !== ed.target) {
                    if (APort.CanLink(t, tar)) {
                        let p = tar.getPositionInView();

                        // 1.数据类型一致或通过转换器达到一致
                        conn._setLinkingPoint(f, p);

                        // 2.数据不一致或不可转换则不作任何响应
                        canLink = true;
                    }
                }
            });

            $(document).on('mouseup', function(eu) {
                if (!canLink) {
                    conn.remove();
                } else {
                    // 附加连线到节点上
                    t.offsetParent.attachConnection(conn, t.type);
                    tar.offsetParent.attachConnection(conn, tar.type);
                }

                $(document).off('mousemove');
                $(document).off('mouseup');
            });
        });
    }

    /**
     * 获得端口所在接口，不是jquery对象
     * @returns {AInterface} 端口所在的接口
     */
    getInterface() {
        return this.parentNode;
    }

    getNode() {
        return this.offsetParent;
    }

    getPositionInView() {
        let t = $(this);
        return { x: t.offset().left + t.width() / 2, y: t.offset().top + t.height() / 2 };
    }

    /**
     * 判断给定的两个端口通过类型转换器是否可以相连接
     * @param {APort} p1 端口1
     * @param {APort} p2 端口2
     * @param {ITypeConverter} tv 类型转换器
     * @returns {boolean} 如果可以相连接则返回true，否则返回false
     * @note 判断两个节点可连的条件：
     *  1. 不是`同一个节点`的端口
     *  2. 端口类型不能一样，即只能`INPUT`对应`OUTPUT`
     *  3. 该端口所在的接口数据类型一样或可以隐式转换
     *  4. 实现类型转换的方法返回true
     */
    static CanLink(p1, p2, tv = null) {
        if (p1.getNode() !== p2.getNode() && p1.port !== p2.port) {
            if (ABaseTypeConverter.CanConvert(p1.getInterface().getDataModel(), p2.getInterface().getDataModel())) {
                return true;
            } else if (tv && tv.CanConvert()) {
                return true;
            }
        }

        return false;
    }
}

/**
 * 连线类
 */
export class AConnection extends HTMLElement {
    constructor(flowView, inPort, outPort) {
        super();

        /** 输入端口，对应节点的输出端口 */
        this.inPort = inPort;

        /** 输出端口，对应节点的输入端口 */
        this.outPort = outPort;

        // this.data = null;

        /** 连线标签 */
        this.path = {
            id: 'conn_' + ConnectionIDGenerator++,
            p1y: 0,
            p2y: 0,
            r: { l: 0, t: 0, w: 0, h: 0 },
            color: 'white',
            toString: function() {
                return (
                    '<polyline class="conn-path" style="stroke:' +
                    this.color +
                    '" id="' +
                    this.id +
                    '" points="0,' +
                    this.p1y +
                    ' ' +
                    this.r.w +
                    ',' +
                    this.p2y +
                    '"/>'
                );
            }
        };

        this.setAttribute('class', 'aproch-conn');

        $(flowView).prepend(this);
    }

    disconnectedCallback() {
        //todo: 从节点移除

        this.path = null;
    }

    _update() {
        this._setLinkingPoint(this.inPort.getPositionInView(), this.outPort.getPositionInView());
    }

    // updatePosition() {
    //     let i = document.querySelector('#' + this.inPortID);
    //     let o = document.querySelector('#' + this.outPortID);
    //     this.path.points.p1 = i.getPositionInView();
    //     this.path.points.p2 = o.getPositionInView();
    // }

    /**
     * 设置正在连接（只有一端是有端口，另一端还没有端口）的连线
     * @param {any} fixed 固定点，一般为鼠标点击时的点
     * @param {any} move 移动点，一般为鼠标移动时的点
     */
    _setLinkingPoint(fixed, move) {
        this.path.r.w = Math.abs(fixed.x - move.x);
        this.path.r.h = Math.abs(fixed.y - move.y);

        if (fixed.x < move.x) {
            this.path.r.l = fixed.x;
        } else {
            this.path.r.l = move.x;
        }
        if (fixed.y < move.y) {
            this.path.r.t = fixed.y;
            if (fixed.x > move.x) {
                this.path.p1y = this.path.r.h;
                this.path.p2y = 0;
            } else {
                this.path.p1y = 0;
                this.path.p2y = this.path.r.h;
            }
        } else {
            this.path.r.t = move.y;
            if (fixed.x < move.x) {
                this.path.p1y = this.path.r.h;
                this.path.p2y = 0;
            } else {
                this.path.p1y = 0;
                this.path.p2y = this.path.r.h;
            }
        }

        this.style.left = this.path.r.l + 'px';
        this.style.top = this.path.r.t + 'px';
        this.style.width = this.path.r.w + 'px';
        this.style.height = this.path.r.h + 'px';
        this.innerHTML = '<svg xmlns="http://www.w3.org/2000/svg" version="1.1" >' + this.path.toString() + '</svg>';
    }
}
