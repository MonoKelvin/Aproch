// 节点ID生成器
declare var NodeIDGenerator: number = 0;
// 接口ID生成器
declare var InterfaceIDGenerator: number = 0;
// 端口ID生成器
declare var PortIDGenerator: number = 0;
// 连线ID生成器
declare var ConnectionIDGenerator: number = 0;

// 最大接口数量
declare const MAX_INTERFACE_COUNTER: number = 64;

declare interface Point {
    x: number = 0;
    y: number = 0;
}

declare const enum EPosition {
    LEFT = 0x0001,
    TOP = 0x0002,
    RIGHT = 0x0004,
    BOTTOM = 0x0008,
    HCENTER = 0x0010,
    VCENTER = 0x0020,
}

declare type CategoryTable<T> = {
    [key: string]: T[];
};

/**
 * 全局数据模型注册表
 * @note 默认内置一个`Other`分类
 */
declare var DMRegistryTable: CategoryTable<string> = {
    Other: [],
};

declare interface String {
    realTextWidth(font: string): number;
}

declare interface Array {
    remove(element: any): boolean;
}

declare interface IItemEventFunc {
    (event?: any, item?: any): void;
}
