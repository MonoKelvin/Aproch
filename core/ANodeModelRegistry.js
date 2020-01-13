import { ANode } from './Aproch.js';
import * as DM from '../core/DataModel.js';

export default class ANodeModelRegistry {
    constructor() {
        this.registryTable = {
            Other: new Set()
        };
    }

    /**
     * 注册一个数据模型
     * @param {string} dmClassName 要注册的类名
     * @param {string} category 注册时的分类，如果没有该分类，默认新创建一个
     * @returns 注册成功返回true，否则为false
     * @note 如果表中类名已存在，则会被覆盖
     */
    registryDataModel(dmClassName, category = 'other') {
        if (!dmClassName) {
            return false;
        }

        if (this.registryTable[category] === undefined) {
            this.registryTable[category] = new Set();
        }

        this.registryTable[category].add(dmClassName);
        return true;
    }

    /**
     * 创建一个节点到指定视图
     * @param {string} name 要创建节点的名字
     * @param {string} category 在哪一个分类
     * @param {AFlowView} flowView 在哪一个视图中创建
     * @returns {ANode | null} 返回创建好的节点，如果创建失败则返回null
     * @note 函数返回之前就已经添加到视图（如果视图存在）中去了
     */
    createNode(name, category, flowView, x = 0, y = 0) {
        let dm = null;

        // 找到数据模型
        this.registryTable[category].forEach((i, _, __) => {
            if (i === name) {
                eval('dm = new DM.' + i + '()');
            }
        });

        if (!dm) {
            console.log(category + '.' + name + ' 节点不存在！');
            return null;
        }

        return new ANode(flowView, dm, x, y);
    }
}
