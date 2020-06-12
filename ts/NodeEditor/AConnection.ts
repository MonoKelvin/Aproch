import APort from './APort';
import AFlowView from './AFlowView';

/**
 * 连线类
 */
export default class AConnection extends HTMLElement {
    /** 输入端口 */
    public inPort: APort | null = null;

    /** 输出端口 */
    public outPort: APort | null = null;

    /** 连线路径 */
    private _path: any;

    constructor(flowView: AFlowView, inPort: APort | null = null, outPort: APort | null = null) {
        super();

        /** 输入端口，对应节点的输出端口 */
        this.inPort = inPort;

        /** 输出端口，对应节点的输入端口 */
        this.outPort = outPort;

        // this.data = null;

        /** 连线标签 */
        this._path = {
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

        this._path = null;
    }

    public setStartFixedPoint(point: Point) {
        this._path.r.l = point.x;
        this._path.r.t = point.y;
    }

    /**
     * 更新连线位置
     * @node 虽然是共有函数，但尽量不要手动调用该方法
     */
    public _update() {
        this._setLinkingPoint(this.inPort.getPositionInView(), this.outPort.getPositionInView());
    }

    /**
     * 设置正在连接（只有一端是有端口，另一端还没有端口）的连线
     * @param {any} fixed 固定点，一般为鼠标点击时的点
     * @param {any} move 移动点，一般为鼠标移动时的点
     * @node 虽然是共有函数，但尽量不要手动调用该方法
     */
    public _setLinkingPoint(fixed: any, move: any) {
        this._path.r.w = Math.abs(fixed.x - move.x);
        this._path.r.h = Math.abs(fixed.y - move.y);

        if (fixed.x < move.x) {
            this._path.r.l = fixed.x;
        } else {
            this._path.r.l = move.x;
        }
        if (fixed.y < move.y) {
            this._path.r.t = fixed.y;
            if (fixed.x > move.x) {
                this._path.p1y = this._path.r.h;
                this._path.p2y = 0;
            } else {
                this._path.p1y = 0;
                this._path.p2y = this._path.r.h;
            }
        } else {
            this._path.r.t = move.y;
            if (fixed.x < move.x) {
                this._path.p1y = this._path.r.h;
                this._path.p2y = 0;
            } else {
                this._path.p1y = 0;
                this._path.p2y = this._path.r.h;
            }
        }

        this.style.left = this._path.r.l + 'px';
        this.style.top = this._path.r.t + 'px';
        this.style.width = this._path.r.w + 'px';
        this.style.height = this._path.r.h + 'px';
        this.innerHTML = '<svg xmlns="http://www.w3.org/2000/svg" version="1.1" >' + this._path.toString() + '</svg>';
    }
}
