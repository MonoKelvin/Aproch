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
import AFlowView from './AFlowView';
import AInterface from './AInterface';
import { AbstractFlowViewItem } from './AFlowViewItem';
var ANode = (function (_super) {
    __extends(ANode, _super);
    function ANode(flowView, dataModel, x, y) {
        if (x === void 0) { x = 0; }
        if (y === void 0) { y = 0; }
        var _this = _super.call(this, flowView) || this;
        _this._nodeTitle = document.createElement('div');
        _this._nodeContent = document.createElement('div');
        _this.dataModel = dataModel;
        _this.setAttribute('class', 'a-node');
        _this._nodeTitle.setAttribute('class', 'node-title');
        _this._nodeTitle.innerHTML = dataModel.name;
        _this._nodeContent.setAttribute('class', 'node-content');
        _this._nodeContent.onmousedown = function (e) {
            e.stopPropagation();
        };
        _this.id = 'node_' + NodeIDGenerator++;
        PortIDGenerator = 0;
        _this.append(_this._nodeTitle);
        _this.append(_this._nodeContent);
        for (var i = 0; i < MAX_INTERFACE_COUNTER; i++) {
            var itfOption = _this.dataModel.builder(i);
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
            if (curElement.classList.contains('a-node')) {
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
    ANode.prototype.onSelected = function () {
        throw new Error('Method not implemented.');
    };
    ANode.prototype.onDeselected = function () {
        throw new Error('Method not implemented.');
    };
    ANode.prototype.onHovered = function () {
        throw new Error('Method not implemented.');
    };
    ANode.prototype.onResize = function () {
        throw new Error('Method not implemented.');
    };
    ANode.prototype.builder = function () {
        throw new Error('Method not implemented.');
    };
    ANode.prototype.onMoving = function (event) {
        this.updateConnectionPosition();
    };
    ANode.prototype.getImplicitWidth = function () {
        return this._nodeTitle.innerText.realTextWidth($(this._nodeTitle).css('font'));
    };
    ANode.prototype.updateConnectionPosition = function () {
        this.getInterfaces().forEach(function (i) {
            var _a, _b;
            if (i.getPort(0)) {
                (_a = i.getPort(0)) === null || _a === void 0 ? void 0 : _a.connections.forEach(function (conn) {
                    conn._update();
                });
            }
            if (i.getPort(1)) {
                (_b = i.getPort(1)) === null || _b === void 0 ? void 0 : _b.connections.forEach(function (conn) {
                    conn._update();
                });
            }
        });
    };
    ANode.prototype.setTitle = function (name) {
        this._nodeTitle.innerHTML = name;
    };
    ANode.prototype.getTitle = function () {
        return this._nodeTitle.innerHTML;
    };
    ANode.prototype.setTitleColor = function (color) {
        this._nodeTitle.style.backgroundColor = color;
    };
    ANode.prototype.getTitleColor = function () {
        return this._nodeTitle.style.backgroundColor;
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
            this._nodeContent.childNodes.forEach(function (i) {
                if (i === itf) {
                    throw new Error('接口已经存在，无法重复添加！');
                }
            });
            this._nodeContent.appendChild(itf);
        }
        catch (e) {
            console.log(e);
        }
    };
    ANode.prototype.getInterfaces = function () {
        var interfaces = [];
        this._nodeContent.childNodes.forEach(function (i) {
            if (i instanceof AInterface) {
                interfaces.push(i);
            }
        });
        return interfaces;
    };
    ANode.prototype.getFlowView = function () {
        var fv = null;
        $.each($(this).parents(), function (_, p) {
            if (p instanceof AFlowView) {
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
            this._nodeContent.appendChild(prompt_1[0]);
        }
        return this;
    };
    return ANode;
}(AbstractFlowViewItem));
export default ANode;
