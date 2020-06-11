// 声明为jquery的$符号
declare var $: any;

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
