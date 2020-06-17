var __extends = (this && this.__extends) || (function () {
    var extendStatics = function (d, b) {
        extendStatics = Object.setPrototypeOf ||
            ({ __proto__: [] } instanceof Array && function (d, b) { d.__proto__ = b; }) ||
            function (d, b) { for (var p in b) if (b.hasOwnProperty(p)) d[p] = b[p]; };
        return extendStatics(d, b);
    };
    return function (d, b) {
        extendStatics(d, b);
        function __() { this.constructor = d; }
        d.prototype = b === null ? Object.create(b) : (__.prototype = b.prototype, new __());
    };
})();
import AbstractWidget from './MonoWidget';
var MonoLabel = (function (_super) {
    __extends(MonoLabel, _super);
    function MonoLabel(label) {
        if (label === void 0) { label = '数据源'; }
        var _this = _super.call(this, label) || this;
        _this.setAttribute('class', 'input-widget-container');
        _this.append(_this.mLabel);
        return _this;
    }
    return MonoLabel;
}(AbstractWidget));
export default MonoLabel;
