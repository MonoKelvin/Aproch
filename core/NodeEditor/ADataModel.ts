import { MonoInputNumber } from '../component/MonoInputWidget';
import { MonoLabel } from '../component/MonoLabel';

export default interface IDataModel {
    /* 数据变量名，一般各个数据模型的值都不一样 */
    // variableName:string;

    /* 数据名，显示在节点标题的部分 */
    name: string;

    /**
     * 构建器，返回类型必须时空值`null`或者一个`对象`，该对象定义为：
     * @param {number} index 从上到下，从0开始对每一个接口的编号
     * @example
     *  this.builder = index => {
     *      switch (index) {
     *      case 0:
     *          return {
     *              ui: new AInputNumberWidget('input'),
     *              inPort: true,
     *              outPort: true
     *          };
     *          break;
     *      case 1:
     *          return {
     *              ui: new ALabelWidget('label'),
     *              inPort: false,
     *              outPort: true
     *          };
     *          break;
     *      default:
     *          break;
     *      }
     *      return null;
     *  }
     */
    builder(index: number): any;

    calculate(index: number): void;

    inputData(index: number, data: any): void;

    outputData(index: number): void;
}

export class OutDataModel implements IDataModel {
    public name: string;

    public inputWidget: MonoInputNumber = new MonoInputNumber('输出');

    constructor() {
        this.name = 'Output Model';
    }

    builder(index: number): any {
        switch (index) {
            case 0:
                return {
                    ui: this.inputWidget,
                    isInPort: false,
                    isOutPort: true,
                };
            default:
                break;
        }
        return null;
    }
    calculate(index: number): void {
        throw new Error('Method not implemented.');
    }
    inputData(index: number, data: any): void {
        throw new Error('Method not implemented.');
    }
    outputData(index: number): void {
        throw new Error('Method not implemented.');
    }
}

export class InDataModel implements IDataModel {
    public name: string;

    public label: MonoLabel = new MonoLabel('输入');

    constructor() {
        this.name = 'Input Model';
    }

    builder(index: number) {
        switch (index) {
            case 0:
                return {
                    ui: this.label,
                    isInPort: true,
                    isOutPort: false,
                };
            default:
                break;
        }
        return null;
    }
    calculate(index: number): void {
        throw new Error('Method not implemented.');
    }
    inputData(index: number, data: any): void {
        throw new Error('Method not implemented.');
    }
    outputData(index: number): void {
        throw new Error('Method not implemented.');
    }
}
