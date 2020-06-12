import ANode from './ANode';
var ANodeModelRegistry = (function () {
    function ANodeModelRegistry() {
        this.registryTable = {
            Other: [],
        };
    }
    ANodeModelRegistry.prototype.registryDataModel = function (dmName, category) {
        if (category === void 0) { category = 'Other'; }
        if (!dmName || this.registryTable[category].indexOf(dmName) == -1) {
            return false;
        }
        if (this.registryTable[category] == undefined) {
            this.registryTable[category] = new Array();
        }
        this.registryTable[category].push(dmName);
        return true;
    };
    ANodeModelRegistry.prototype.createNode = function (dmName, category, flowView, x, y) {
        if (x === void 0) { x = 0; }
        if (y === void 0) { y = 0; }
        var dm = null;
        this.registryTable[category].forEach(function (i) {
            if (i === dmName) {
                eval('dm = new DM.' + i + '()');
            }
        });
        if (!dm) {
            console.log(category + '.' + dmName + ' 节点不存在！');
            return null;
        }
        var node = new ANode(flowView, dm, x, y);
        $(node).css('width', node.getImplicitWidth() + 22 + 'px');
        return node;
    };
    return ANodeModelRegistry;
}());
export default ANodeModelRegistry;
