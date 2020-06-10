"use strict";
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
Object.defineProperty(exports, "__esModule", { value: true });
exports.AInputNumberWidget = exports.ALabelWidget = exports.AbstractWidget = exports.EInputType = exports.EWidgetType = void 0;
exports.EWidgetType = {
    Label: 'label',
    Input: 'input',
    Check: 'check',
    Vector: 'vector',
    Matrix: 'matrix',
    Image: 'image',
    File: 'file'
};
exports.EInputType = {
    Int: 'int',
    Float: 'float',
    Number: 'number',
    String: 'string',
    Text: 'text',
    Bool: 'bool'
};
var AbstractWidget = (function (_super) {
    __extends(AbstractWidget, _super);
    function AbstractWidget() {
        var _this = _super.call(this) || this;
        _this.label = document.createElement('span');
        _this.widget = null;
        return _this;
    }
    AbstractWidget.prototype.setLabel = function (content, associateControl) {
        if (associateControl === void 0) { associateControl = true; }
        this.label.innerHTML = content;
        if (associateControl === true) {
            this.label.setAttribute('class', 'control-label');
        }
        else {
            this.label.setAttribute('class', 'node-label');
        }
    };
    AbstractWidget.prototype.destroyWidgets = function () {
        this.label = null;
        delete this.label;
        this.widget = null;
        delete this.widget;
    };
    return AbstractWidget;
}(HTMLElement));
exports.AbstractWidget = AbstractWidget;
var ALabelWidget = (function (_super) {
    __extends(ALabelWidget, _super);
    function ALabelWidget(label) {
        if (label === void 0) { label = '数据源'; }
        var _this = _super.call(this) || this;
        _this.setAttribute('class', 'input-widget-container');
        _this.setLabel(label, false);
        _this.append(_this.label);
        return _this;
    }
    ALabelWidget.prototype.disconnectedCallback = function () {
        this.destroyWidgets();
    };
    return ALabelWidget;
}(AbstractWidget));
exports.ALabelWidget = ALabelWidget;
var AInputNumberWidget = (function (_super) {
    __extends(AInputNumberWidget, _super);
    function AInputNumberWidget(label, defValue, minValue, maxValue) {
        if (defValue === void 0) { defValue = 0; }
        if (minValue === void 0) { minValue = -Number.MAX_VALUE; }
        if (maxValue === void 0) { maxValue = Number.MAX_VALUE; }
        var _this = _super.call(this) || this;
        _this.onComplete = function () { };
        _this.setAttribute('class', 'input-widget-container');
        _this.setLabel(label);
        _this.widget = document.createElement('input');
        _this.widget.setAttribute('class', 'input-widget');
        _this.widget.value = 0;
        _this.widget.min = minValue;
        _this.widget.max = maxValue;
        _this.widget.defaultValue = defValue;
        _this.widget.type = 'number';
        _this.widget.oninput = function () {
            this.value = this.value.replace(/[^\-?\d.]/g, '');
        };
        _this.addEventListener('change', function () {
            if (this.widget.value.trim() === '') {
                this.widget.value = 0;
            }
            this.onComplete();
            this.widget.blur();
        });
        _this.append(_this.label);
        _this.append(_this.widget);
        return _this;
    }
    AInputNumberWidget.prototype.disconnectedCallback = function () {
        this.destroyWidgets();
    };
    AInputNumberWidget.prototype.attributeChangedCallback = function () {
        console.log('this.widget.value :', this.widget.value);
    };
    return AInputNumberWidget;
}(AbstractWidget));
exports.AInputNumberWidget = AInputNumberWidget;
