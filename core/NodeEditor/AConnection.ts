/// <reference path="../Types.d.ts" />
import $ from 'aproch';
import APort from './APort';
import AFlowView from './AFlowView';

/**
 * 连线类
 */
export default class AConnection extends HTMLElement {
    inPort: APort = null;

    outPort: APort = null;

    path: any;

    constructor(flowView: AFlowView, inPort: APort = null, outPort: APort = null) {
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
            this.inPort.connections.remove(this);
        }
        if (this.outPort) {
            this.outPort.connections.remove(this);
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
