import { ITypeConverter, ABaseTypeConverter } from './TypeConverter.js';

var NodeIDGenerator = 0;
var InterfaceIDGenerator = 0;
var PortIDGenerator = 0;
var ConnectionIDGenerator = 0;

export const EPortType = {
    INPUT: 0,
    OUTPUT: 1,
};

const MAX_INTERFACE_COUNTER = 100;

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
        // this.connections = [];

        this.onwheel = function (evt) {
            var t = $(this); // 视图的jquery对象

            // 按下ctrl放大缩小视图
            if (evt.ctrlKey == 1) {
                evt.preventDefault();
            }
        };

        // 右键菜单事件
        this.oncontextmenu = function (e) {
            e.preventDefault();
            // todo: 弹出菜单
            return false;
        };

        /** 注册事件 */
        this.onmousedown = function (evt) {
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
            this.attachTransformForNode(node);
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

    addLinkingConnection(sourcePort) {
        let conn = new AConnection(this);

        // 设置起始地固定点
        let p = sourcePort.getPositionInView();
        conn.path.r.l = p.x + parseInt(this.style.left);
        conn.path.r.t = p.y + parseInt(this.style.top);

        return conn;
    }

    /**
     * 为节点附加变换
     * @param {any} node 要添加到该场景中的节点
     * @param {any} options 相关配置选项
     */
    attachTransformForNode(
        node,
        options = {
            /** 可以移动部分的元素名称，用class名 */
            moveable: 'node-title',
            /** 是否可以改变宽度 */
            isWidthCanChange: true,
            /** 是否可以改变高度 */
            isHeightCanChange: false,
        }
    ) {
        const t = $(node);
        const m = t.find('.' + options.moveable).first();

        const iwc = options.isWidthCanChange;
        const ihc = options.isHeightCanChange;
        const box_sizing = t.css('box-sizing');
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
            'max-width': w_width - c_width,
        });
        t.on('mousedown', (e) => {
            e.stopPropagation();
        });
        t.resize(function () {
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
        m.on('mousedown', function (ed) {
            left = parseInt(t.css('left'));
            top = parseInt(t.css('top'));
            height = t.outerHeight();
            width = t.outerWidth();
            $(document).on('mousemove', function (e) {
                let left2 = left + e.pageX - ed.pageX;
                let top2 = top + e.pageY - ed.pageY;
                t.css({ top: top2, left: left2 });
                t[0]._updateConnectionPosition();
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
        let rs = document.createElement('span');
        rs.setAttribute('class', 'resize-indicator');
        t.append(rs);
        t.css({
            'min-height': t.height(),
            'min-width': t.width(),
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
                t[0]._updateConnectionPosition();
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

export class ANode extends HTMLElement {
    constructor(flowView, dataModel, x = 0, y = 0) {
        super();

        /** UUID，标识每一个节点 */
        // this.uuid = getUUID();

        /** 节点头部元素 */
        this.nodeTitle = document.createElement('div');

        /** 节点内容，放置所有接口的容器 */
        this.nodeContent = document.createElement('div');

        /** 管理的数据模型 */
        this.dataModel = dataModel;

        // 初始化节点控件
        this.setAttribute('class', 'node-widget');
        this.nodeTitle.setAttribute('class', 'node-title');
        this.nodeTitle.innerHTML = dataModel.name;
        this.nodeContent.setAttribute('class', 'node-content');
        this.id = 'node_' + NodeIDGenerator++;

        // 端口ID清零
        PortIDGenerator = 0;

        // 添加为子组件
        this.append(this.nodeTitle);
        this.append(this.nodeContent);

        // 初始化数据模型，添加接口
        for (let i = 0; i < MAX_INTERFACE_COUNTER; i++) {
            var itfOption = this.dataModel.uiBuilder(i);
            if (!itfOption) {
                break;
            }
            new AInterface(this, itfOption);
        }

        // 添加到场景并设置位置
        flowView.addNode(this);
        this.setPosition(x, y);
    }

    /**
     * 元素被移除时调用
     */
    disconnectedCallback() {
        _propagationData();
    }

    _propagationData() {
        this.dataModel.forEach((dm) => {
            dm.dataModel.outputData();
        });
    }

    /**
     * 更新连线的位置
     */
    _updateConnectionPosition() {
        this.getInterfaces().forEach((i) => {
            if (i.getPort(EPortType.INPUT)) {
                i.getPort(EPortType.INPUT).connections.forEach((conn) => {
                    conn._update();
                });
            }
            if (i.getPort(EPortType.OUTPUT)) {
                i.getPort(EPortType.OUTPUT).connections.forEach((conn) => {
                    conn._update();
                });
            }
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
     * @returns 返回坐标对象{x,y}
     */
    getPosition() {
        return {
            x: parseInt(this.style.left),
            y: parseInt(this.style.top),
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
        this.style.left = parseInt(this.style.left) + dx + 'px';
        this.style.top = parseInt(this.style.top) + dy + 'px';
    }

    /**
     * 添加接口
     * @param {AInterface} itf 要添加的接口
     */
    addInterface(itf) {
        try {
            this.nodeContent.childNodes.forEach((i) => {
                if (i === itf) {
                    throw new Error('接口已经存在，无法重复添加。interface:', itf);
                }
            });
            this.nodeContent.appendChild(itf);
        } catch (e) {
            console.log(e);
        }
    }

    /**
     * 获得所有的接口
     * @returns {AInterface[]} 接口数组
     */
    getInterfaces() {
        const interfaces = [];
        this.nodeContent.childNodes.forEach((i) => {
            if (i instanceof AInterface) {
                interfaces.push(i);
            }
        });
        return interfaces;
    }

    /**
     * 获得节点当前所在的视图
     * @returns {AFlowView} 返回视图类
     */
    getFlowView() {
        let fv = null;
        $.each($(this).parents(), function (_, p) {
            if (p instanceof AFlowView) {
                fv = p;
                return;
            }
        });
        return fv;
    }

    /**
     * 在节点末尾处追加一些有用的提示文本
     * @param {Object} option 内容对象。可用参数示例：
     *
     * ### content: string
     *  提示的文本内容，支持html格式
     *
     * ### type: string
     *  提示类型，支持一下参数
     *  * default（默认）
     *  * alert
     *  * info
     *  * warning
     *
     * ### delay: string | number
     *  延迟关闭时间。单位为ms，同时支持以下参数
     *  * short = 1500ms （默认）
     *  * long = 3000ms
     * @return {ANode} 返回自身节点，即可以再次使用该方法
     */
    pushPromptText(option) {
        if (typeof option === 'object') {
            let prompt = $('<div class="node-prompt"></div>');
            prompt.html(option.content);
            prompt.prepend($('<i class="fa fa-exclamation-triangle" aria-hidden="true"></i>'));

            if (typeof option.type == 'string') {
                prompt.addClass('bg-' + option.type);
            }

            let timeout = 1500;
            if (option.delay === 'long') {
                timeout = 3000;
            } else if (isNaN(parseInt(option.delay))) {
                timeout = 1500;
            }

            setTimeout(() => {
                prompt.remove();
                prompt = null;
            }, timeout);

            this.nodeContent.appendChild(prompt[0]);
        }

        return this;
    }
}

export class AInterface extends HTMLElement {
    constructor(node, options) {
        super();

        /** 输入端口（左侧） */
        this.inPort = null;

        /** 输出端口（右侧） */
        this.outPort = null;

        this.id = 'itf_' + NodeIDGenerator + '_' + InterfaceIDGenerator++;
        this.setAttribute('class', 'node-interface');

        node.addInterface(this);

        this.setPort(options.isInPort, options.isOutPort);
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
        }
        if (isOutPort && this.outPort) {
            this.outPort.remove();
            this.outPort = null;
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
            if (!isOutPort) {
                $(this).addClass('interface-in');
            }
        }
        if (isOutPort && !this.outPort) {
            this.outPort = new APort(EPortType.OUTPUT);
            this.append(this.outPort);
            if (!isInPort) {
                $(this).addClass('interface-out');
            }
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

    getNode() {
        return this.offsetParent;
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
        this.type = type;

        /** 端口连接连线的数量限制，在此值范围内该端口才能进行连线。
         * @note 默认输出端口最多连接`无数条`，输入端口最多连接`1条`
         */
        this.connectCountLimit = type === EPortType.INPUT ? 1 : Infinity;

        /** 管理连接的所有端口 */
        this.connections = [];

        this.id = 'port_' + NodeIDGenerator + '_' + PortIDGenerator++;

        if (this.type == EPortType.INPUT) {
            this.setAttribute('class', 'node-port-in');
        } else {
            this.setAttribute('class', 'node-port-out');
        }

        // 点击时创建连线
        $(this).on('mousedown', function (ed) {
            const t = this;
            const tp = t.offsetParent;
            let tar = null;
            let f = t.getPositionInView();
            let conn = null;
            let canLink = false; // 是否可连

            /*
                1.输入端口如果有连线则移动已经存在的那条线
                2.否则如果在限制连线数量的范围内，就创建新的连线
            */
            if (t.type == EPortType.INPUT && t.connections.length > 0) {
                conn = t.connections[0];
                f = t.connections[0].inPort.getPositionInView();
            } else if (t.getConnectionCount() < t.connectCountLimit) {
                // 创建连接线
                conn = tp.getFlowView().addLinkingConnection(t);
            } else {
                // 提示数量已达上线值，只针对 connectCountLimit > 1 的情况
                t.getNode().pushPromptText({
                    content: '连线数量已达上限：' + t.connectCountLimit,
                    type: 'alert',
                    delay: 'long',
                });
                return;
            }

            $(document).on('mousemove', function (em) {
                conn._setLinkingPoint(f, { x: em.clientX, y: em.clientY });

                tar = em.target;

                // 遇到另一个端口
                canLink = false;
                if (tar instanceof APort && tar !== ed.target) {
                    if (APort.CanLink(t, tar)) {
                        const p = tar.getPositionInView();

                        // 1.数据类型一致或通过转换器达到一致
                        conn._setLinkingPoint(f, p);

                        // 2.数据不一致或不可转换则不作任何响应
                        canLink = true;
                    }
                }
            });

            $(document).on('mouseup', function (eu) {
                if (!canLink) {
                    conn.remove();
                } else {
                    // 附加连线到节点上
                    t.attachConnection(conn);
                    tar.attachConnection(conn);
                }

                $(document).off('mousemove');
                $(document).off('mouseup');
            });
        });
    }

    /**
     * 获取连接的对立端口
     * @returns {AConnection[]} 如果是输入端口，那返回结果只有一条连线，否则可能有多条
     */
    getOppositePort() {
        let ports = [];
        if (this.type === EPortType.INPUT) {
            if (this.connections.length > 0) {
                ports.push(this.connections[0].inPort);
            }
        } else {
            this.connections.forEach((c) => {
                ports.push(c.outPort);
            });
        }
        return ports;
    }

    /**
     * 获得端口所在接口，不是jquery对象
     * @returns {AInterface} 端口所在的接口
     */
    getInterface() {
        return this.parentNode;
    }

    /**
     * 附加连线到该端口上
     * @param {AConnection} conn 要附加的连线
     */
    attachConnection(conn) {
        this.connections.push(conn);
        if (this.type === EPortType.INPUT) {
            conn.outPort = this;
        } else {
            conn.inPort = this;
        }
    }

    detachConnection(conn) {
        this.connections.splice(conn);
        // todo: 更新数据
    }

    /**
     * 获得端口所在的节点
     */
    getNode() {
        return this.offsetParent;
    }

    /**
     * 获得端口上连线的数量
     */
    getConnectionCount() {
        return this.connections.length;
    }

    /**
     * 获得端口在视图中的位置
     * @returns 返回坐标对象{x, y}
     */
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
     *  2. 不能存在连接
     *  3. 端口类型不能一样，即只能`INPUT`对应`OUTPUT`
     *  4. 在输入端口数量限制内（通常输入端口只能连一条）
     *  5. 该端口所在的接口数据类型一样或可以隐式转换
     *  6. 实现类型转换的方法返回true
     */
    static CanLink(p1, p2, tv = null) {
        if (
            p1.getNode() !== p2.getNode() &&
            p1.type !== p2.type &&
            p1.getOppositePort()[0] !== p2 &&
            p1.getConnectionCount() < p1.connectCountLimit &&
            p2.getConnectionCount() < p2.connectCountLimit
        ) {
            if (ABaseTypeConverter.CanConvert(p1.getInterface(), p2.getInterface())) {
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
            toString: function () {
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
            },
        };

        this.setAttribute('class', 'aproch-conn');

        $(flowView).prepend(this);
    }

    disconnectedCallback() {
        if (this.inPort) {
            this.inPort.detachConnection(this);
        }
        if (this.outPort) {
            this.outPort.detachConnection(this);
        }

        this.path = null;
    }

    _update() {
        this._setLinkingPoint(this.inPort.getPositionInView(), this.outPort.getPositionInView());
    }

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
