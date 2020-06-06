import { EPosition } from '../Types.js';
import { addMoveComponent } from '../utilities.js';

export class MonoFloatPanel {
    constructor(options) {
        /** 所指向的js元素对象 */
        this.ele = document.querySelector(options.ele);
        if (this.ele == undefined || this.ele == null) {
            return;
        }

        /** 是否能够移动 */
        this.canMove = options && options.canMove != undefined ? Boolean(options.canMove) : true;

        /**  默认浮动位置 */
        this.position =
            options && options.position != undefined ? parseInt(options.position) : EPosition.RIGHT | EPosition.BOTTOM;

        if (this.canMove) {
            addMoveComponent(this.ele, document.documentElement);
        }

        this.setPosition(this.position);
    }

    /**
     * 设置浮动面板的位置
     * @param {EPosition} position 位置
     */
    setPosition(position) {
        let t = $(this.ele);
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
