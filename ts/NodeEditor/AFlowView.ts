import { addResizeComponent, getElementsByClassName } from '../Utilities';
import { AbstractFlowViewItem } from './AFlowViewItem';
import ANode from './ANode';
import APort from './APort';
import AConnection from './AConnection';
import ANodeModelRegistry from './ANodeModelRegistry';

/**
 * 视图事件状态，用于在视图中区别不同事件
 */
enum EViewEventState {
    NoneState, // 无
    SelectItem, // 单选
    DeselectItem, // 取消选择
    MultiSelectItem, // 多选
    MoveItem, // 移动
}

export default class AFlowView extends HTMLElement {
    /**
     * 保存的节点表，所有视图共享
     * @see ANodeModelRegistry
     * @note ES6不支持静态属性，ES7支持
     */
    static NodeTable: ANodeModelRegistry;

    private name: string | null = 'Flow View';

    eventState: EViewEventState;

    /** 创建一个指定name的节点编辑视图控件
     * @param {string} name 该视图的名称
     */
    constructor(name: string) {
        super();

        this.name = this.getAttribute('name') ? this.getAttribute('name') : name;

        /** 场景中所有的节点 */
        // this.nodes = [];

        /** 事件状态 */
        this.eventState = EViewEventState.NoneState;

        /** 场景中选择的项目 */
        // this.selectedItems = [];

        /** 场景中的连线 */
        // this.connections = [];

        this.onwheel = function (evt) {
            var t = $(this); // 视图的jquery对象

            // 按下ctrl放大缩小视图
            if (evt.ctrlKey == true) {
                evt.preventDefault();
            }
        };

        // 右键菜单事件
        this.oncontextmenu = function (e) {
            e.preventDefault();
            // todo: 弹出菜单
            return false;
        };

        this.onclick = (evt: any) => {
            // 点击到空白处
            if (evt.target == this && !evt.shiftKey && !evt.ctrlKey) {
                let selectedNodes = this.getSelectedItems();
                $(selectedNodes).removeClass('fv-item-slt');
            }
        };

        /** 注册事件 */
        this.onmousedown = (evt) => {
            let t = $(this); // 视图的jquery对象
            let tdom = this; // 视图的dom对象
            let oldX = parseInt(t.offset().left); // 视图起始x点
            let oldY = parseInt(t.offset().top); // 视图起始y点
            let startEvtX = evt.clientX; // 鼠标点击时起始x点
            let startEvtY = evt.clientY; // 鼠标点击时起始y点
            let sfX = null; // selection-frame 选择框起始x点
            let sfY = null; // selection-frame 选择框起始y点
            let sfDiv: any = null; // selection-frame 选择框div标签

            const curItem = ANode.GetNodeByChildComponent(evt.target);
            if (curItem && curItem.classList.contains('a-node')) {
                if (!evt.shiftKey && this.getSelectedItems().length <= 1) {
                    this.clearSelectedItems();
                }
                curItem.classList.add('fv-item-slt');
                // TODO:
                tdom.getSelectedItems().forEach((item) => {
                    const jqItem = $(item);
                    const left = parseInt(jqItem.css('left'));
                    const top = parseInt(jqItem.css('top'));
                    $(document).on('mousemove', (e: any) => {
                        jqItem.css({ top: top + e.pageY - startEvtY, left: left + e.pageX - startEvtX });
                        item.onMoving();
                    });
                    $(document).on('mouseup', () => {
                        $(document).off('mousemove');
                        $(document).off('mousedown');
                        $(document).off('mouseup');
                    });
                });

                return;
            }

            // 清空选择集
            // this.clearSelectedItems();

            // 创建选择框
            sfDiv = $('<div class="selection-frame"></div>');

            // 视图移动事件
            $(document).on('mousemove', (em: MouseEvent) => {
                // 按下shift键就多选
                if (evt.shiftKey == true) {
                    t.append(sfDiv);
                    sfDiv.css('left', startEvtX + 'px');
                    sfDiv.css('top', startEvtY + 'px');

                    // evt.preventDefault();

                    sfX = em.clientX;
                    sfY = em.clientY;
                    sfDiv.css('left', Math.min(sfX, startEvtX) - oldX + 'px');
                    sfDiv.css('top', Math.min(sfY, startEvtY) - oldY + 'px');
                    sfDiv.css('width', Math.abs(sfX - startEvtX) + 'px');
                    sfDiv.css('height', Math.abs(sfY - startEvtY) + 'px');

                    let _l = sfDiv.offset().left,
                        _t = sfDiv.offset().top;

                    let _w = sfDiv.innerWidth(),
                        _h = sfDiv.innerHeight();

                    // FIXME
                    /*for (var i = 0; i < tdom.selectedItems.length; i++) {
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
                                tdom.selectedItems[i].className = 'a-node';
                            }
                        }
                    }*/
                } else {
                    t.css('cursor', 'grabbing');
                    let tLeft = oldX + em.clientX - evt.clientX;
                    let tTop = oldY + em.clientY - evt.clientY;

                    if (tLeft > 0) {
                        t.css('left', '0px');
                    } else if (tLeft < t.parent().innerWidth() - t.innerWidth()) {
                        t.css('left', t.parent().innerWidth() - t.innerWidth());
                    } else {
                        t.css('left', tLeft + 'px');
                    }

                    if (tTop > 0) {
                        t.css('top', '0px');
                    } else if (tTop < t.parent().innerHeight() - t.innerHeight()) {
                        t.css('top', t.parent().innerHeight() - t.innerHeight());
                    } else {
                        t.css('top', tTop + 'px');
                    }
                }
                return false;
            });

            $(document).on('mouseup', function () {
                t.css('cursor', 'grab');

                if (sfDiv) {
                    sfDiv.remove();
                }

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

        this.addCenterGuidLine();
        this.moveToOrigin();
    }

    /**
     * 当鼠标在item上按下时
     * @param {Event} event 点击事件
     */
    private onItemPressed(event: MouseEvent) {}

    /**
     * 当items移动时
     * @param {Event} event 移动事件
     */
    private onItemsMoving(event: MouseEvent) {}

    /**
     * 当框选时
     * @param {Event} event 框选事件
     */
    private onBoxSelect(event: MouseEvent) {}

    /**
     * 当item菜单事件触发时
     * @param {Event} event item菜单事件
     */
    private onItemContextMenu(event: MouseEvent) {}

    /**
     * 当视图菜单事件触发时
     * @param {Event} event 视图菜单事件
     */
    private onContextMenu(event: MouseEvent) {}

    /**
     * 清空选择集
     */
    public clearSelectedItems() {
        $(this).children('.fv-item-slt').removeClass('fv-item-slt');
    }

    /**
     * 获取选择的 item
     */
    public getSelectedItems() {
        return getElementsByClassName<AbstractFlowViewItem>(this, 'fv-item-slt');
    }

    /**
     * 添加一个节点
     * @param {ANode} node 要加入场景的节点
     * @param {number} x 加入时的x坐标
     * @param {number} y 加入时的y坐标
     */
    public addNode(node: ANode, x: number = 0, y: number = 0) {
        const offset = this.getOrigin();

        this.append(node);
        node.setPosition(x + offset.x, y + offset.y);
        addResizeComponent(node, () => {
            node.updateConnectionPosition();
        });
        // this.nodes.push(node);
    }

    /**
     * 获得视图中的所有节点
     */
    public getNodes() {
        return getElementsByClassName<ANode>(this, 'a-node');
    }

    /**
     * 删除一个节点
     * @param {ANode} node 要删除的节点
     */
    deleteNode(node: ANode) {
        if (node) {
            node.remove();
        } else {
            console.log('删除了一个无效的节点');
        }
    }

    addLinkingConnection(sourcePort: APort) {
        let conn = new AConnection(this);

        // 设置起始地固定点
        const p = sourcePort.getPositionInView();
        conn.setStartFixedPoint({
            x: p.x + parseInt(this.style.left),
            y: p.y + parseInt(this.style.top),
        });

        return conn;
    }

    /**
     * 添加中心导航线，如果存在则不做任何动作
     * @note 默认在视图创建时添加上
     * @see removeCenterGuidLine()
     */
    addCenterGuidLine() {
        if ($(this).children('[class^="guid-line"]').length > 0) {
            return;
        }
        $(this).prepend($('<div class="guid-line-h"></div><div class="guid-line-v"></div>'));
    }

    /**
     * 移除中心导航线，如果不存在则不做任何动作
     * @see addCenterGuidLine()
     */
    removeCenterGuidLine() {
        $(this).children('[class^="guid-line"]').remove();
    }

    /**
     * 获取视图中心的位置，及相对于视图左上角的位置
     * @return {Object} 返回偏移坐标，格式：{x, y}
     * @see getOriginOffset()、getViewOffset()
     */
    getOrigin() {
        return {
            x: this.offsetWidth / 2,
            y: this.offsetHeight / 2,
        };
    }

    /**
     * 获得视图中心的偏移位置，即`视图中心`相对于`视口`左上角的偏移
     * @return {Object} 返回偏移坐标，格式：{x, y}
     * @see getOrigin()、getViewOffset()
     */
    getOriginOffset() {
        return {
            x: (this.offsetLeft + this.offsetWidth) / 2,
            y: (this.offsetTop + this.offsetHeight) / 2,
        };
    }

    /**
     * 获取视图的偏移量，即视图左上角到视口左上角的偏移量
     * @return {Object} 返回偏移坐标，格式：{x, y}
     * @note 返回的偏移量都大于等于0
     * @see getOrigin()、getOriginOffset()
     */
    public getViewOffset() {
        return {
            x: -this.offsetLeft,
            y: -this.offsetTop,
        };
    }

    /**
     * 视口坐标转到视图中的坐标
     * @param {Object}} point 视口坐标
     * @return {Object} 返回视图中的坐标，格式：{x, y}
     */
    public viewportToFlowView(point: Point): Point {
        return {
            x: point.x - this.offsetLeft,
            y: point.y - this.offsetTop,
        };
    }

    /**
     * 将视图移动到原点，即视图中心位置在外部框架容器中心处
     */
    public moveToOrigin() {
        let t = $(this);
        const hw = (t.parent().innerWidth() - t.innerWidth()) / 2,
            hh = (t.parent().innerHeight() - t.innerHeight()) / 2;

        t.css('left', hw + 'px').css('top', hh + 'px');
    }

    /**
     * 更新item的选择集合
     * @param {Event} event 事件，一般为鼠标点选节点的事件
     * @param {ANode} curSelectedNode 当前选择的节点
     */
    private _updateSelectionSet(event: any, curSelectedNode: ANode) {
        let node = $(curSelectedNode);
        // 加选
        if (event.ctrlKey == 1 || event.shiftKey == 1) {
            if (node.hasClass('fv-item-slt')) {
                node.removeClass('fv-item-slt');
                return;
            }
        } else {
            $(this).children('.a-node').removeClass('fv-item-slt');
        }
        node.addClass('fv-item-slt');
    }
}
