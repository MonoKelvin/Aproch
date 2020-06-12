import { addMoveComponent } from '../Utilities';
var MonoFloatPanel = (function () {
    function MonoFloatPanel(options) {
        this._canMove = true;
        this._position = 4 | 8;
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
    MonoFloatPanel.prototype.setPosition = function (position) {
        if (!position) {
            return;
        }
        var t = $(this._ele);
        var area = $(document.documentElement);
        if (position & 1) {
            t.css('left', '0px');
        }
        if (position & 4) {
            t.css('left', area.innerWidth() - t.innerWidth() + 'px');
        }
        if (position & 2) {
            t.css('top', '0px');
        }
        if (position & 8) {
            t.css('top', area.innerHeight() - t.innerHeight() + 'px');
        }
        if (position & 16) {
            t.css('left', (area.innerWidth() - t.innerWidth()) / 2 + 'px');
        }
        if (position & 32) {
            t.css('top', (area.innerHeight() - t.innerHeight()) / 2 + 'px');
        }
    };
    return MonoFloatPanel;
}());
export default MonoFloatPanel;
