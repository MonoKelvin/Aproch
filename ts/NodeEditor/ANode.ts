import AFlowView from './AFlowView';
import IDataModel from './ADataModel';
import AInterface from './AInterface';
import AConnection from './AConnection';
import { EPortType } from './APort';
import { AbstractFlowViewItem } from './AFlowViewItem';

export default class ANode extends AbstractFlowViewItem {
    /**
     * 通过子组件获取父节点
     * @param {Object} childComponent 节点子组件
     */
    public static GetNodeByChildComponent(childComponent: any): ANode | null {
        let curElement = childComponent;
        while (curElement != document) {
            if (curElement.classList.contains('a-node')) {
                return curElement as ANode;
            }
            curElement = curElement.parentNode;
        }

        return null;
    }

    /**
     * 节点标题元素
     */
    private _nodeTitle: HTMLElement;

    /**
     * 节点内容元素
     */
    private _nodeContent: HTMLElement;

    /**
     * 数据模型
     */
    public dataModel: IDataModel;

    constructor(flowView: AFlowView, dataModel: IDataModel, x: number = 0, y: number = 0) {
        super(flowView);

        /** UUID，标识每一个节点 */
        // this.uuid = getUUID();

        /** 节点头部元素 */
        this._nodeTitle = document.createElement('div');

        /** 节点内容，放置所有接口的容器 */
        this._nodeContent = document.createElement('div');

        /** 管理的数据模型 */
        this.dataModel = dataModel;

        // 初始化节点控件
        this.setAttribute('class', 'a-node');
        this._nodeTitle.setAttribute('class', 'node-title');
        this._nodeTitle.innerHTML = dataModel.name;
        this._nodeContent.setAttribute('class', 'node-content');
        this._nodeContent.onmousedown = (e) => {
            e.stopPropagation();
        };
        this.id = 'node_' + NodeIDGenerator++;

        // 端口ID清零
        PortIDGenerator = 0;

        // 添加为子组件
        this.append(this._nodeTitle);
        this.append(this._nodeContent);

        // 初始化数据模型，添加接口
        for (let i = 0; i < MAX_INTERFACE_COUNTER; i++) {
            var itfOption = this.dataModel.builder(i);
            if (!itfOption) {
                break;
            }
            new AInterface(this, itfOption);
        }

        // 添加到场景并设置位置
        flowView.addNode(this, x, y);
    }

    /**
     * 元素被移除时调用
     */
    protected disconnectedCallback() {
        this._propagationData();
    }

    private _propagationData() {
        // this.dataModel.forEach((dm) => {
        //     dm.dataModel.outputData();
        // });
    }

    public onSelected(): void {
        throw new Error('Method not implemented.');
    }
    public onDeselected(): void {
        throw new Error('Method not implemented.');
    }
    public onHovered(): void {
        throw new Error('Method not implemented.');
    }
    public onResize(): void {
        throw new Error('Method not implemented.');
    }
    protected builder(): void {
        throw new Error('Method not implemented.');
    }

    public onMoving(event?: MouseEvent) {
        this.updateConnectionPosition();
    }

    /**
     * 获取推荐宽度
     * @return {number} 返回值为标题的实际像素宽度
     */
    public getImplicitWidth(): number {
        return (this._nodeTitle.innerText as any).realTextWidth($(this._nodeTitle).css('font'));
    }

    /**
     * 更新连线的位置
     */
    public updateConnectionPosition() {
        this.getInterfaces().forEach((i) => {
            if (i.getPort(EPortType.INPUT)) {
                i.getPort(EPortType.INPUT)?.connections.forEach((conn: AConnection) => {
                    conn._update();
                });
            }
            if (i.getPort(EPortType.OUTPUT)) {
                i.getPort(EPortType.OUTPUT)?.connections.forEach((conn: AConnection) => {
                    conn._update();
                });
            }
        });
    }

    /**
     * 设置节点标题
     * @param {string} name 标题字符串
     */
    public setTitle(name: string) {
        this._nodeTitle.innerHTML = name;
    }

    /**
     * 获得节点标题显示的文字
     */
    public getTitle() {
        return this._nodeTitle.innerHTML;
    }

    /**
     * 设置节点头部颜色
     * @param {string} color 节点头部的颜色，
     * @example
     *  setTitleColor("#123456");
     *  setTitleColor("rgb(255,255,255)")
     * @warning 尽量不要设置透明alpha通道
     */
    public setTitleColor(color: string) {
        this._nodeTitle.style.backgroundColor = color;
    }

    public getTitleColor() {
        return this._nodeTitle.style.backgroundColor;
    }

    /** 获得节点在场景中的位置
     * @returns 返回坐标对象{x,y}
     */
    public getPosition() {
        return {
            x: parseInt(this.style.left),
            y: parseInt(this.style.top),
        };
    }

    /**
     * 设置节点在视图中的位置
     * @param {number} x x坐标
     * @param {number} y y坐标
     */
    public setPosition(x: number, y: number) {
        /* 设置位置 */
        this.style.left = x + 'px';
        this.style.top = y + 'px';
    }

    /**
     * 增加节点在视图中的位置
     * @param {number} dx x增量
     * @param {number} dy y增量
     * @note 等价于 new_position = old_position + (dx, dy)
     */
    public addPosition(dx: number, dy: number) {
        this.style.left = parseInt(this.style.left) + dx + 'px';
        this.style.top = parseInt(this.style.top) + dy + 'px';
    }

    /**
     * 添加接口
     * @param {AInterface} itf 要添加的接口
     */
    public addInterface(itf: AInterface) {
        try {
            this._nodeContent.childNodes.forEach((i) => {
                if (i === itf) {
                    throw new Error('接口已经存在，无法重复添加！');
                }
            });
            this._nodeContent.appendChild(itf);
        } catch (e) {
            console.log(e);
        }
    }

    /**
     * 获得所有的接口
     * @returns {AInterface[]} 接口数组
     */
    public getInterfaces() {
        const interfaces: AInterface[] = [];
        this._nodeContent.childNodes.forEach((i) => {
            if (i instanceof AInterface) {
                interfaces.push(i);
            }
        });
        return interfaces;
    }

    /**
     * 获得节点当前所在的视图
     * @returns {AFlowView} 返回视图类
     */
    public getFlowView(): AFlowView | null {
        let fv = null;
        $.each($(this).parents(), function (_: any, p: any) {
            if (p instanceof AFlowView) {
                fv = p;
                return;
            }
        });
        return fv;
    }

    /**
     * 在节点末尾处追加一些有用的提示文本
     * @param {Object} option 内容对象。可用参数示例：
     *
     * ### content: string
     *  提示的文本内容，支持html格式
     *
     * ### type: string
     *  提示类型，支持一下参数
     *  * default（默认）
     *  * alert
     *  * info
     *  * warning
     *
     * ### delay: string | number
     *  延迟关闭时间。单位为ms，同时支持以下参数
     *  * short = 1500ms （默认）
     *  * long = 3000ms
     * @return {ANode} 返回自身节点，即可以再次使用该方法
     * //TODO: 在每个文本后添加删除按钮
     */
    public pushPromptText(option: any) {
        if (typeof option === 'object') {
            let prompt = $('<div class="node-prompt"></div>');
            prompt.html(option.content);
            prompt.prepend($('<i class="fa fa-exclamation-triangle" aria-hidden="true"></i>'));

            if (typeof option.type == 'string') {
                prompt.addClass('bg-' + option.type);
            }

            let timeout = 1500;
            if (option.delay === 'long') {
                timeout = 3000;
            } else if (isNaN(parseInt(option.delay))) {
                timeout = 1500;
            }

            setTimeout(() => {
                prompt.remove();
                prompt = null;
            }, timeout);

            this._nodeContent.appendChild(prompt[0]);
        }

        return this;
    }
}
