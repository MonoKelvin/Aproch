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
import APort from './APort';
var AInterface = (function (_super) {
    __extends(AInterface, _super);
    function AInterface(node, options) {
        var _this = _super.call(this) || this;
        _this._inPort = null;
        _this._outPort = null;
        _this.id = 'itf_' + NodeIDGenerator + '_' + InterfaceIDGenerator++;
        _this.setAttribute('class', 'a-interface');
        node.addInterface(_this);
        _this.setPort(options.isInPort, options.isOutPort);
        _this.append(options.ui);
        return _this;
    }
    AInterface.prototype.disconnectedCallback = function () {
        this.removePort(true, true);
    };
    AInterface.prototype.removePort = function (isInPort, isOutPort) {
        if (isInPort && this._inPort) {
            this._inPort.remove();
            this._inPort = null;
        }
        if (isOutPort && this._outPort) {
            this._outPort.remove();
            this._outPort = null;
        }
    };
    AInterface.prototype.setPort = function (isInPort, isOutPort) {
        if (isInPort && !this._inPort) {
            this._inPort = new APort(0);
            this.append(this._inPort);
            if (!isOutPort) {
                $(this).addClass('interface-in');
            }
        }
        if (isOutPort && !this._outPort) {
            this._outPort = new APort(1);
            this.append(this._outPort);
            if (!isInPort) {
                $(this).addClass('interface-out');
            }
        }
        this.removePort(!isInPort, !isOutPort);
    };
    AInterface.prototype.getPort = function (type) {
        if (type === 0) {
            return this._inPort;
        }
        if (type === 1) {
            return this._outPort;
        }
        return null;
    };
    AInterface.prototype.getNode = function () {
        return this.offsetParent;
    };
    return AInterface;
}(HTMLElement));
export default AInterface;
