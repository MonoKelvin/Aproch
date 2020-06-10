"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
exports.InDataModel = exports.OutDataModel = void 0;
var OutDataModel = (function () {
    function OutDataModel() {
        this.inputWidget = new AInputNumberWidget('输出');
        this.name = 'Output Model';
    }
    OutDataModel.prototype.builder = function (index) {
        switch (index) {
            case 0:
                return {
                    ui: this.inputWidget,
                    isInPort: false,
                    isOutPort: true,
                };
            default:
                break;
        }
        return null;
    };
    OutDataModel.prototype.calculate = function (index) {
        throw new Error('Method not implemented.');
    };
    OutDataModel.prototype.inputData = function (index, data) {
        throw new Error('Method not implemented.');
    };
    OutDataModel.prototype.outputData = function (index) {
        throw new Error('Method not implemented.');
    };
    return OutDataModel;
}());
exports.OutDataModel = OutDataModel;
var InDataModel = (function () {
    function InDataModel() {
        this.label = new ALabelWidget('输入');
        this.name = 'Input Model';
    }
    InDataModel.prototype.builder = function (index) {
        switch (index) {
            case 0:
                return {
                    ui: this.label,
                    isInPort: true,
                    isOutPort: false,
                };
            default:
                break;
        }
        return null;
    };
    InDataModel.prototype.calculate = function (index) {
        throw new Error('Method not implemented.');
    };
    InDataModel.prototype.inputData = function (index, data) {
        throw new Error('Method not implemented.');
    };
    InDataModel.prototype.outputData = function (index) {
        throw new Error('Method not implemented.');
    };
    return InDataModel;
}());
exports.InDataModel = InDataModel;
