import ANode from './ANode';
import APort, { EPortType } from './APort';

export declare interface IInterfaceOption {
    ui: any;
    isInPort: boolean;
    isOutPort: boolean;
}

export default class AInterface extends HTMLElement {
    /**
     * 输入端口（左侧）
     */
    private _inPort: APort | null;

    /**
     * 输出端口（右侧）
     */
    private _outPort: APort | null;

    constructor(node: ANode, options: IInterfaceOption) {
        super();

        this._inPort = null;
        this._outPort = null;

        this.id = 'itf_' + NodeIDGenerator + '_' + InterfaceIDGenerator++;
        this.setAttribute('class', 'a-interface');

        node.addInterface(this);

        this.setPort(options.isInPort, options.isOutPort);
        this.append(options.ui);
    }

    protected disconnectedCallback() {
        this.removePort(true, true);
    }

    /**
     * 移除输入端口，没有输入端口则什么都不做
     * @param {boolean} isInPort 是否要移除输入端口（左侧）
     * @param {boolean} isOutPort 是否要移除输出端口（右侧）
     */
    public removePort(isInPort: boolean, isOutPort: boolean) {
        if (isInPort && this._inPort) {
            this._inPort.remove();
            this._inPort = null;
        }
        if (isOutPort && this._outPort) {
            this._outPort.remove();
            this._outPort = null;
        }
    }

    /**
     * 设置输出输出端口
     * @param {boolean} isInPort 是否添加输入端口
     * @param {boolean} isOutPort 是否添加输出端口
     * @note 当存在输入或输出端口时，可以设置为false以移除端口
     * @see removePort();
     */
    public setPort(isInPort: boolean, isOutPort: boolean) {
        if (isInPort && !this._inPort) {
            this._inPort = new APort(EPortType.INPUT);
            this.append(this._inPort);
            if (!isOutPort) {
                $(this).addClass('interface-in');
            }
        }
        if (isOutPort && !this._outPort) {
            this._outPort = new APort(EPortType.OUTPUT);
            this.append(this._outPort);
            if (!isInPort) {
                $(this).addClass('interface-out');
            }
        }

        this.removePort(!isInPort, !isOutPort);
    }

    /** 获得端口
     * @param {EPortType} type 端口类型
     * @returns {APort} port 返回端口，若无端口则返回空null
     */
    public getPort(type: EPortType) {
        if (type === EPortType.INPUT) {
            return this._inPort;
        }
        if (type === EPortType.OUTPUT) {
            return this._outPort;
        }
        return null;
    }

    public getNode() {
        return this.offsetParent as ANode;
    }
}
