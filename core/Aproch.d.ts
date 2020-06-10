declare global {
    var $: any;
    declare var NodeIDGenerator: number;
    declare var InterfaceIDGenerator: number;
    declare var PortIDGenerator: number;
    declare var ConnectionIDGenerator: number;
}

// export const PInterfaceOption = {
//     ui: null,
//     inPort: true,
//     outPort: true
// };

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
    onMoving(): void;

    /**
     * 当item尺寸发生改变
     */
    onResize(): void;
}
