import { addMoveComponent } from '../Utilities';

export interface IFloatPanelOption {
    /** 元素标识，可以是`#+id`或者`.+class`的形式 */
    ele: string;

    /** 是否能够移动 */
    canMove?: boolean;

    /**  默认浮动位置 */
    position?: EPosition;

    /** 移动回调函数 */
    onMoving(): void;
}

export default class MonoFloatPanel {
    private readonly _ele: HTMLElement | null;

    private readonly _canMove?: boolean = true;

    private _position?: EPosition = EPosition.RIGHT | EPosition.BOTTOM;

    constructor(options: IFloatPanelOption) {
        /** 所指向的js元素对象 */
        this._ele = document.querySelector(options.ele);
        if (!this._ele) {
            return;
        }

        this._canMove = options.canMove;
        this._position = options.position;

        if (this._canMove) {
            addMoveComponent(this._ele, document.documentElement, options.onMoving);
        }

        this.setPosition(this._position);
    }

    /**
     * 设置浮动面板的位置
     * @param {EPosition} position 位置
     */
    setPosition(position?: EPosition) {
        if (!position) {
            return;
        }

        let t = $(this._ele);
        let area = $(document.documentElement);
        if (position & EPosition.LEFT) {
            t.css('left', '0px');
        }
        if (position & EPosition.RIGHT) {
            t.css('left', area.innerWidth() - t.innerWidth() + 'px');
        }
        if (position & EPosition.TOP) {
            t.css('top', '0px');
        }
        if (position & EPosition.BOTTOM) {
            t.css('top', area.innerHeight() - t.innerHeight() + 'px');
        }
        if (position & EPosition.HCENTER) {
            t.css('left', (area.innerWidth() - t.innerWidth()) / 2 + 'px');
        }
        if (position & EPosition.VCENTER) {
            t.css('top', (area.innerHeight() - t.innerHeight()) / 2 + 'px');
        }
    }
}
