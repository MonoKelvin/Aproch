import { EPosition } from '../Types.js';
import { attachMoveComponent } from '../utilities.js';

export class MonoFloatPanel {
    constructor(options) {
        let t = document.querySelector(options.ele);
        let jqThis = $(t);
        if (jqThis.length == 0) {
            return;
        }

        /** 是否能够移动 */
        this.canMove = options && options.canMove !== undefined ? Boolean(options.canMove) : true;

        /**  默认浮动位置 */
        this.position = EPosition.RIGHT | EPosition.BOTTOM;

        if (this.canMove) {
            attachMoveComponent(t, document.documentElement);
        }
    }
}
