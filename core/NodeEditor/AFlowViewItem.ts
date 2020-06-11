import AFlowView from './AFlowView';

export interface IViewItemEvent {
    /**
     * 当item被选择
     */
    onSelected(): void;

    /**
     * 当item被取消选择
     */
    onDeselected(): void;

    /**
     * 当鼠标悬停在item上
     */
    onHovered(): void;

    /**
     * 当移动item
     */
    onMoving(event?: MouseEvent): void;

    /**
     * 当item尺寸发生改变
     */
    onResize(): void;
}

export abstract class AbstractFlowViewItem extends HTMLElement implements IViewItemEvent {
    constructor(flowView: AFlowView) {
        super();
    }

    public abstract onSelected(): void;
    public abstract onDeselected(): void;
    public abstract onHovered(): void;
    public abstract onMoving(event?: MouseEvent): void;
    public abstract onResize(): void;

    /**
     * 构造元素的抽象方法
     */
    protected abstract builder(): void;
}
