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
var AFlowView_1 = __importDefault(require("./AFlowView"));
var ANode = (function (_super) {
    __extends(ANode, _super);
    function ANode(flowView, dataModel, x, y) {
        if (x === void 0) { x = 0; }
        if (y === void 0) { y = 0; }
        var _this = _super.call(this) || this;
        _this.nodeTitle = document.createElement('div');
        _this.nodeContent = document.createElement('div');
        _this.dataModel = dataModel;
        _this.setAttribute('class', 'aproch-node');
        _this.nodeTitle.setAttribute('class', 'node-title');
        _this.nodeTitle.innerHTML = dataModel.name;
        _this.nodeContent.setAttribute('class', 'node-content');
        _this.nodeContent.onmousedown = function (e) {
            e.stopPropagation();
        };
        _this.id = 'node_' + NodeIDGenerator++;
        PortIDGenerator = 0;
        _this.append(_this.nodeTitle);
        _this.append(_this.nodeContent);
        for (var i = 0; i < MAX_INTERFACE_COUNTER; i++) {
            var itfOption = _this.dataModel.uiBuilder(i);
            if (!itfOption) {
                break;
            }
            new AInterface(_this, itfOption);
        }
        flowView.addNode(_this, x, y);
        return _this;
    }
    ANode.GetNodeByChildComponent = function (childComponent) {
        var curElement = childComponent;
        while (curElement != document) {
            if (curElement.classList.contains('aproch-node')) {
                return curElement;
            }
            curElement = curElement.parentNode;
        }
        return null;
    };
    ANode.prototype.disconnectedCallback = function () {
        this._propagationData();
    };
    ANode.prototype._propagationData = function () {
    };
    ANode.prototype.updateConnectionPosition = function () {
        this.getInterfaces().forEach(function (i) {
            if (i.getPort(EPortType.INPUT)) {
                i.getPort(EPortType.INPUT).connections.forEach(function (conn) {
                    conn._update();
                });
            }
            if (i.getPort(EPortType.OUTPUT)) {
                i.getPort(EPortType.OUTPUT).connections.forEach(function (conn) {
                    conn._update();
                });
            }
        });
    };
    ANode.prototype.setTitle = function (name) {
        this.nodeTitle.innerHTML = name;
    };
    ANode.prototype.getTitle = function () {
        return this.nodeTitle.innerHTML;
    };
    ANode.prototype.setTitleColor = function (color) {
        this.nodeTitle.style.background = color;
    };
    ANode.prototype.getPosition = function () {
        return {
            x: parseInt(this.style.left),
            y: parseInt(this.style.top),
        };
    };
    ANode.prototype.setPosition = function (x, y) {
        this.style.left = x + 'px';
        this.style.top = y + 'px';
    };
    ANode.prototype.addPosition = function (dx, dy) {
        this.style.left = parseInt(this.style.left) + dx + 'px';
        this.style.top = parseInt(this.style.top) + dy + 'px';
    };
    ANode.prototype.addInterface = function (itf) {
        try {
            this.nodeContent.childNodes.forEach(function (i) {
                if (i === itf) {
                    throw new Error('接口已经存在，无法重复添加。interface:', itf);
                }
            });
            this.nodeContent.appendChild(itf);
        }
        catch (e) {
            console.log(e);
        }
    };
    ANode.prototype.getInterfaces = function () {
        var interfaces = [];
        this.nodeContent.childNodes.forEach(function (i) {
            if (i instanceof AInterface) {
                interfaces.push(i);
            }
        });
        return interfaces;
    };
    ANode.prototype.getFlowView = function () {
        var fv = null;
        $.each($(this).parents(), function (_, p) {
            if (p instanceof AFlowView_1.default) {
                fv = p;
                return;
            }
        });
        return fv;
    };
    ANode.prototype.pushPromptText = function (option) {
        if (typeof option === 'object') {
            var prompt_1 = $('<div class="node-prompt"></div>');
            prompt_1.html(option.content);
            prompt_1.prepend($('<i class="fa fa-exclamation-triangle" aria-hidden="true"></i>'));
            if (typeof option.type == 'string') {
                prompt_1.addClass('bg-' + option.type);
            }
            var timeout = 1500;
            if (option.delay === 'long') {
                timeout = 3000;
            }
            else if (isNaN(parseInt(option.delay))) {
                timeout = 1500;
            }
            setTimeout(function () {
                prompt_1.remove();
                prompt_1 = null;
            }, timeout);
            this.nodeContent.appendChild(prompt_1[0]);
        }
        return this;
    };
    return ANode;
}(HTMLElement));
exports.default = ANode;
