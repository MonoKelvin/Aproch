"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
var ANodeModelRegistry = (function () {
    function ANodeModelRegistry() {
        this.registryTable = {
            Other: new Set(),
        };
    }
    ANodeModelRegistry.prototype.registryDataModel = function (dmClassName, category) {
        if (category === void 0) { category = 'other'; }
        if (!dmClassName) {
            return false;
        }
        if (this.registryTable[category] === undefined) {
            this.registryTable[category] = new Set();
        }
        this.registryTable[category].add(dmClassName);
        return true;
    };
    ANodeModelRegistry.prototype.createNode = function (name, category, flowView, x, y) {
        if (x === void 0) { x = 0; }
        if (y === void 0) { y = 0; }
        var dm = null;
        this.registryTable[category].forEach(function (i) {
            if (i === name) {
                eval('dm = new DM.' + i + '()');
            }
        });
        if (!dm) {
            console.log(category + '.' + name + ' 节点不存在！');
            return null;
        }
        var node = new ANode(flowView, dm, x, y);
        var rw = node.nodeTitle.innerText.realTextWidth($(node.nodeTitle).css('font'));
        $(node).css('width', rw + 22 + 'px');
        return node;
    };
    return ANodeModelRegistry;
}());
exports.default = ANodeModelRegistry;
