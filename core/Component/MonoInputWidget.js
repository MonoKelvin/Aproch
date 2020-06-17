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
var MonoInput = (function (_super) {
    __extends(MonoInput, _super);
    function MonoInput(label, defaultValue) {
        if (defaultValue === void 0) { defaultValue = ''; }
        var _this = _super.call(this, label) || this;
        _this.type = 6;
        _this.placeholder = '';
        _this.mDefaultValue = defaultValue;
        _this.setAttribute('class', 'input-widget-container');
        _this.widget = document.createElement('input');
        _this.widget.setAttribute('class', 'input-widget');
        _this.widget.defaultValue = defaultValue;
        return _this;
    }
    MonoInput.prototype.onComplete = function () {
        throw new Error('Method not implemented.');
    };
    MonoInput.prototype.onValueChanged = function () {
        throw new Error('Method not implemented.');
    };
    MonoInput.prototype.validate = function () {
        throw new Error('Method not implemented.');
    };
    return MonoInput;
}(AbstractWidget));
export default MonoInput;
var MonoInputNumber = (function (_super) {
    __extends(MonoInputNumber, _super);
    function MonoInputNumber(label, defValue, minValue, maxValue) {
        if (defValue === void 0) { defValue = 0; }
        if (minValue === void 0) { minValue = -Number.MAX_VALUE; }
        if (maxValue === void 0) { maxValue = Number.MAX_VALUE; }
        var _this = _super.call(this, label, defValue) || this;
        _this.widget.min = minValue;
        _this.widget.max = maxValue;
        _this.widget.value = 0;
        _this.widget.type = 'number';
        _this.widget.oninput = function () {
            this.value = this.value.replace(/[^\-?\d.]/g, '');
        };
        _this.addEventListener('change', function () {
            if (_this.widget.value.trim() === '') {
                _this.widget.value = 0;
            }
            _this.onComplete();
            _this.widget.blur();
        });
        _this.append(_this.label);
        _this.append(_this.widget);
        return _this;
    }
    return MonoInputNumber;
}(MonoInput));
export { MonoInputNumber };
