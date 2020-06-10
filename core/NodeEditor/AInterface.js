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
var Aprochh_1 = require("../Aprochh");
var MAX_INTERFACE_COUNTER = 100;
var AInterface = (function (_super) {
    __extends(AInterface, _super);
    function AInterface(node, options) {
        var _this = _super.call(this) || this;
        _this.inPort = null;
        _this.outPort = null;
        _this.id = 'itf_' + Aprochh_1.NodeIDGenerator + '_' + Aprochh_1.InterfaceIDGenerator++;
        _this.setAttribute('class', 'aproch-interface');
        node.addInterface(_this);
        _this.setPort(options.isInPort, options.isOutPort);
        _this.append(options.ui);
        return _this;
    }
    AInterface.prototype.disconnectedCallback = function () {
        this.removePort(true, true);
    };
    AInterface.prototype.removePort = function (isInPort, isOutPort) {
        if (isInPort && this.inPort) {
            this.inPort.remove();
            this.inPort = null;
        }
        if (isOutPort && this.outPort) {
            this.outPort.remove();
            this.outPort = null;
        }
    };
    AInterface.prototype.setPort = function (isInPort, isOutPort) {
        if (isInPort && !this.inPort) {
            this.inPort = new APort(EPortType.INPUT);
            this.append(this.inPort);
            if (!isOutPort) {
                $(this).addClass('interface-in');
            }
        }
        if (isOutPort && !this.outPort) {
            this.outPort = new APort(EPortType.OUTPUT);
            this.append(this.outPort);
            if (!isInPort) {
                $(this).addClass('interface-out');
            }
        }
        this.removePort(!isInPort, !isOutPort);
    };
    AInterface.prototype.getPort = function (type) {
        if (type === EPortType.INPUT) {
            return this.inPort;
        }
        if (type === EPortType.OUTPUT) {
            return this.outPort;
        }
        return null;
    };
    AInterface.prototype.getNode = function () {
        return this.offsetParent;
    };
    return AInterface;
}(HTMLElement));
exports.default = AInterface;
