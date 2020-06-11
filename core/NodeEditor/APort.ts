/// <reference path="../Aproch.d.ts" />
/// <reference path="../../lib/jquery/jquery-3.4.1.min.js" />

import AConnection from './AConnection';
import ANode from './ANode';

export const enum EPortType {
    INPUT,
    OUTPUT,
}

export default class APort extends HTMLElement {
    type: EPortType;

    connectCountLimit: number = 1;

    connections: AConnection[] = [];

    /**
     * 创建一个端口
     * @param {EPortType} type 端口类型
     */
    constructor(type: EPortType) {
        super();

        /** 端口类型，只有输入和输出，其具体可接纳的数据类型由接口 @see `AInterface` 控制 */
        this.type = type;

        /** 端口连接连线的数量限制，在此值范围内该端口才能进行连线。
         * @note 默认输出端口最多连接`无数条`，输入端口最多连接`1条`
         */
        this.connectCountLimit = type == EPortType.INPUT ? 1 : Infinity;

        this.id = 'port_' + NodeIDGenerator + '_' + PortIDGenerator++;

        if (this.type == EPortType.INPUT) {
            this.setAttribute('class', 'aproch-port-in');
        } else {
            this.setAttribute('class', 'aproch-port-out');
        }

        // 点击时创建连线
        $(this).on('mousedown', (ed: any) => {
            const t = this;
            const tp = t.getNode();
            let tar: any = null;
            let f = t.getPositionInView();
            let conn: AConnection | null = null;
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
                const fvOffset = t.getNode().getFlowView().viewportToFlowView({ x: em.clientX, y: em.clientY });
                conn._setLinkingPoint(f, fvOffset);

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

            $(document).on('mouseup', function (eu: any) {
                if (!canLink) {
                    conn?.remove();
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

    protected disconnectedCallback() {
        $(this.connections).each((_: any, conn: AConnection) => {
            this.detachConnection(conn);
        });
    }

    /**
     * 获取连接的对立端口
     * @returns {AConnection[]} 如果是输入端口，那返回结果只有一条连线，否则可能有多条
     */
    public getOppositePort() {
        let ports: APort[] = [];
        if (this.type === EPortType.INPUT) {
            this.connections.forEach((c) => {
                ports.push(c.inPort);
            });
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
    public getInterface() {
        return this.parentNode;
    }

    /**
     * 附加连线到该端口上
     * @param {AConnection} conn 要附加的连线
     */
    public attachConnection(conn: AConnection) {
        this.connections.push(conn);
        if (this.type === EPortType.INPUT) {
            conn.outPort = this;
        } else {
            conn.inPort = this;
        }
    }

    /**
     * 分离该端口上指定的连线，同时也会分离与该连线链接的另一头的端口
     * @param {AConnection} conn 链接该端口上的连线
     */
    public detachConnection(conn: AConnection) {
        conn.remove();

        // todo: 更新数据
    }

    /**
     * 获得端口所在的节点
     */
    public getNode(): ANode {
        return this.offsetParent as ANode;
    }

    /**
     * 获得端口上连线的数量
     */
    public getConnectionCount() {
        return this.connections.length;
    }

    /**
     * 获得端口在视图中的位置
     * @returns 返回坐标对象{x, y}
     */
    public getPositionInView() {
        let t = $(this);
        const p = { x: t.offset().left + this.offsetWidth / 2, y: t.offset().top + this.offsetHeight / 2 };
        return this.getNode().getFlowView()?.viewportToFlowView(p);
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
            if (new ABaseTypeConverter().canConvert(p1.getInterface(), p2.getInterface())) {
                return true;
            } else if (tv && tv.CanConvert()) {
                return true;
            }
        }

        return false;
    }
}
