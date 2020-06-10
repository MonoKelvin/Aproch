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
var __importDefault = (this && this.__importDefault) || function (mod) {
    return (mod && mod.__esModule) ? mod : { "default": mod };
};
Object.defineProperty(exports, "__esModule", { value: true });
var aproch_1 = __importDefault(require("aproch"));
var AConnection = (function (_super) {
    __extends(AConnection, _super);
    function AConnection(flowView, inPort, outPort) {
        if (inPort === void 0) { inPort = null; }
        if (outPort === void 0) { outPort = null; }
        var _this = _super.call(this) || this;
        _this.inPort = null;
        _this.outPort = null;
        _this.inPort = inPort;
        _this.outPort = outPort;
        _this.path = {
            id: 'conn_' + ConnectionIDGenerator++,
            p1y: 0,
            p2y: 0,
            r: { l: 0, t: 0, w: 0, h: 0 },
            color: 'white',
            toString: function () {
                return ('<polyline class="conn-path" style="stroke:' +
                    this.color +
                    '" id="' +
                    this.id +
                    '" points="0,' +
                    this.p1y +
                    ' ' +
                    this.r.w +
                    ',' +
                    this.p2y +
                    '"/>');
            },
        };
        _this.setAttribute('class', 'aproch-conn');
        aproch_1.default(flowView).prepend(_this);
        return _this;
    }
    AConnection.prototype.disconnectedCallback = function () {
        if (this.inPort) {
            this.inPort.connections.remove(this);
        }
        if (this.outPort) {
            this.outPort.connections.remove(this);
        }
        this.path = null;
    };
    AConnection.prototype._update = function () {
        this._setLinkingPoint(this.inPort.getPositionInView(), this.outPort.getPositionInView());
    };
    AConnection.prototype._setLinkingPoint = function (fixed, move) {
        this.path.r.w = Math.abs(fixed.x - move.x);
        this.path.r.h = Math.abs(fixed.y - move.y);
        if (fixed.x < move.x) {
            this.path.r.l = fixed.x;
        }
        else {
            this.path.r.l = move.x;
        }
        if (fixed.y < move.y) {
            this.path.r.t = fixed.y;
            if (fixed.x > move.x) {
                this.path.p1y = this.path.r.h;
                this.path.p2y = 0;
            }
            else {
                this.path.p1y = 0;
                this.path.p2y = this.path.r.h;
            }
        }
        else {
            this.path.r.t = move.y;
            if (fixed.x < move.x) {
                this.path.p1y = this.path.r.h;
                this.path.p2y = 0;
            }
            else {
                this.path.p1y = 0;
                this.path.p2y = this.path.r.h;
            }
        }
        this.style.left = this.path.r.l + 'px';
        this.style.top = this.path.r.t + 'px';
        this.style.width = this.path.r.w + 'px';
        this.style.height = this.path.r.h + 'px';
        this.innerHTML = '<svg xmlns="http://www.w3.org/2000/svg" version="1.1" >' + this.path.toString() + '</svg>';
    };
    return AConnection;
}(HTMLElement));
exports.default = AConnection;
