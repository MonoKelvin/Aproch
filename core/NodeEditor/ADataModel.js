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
import { MonoInputNumber } from '../Component/MonoInputWidget';
import MonoLabel from '../Component/MonoLabel';
var AbstractDataModel = (function () {
    function AbstractDataModel(name) {
        this.name = '';
        this.dataWidget = [];
        this.name = name;
    }
    AbstractDataModel.prototype.setDataWidget = function (index, widget) {
        this.dataWidget[index] = widget;
    };
    AbstractDataModel.prototype.getDataWidget = function (index) {
        return this.dataWidget[index];
    };
    return AbstractDataModel;
}());
export default AbstractDataModel;
var DM_SimpleOut = (function (_super) {
    __extends(DM_SimpleOut, _super);
    function DM_SimpleOut(name) {
        var _this = _super.call(this, name) || this;
        _this.dataWidget = [new MonoInputNumber('输出')];
        return _this;
    }
    DM_SimpleOut.prototype.builder = function (index) {
        switch (index) {
            case 0:
                return {
                    ui: this.dataWidget[0],
                    isInPort: false,
                    isOutPort: true,
                };
            default:
                break;
        }
        return null;
    };
    DM_SimpleOut.prototype.calculate = function (index) {
        throw new Error('Method not implemented.');
    };
    DM_SimpleOut.prototype.inputData = function (index, data) {
        throw new Error('Method not implemented.');
    };
    DM_SimpleOut.prototype.outputData = function (index) {
        throw new Error('Method not implemented.');
    };
    return DM_SimpleOut;
}(AbstractDataModel));
export { DM_SimpleOut };
var DM_SimpleIn = (function (_super) {
    __extends(DM_SimpleIn, _super);
    function DM_SimpleIn(name) {
        var _this = _super.call(this, name) || this;
        _this.dataWidget = [new MonoLabel('输入')];
        return _this;
    }
    DM_SimpleIn.prototype.builder = function (index) {
        switch (index) {
            case 0:
                return {
                    ui: this.dataWidget,
                    isInPort: true,
                    isOutPort: false,
                };
            default:
                break;
        }
        return null;
    };
    DM_SimpleIn.prototype.calculate = function (index) {
        throw new Error('Method not implemented.');
    };
    DM_SimpleIn.prototype.inputData = function (index, data) {
        throw new Error('Method not implemented.');
    };
    DM_SimpleIn.prototype.outputData = function (index) {
        throw new Error('Method not implemented.');
    };
    return DM_SimpleIn;
}(AbstractDataModel));
export { DM_SimpleIn };
