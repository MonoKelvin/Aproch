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
var Aprochh_1 = require("../Aprochh");
var APort = (function (_super) {
    __extends(APort, _super);
    function APort(type) {
        var _this = _super.call(this) || this;
        _this.connectCountLimit = 1;
        _this.connections = [];
        _this.type = type;
        _this.connectCountLimit = type == 0 ? 1 : Infinity;
        _this.id = 'port_' + Aprochh_1.NodeIDGenerator + '_' + Aprochh_1.PortIDGenerator++;
        if (_this.type == 0) {
            _this.setAttribute('class', 'aproch-port-in');
        }
        else {
            _this.setAttribute('class', 'aproch-port-out');
        }
        aproch_1.default(_this).on('mousedown', function (ed) {
            var t = this;
            var tp = t.offsetParent;
            var tar = null;
            var f = t.getPositionInView();
            var conn = null;
            var canLink = false;
            if (t.type == 0 && t.connections.length > 0) {
                conn = t.connections[0];
                f = t.connections[0].inPort.getPositionInView();
            }
            else if (t.getConnectionCount() < t.connectCountLimit) {
                conn = tp.getFlowView().addLinkingConnection(t);
            }
            else {
                t.getNode().pushPromptText({
                    content: '连线数量已达上限：' + t.connectCountLimit,
                    type: 'alert',
                    delay: 'long',
                });
                return;
            }
            aproch_1.default(document).on('mousemove', function (em) {
                var fvOffset = t.getNode().getFlowView().viewportToFlowView({ x: em.clientX, y: em.clientY });
                conn._setLinkingPoint(f, fvOffset);
                tar = em.target;
                canLink = false;
                if (tar instanceof APort && tar !== ed.target) {
                    if (APort.CanLink(t, tar)) {
                        var p = tar.getPositionInView();
                        conn._setLinkingPoint(f, p);
                        canLink = true;
                    }
                }
            });
            aproch_1.default(document).on('mouseup', function (eu) {
                if (!canLink) {
                    conn.remove();
                }
                else {
                    t.attachConnection(conn);
                    tar.attachConnection(conn);
                }
                aproch_1.default(document).off('mousemove');
                aproch_1.default(document).off('mouseup');
            });
        });
        return _this;
    }
    APort.prototype.disconnectedCallback = function () {
        var _this = this;
        aproch_1.default(this.connections).each(function (_, conn) {
            _this.detachConnection(conn);
        });
    };
    APort.prototype.getOppositePort = function () {
        var ports = [];
        if (this.type === 0) {
            this.connections.forEach(function (c) {
                ports.push(c.inPort);
            });
        }
        else {
            this.connections.forEach(function (c) {
                ports.push(c.outPort);
            });
        }
        return ports;
    };
    APort.prototype.getInterface = function () {
        return this.parentNode;
    };
    APort.prototype.attachConnection = function (conn) {
        this.connections.push(conn);
        if (this.type === 0) {
            conn.outPort = this;
        }
        else {
            conn.inPort = this;
        }
    };
    APort.prototype.detachConnection = function (conn) {
        conn.remove();
    };
    APort.prototype.getNode = function () {
        return this.offsetParent;
    };
    APort.prototype.getConnectionCount = function () {
        return this.connections.length;
    };
    APort.prototype.getPositionInView = function () {
        var t = aproch_1.default(this);
        var p = { x: t.offset().left + this.offsetWidth / 2, y: t.offset().top + this.offsetHeight / 2 };
        return this.getNode().getFlowView().viewportToFlowView(p);
    };
    APort.CanLink = function (p1, p2, tv) {
        if (tv === void 0) { tv = null; }
        if (p1.getNode() !== p2.getNode() &&
            p1.type !== p2.type &&
            p1.getOppositePort()[0] !== p2 &&
            p1.getConnectionCount() < p1.connectCountLimit &&
            p2.getConnectionCount() < p2.connectCountLimit) {
            if (new ABaseTypeConverter().canConvert(p1.getInterface(), p2.getInterface())) {
                return true;
            }
            else if (tv && tv.CanConvert()) {
                return true;
            }
        }
        return false;
    };
    return APort;
}(HTMLElement));
exports.default = APort;
