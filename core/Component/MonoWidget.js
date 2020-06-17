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
var AbstractWidget = (function (_super) {
    __extends(AbstractWidget, _super);
    function AbstractWidget(name) {
        var _this = _super.call(this) || this;
        _this.mLabel = document.createElement('span');
        _this.toolTip = '';
        _this.widget = null;
        _this.mLabel.innerHTML = name;
        _this.mLabel.classList.add('node-label');
        return _this;
    }
    Object.defineProperty(AbstractWidget.prototype, "label", {
        get: function () {
            return this.mLabel.innerHTML;
        },
        set: function (name) {
            this.mLabel.innerHTML = name;
        },
        enumerable: false,
        configurable: true
    });
    return AbstractWidget;
}(HTMLElement));
export default AbstractWidget;
