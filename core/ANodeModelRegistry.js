import { ANode, AInterface } from './Aproch';
import { IDataModel } from './DataModel';

export default class ANodeModelRegistry {
    constructor() {
        this.registryTable = {
            other: new Set()
        };
    }

    /**
     * 注册一个数据模型
     * @param {string} dmClassName 要注册的类名
     * @param {string} category 注册时的分类
     * @returns 返回注册后的状态，当返回：
     * 1：注册成功；
     * 2：重复注册，不覆盖；
     * -1：类名为空，不允许注册空类型；
     */
    registryDataModel(dmClassName, category = 'other') {
        if (!dmClassName) {
            return -1;
        }

        if (this.registryTable[category].has(dmClassName)) {
            return 2;
        } else {
            this.registryTable[category].add(dmClassName);
        }

        return 1;
    }

    /**
     * 创建一个节点到指定视图
     * @param {string} name 要创建节点的名字
     * @param {string} category 在哪一个分类
     * @param {AFlowView} flowView 在哪一个视图中创建
     * @returns {ANode | null} 返回创建好的节点，如果创建失败则返回null
     * @note 函数返回之前就已经添加到视图（如果视图存在）中去了
     */
    createNode(name, category, flowView) {
        let dm = null;

        // 找到数据模型
        this.registryTable[category].forEach((i, _, __) => {
            if (i.name === name) {
                eval('dm = new ' + i + '()');
            }
        });

        if (!dm) {
            console.log(category + '.' + name + ' 节点不存在！');
            return null;
        }

        let itfOption = null;
        let node = new ANode(flowView, dm);

        for (let i = 0; ; i++) {
            itfOption = dm.uiBuilder(i++);
            if (itfOption === null) {
                break;
            }
            new AInterface(node, itfOption);
        }

        return node;
    }
}
