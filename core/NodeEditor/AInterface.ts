/// <reference path="../Types.d.ts" />
import { NodeIDGenerator, InterfaceIDGenerator } from '../Aprochh';

const MAX_INTERFACE_COUNTER = 100;

export default class AInterface extends HTMLElement {
    constructor(node, options) {
        super();

        /** 输入端口（左侧） */
        this.inPort = null;

        /** 输出端口（右侧） */
        this.outPort = null;

        this.id = 'itf_' + NodeIDGenerator + '_' + InterfaceIDGenerator++;
        this.setAttribute('class', 'aproch-interface');

        node.addInterface(this);

        this.setPort(options.isInPort, options.isOutPort);
        this.append(options.ui);
    }

    disconnectedCallback() {
        this.removePort(true, true);
    }

    /**
     * 移除输入端口，没有输入端口则什么都不做
     * @param {boolean} isInPort 是否要移除输入端口（左侧）
     * @param {boolean} isOutPort 是否要移除输出端口（右侧）
     */
    removePort(isInPort, isOutPort) {
        if (isInPort && this.inPort) {
            this.inPort.remove();
            this.inPort = null;
        }
        if (isOutPort && this.outPort) {
            this.outPort.remove();
            this.outPort = null;
        }
    }

    /**
     * 设置输出输出端口
     * @param {boolean} isInPort 是否添加输入端口
     * @param {boolean} isOutPort 是否添加输出端口
     * @note 当存在输入或输出端口时，可以设置为false以移除端口
     * @see removePort();
     */
    setPort(isInPort, isOutPort) {
        if (isInPort && !this.inPort) {
            this.inPort = new APort(EPortType.INPUT);
            this.append(this.inPort);
            if (!isOutPort) {
                $(this).addClass('interface-in');
            }
        }
        if (isOutPort && !this.outPort) {
            this.outPort = new APort(EPortType.OUTPUT);
            this.append(this.outPort);
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
    getPort(type) {
        if (type === EPortType.INPUT) {
            return this.inPort;
        }
        if (type === EPortType.OUTPUT) {
            return this.outPort;
        }
        return null;
    }

    getNode() {
        return this.offsetParent;
    }
}
