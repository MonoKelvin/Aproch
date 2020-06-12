import AFlowView from './AFlowView';
import ANode from './ANode';

export default class ANodeModelRegistry {
    /** 单个对象的内部注册表 */
    public registryTable: CategoryTable<string>;

    constructor() {
        this.registryTable = {
            Other: [],
        };
    }

    /**
     * 注册一个数据模型
     * @param {string} dmName 要注册的数据模型的名字
     * @param {string} category 注册时的分类，如果没有该分类，默认新创建一个
     * @return 注册成功返回true，否则为false
     */
    public registryDataModel(dmName: string, category = 'Other') {
        if (!dmName || this.registryTable[category].indexOf(dmName) == -1) {
            return false;
        }

        if (this.registryTable[category] == undefined) {
            this.registryTable[category] = new Array();
        }

        this.registryTable[category].push(dmName);
        return true;
    }

    /**
     * 创建一个节点到指定视图
     * @param {string} dmName 要创建节点的数据模型名字
     * @param {string} category 在哪一个分类
     * @param {AFlowView} flowView 在哪一个视图中创建
     * @returns {ANode | null} 返回创建好的节点，如果创建失败则返回null
     * @note 函数返回之前就已经添加到视图（如果视图存在）中去了
     */
    public createNode(dmName: string, category: string, flowView: AFlowView, x = 0, y = 0) {
        let dm = null;

        // 找到数据模型
        this.registryTable[category].forEach((i) => {
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
    }
}
