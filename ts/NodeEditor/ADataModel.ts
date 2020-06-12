import AbstractWidget from '../Component/MonoWidget';
import { MonoInputNumber } from '../Component/MonoInputWidget';
import MonoLabel from '../Component/MonoLabel';

export default abstract class AbstractDataModel {
    /* 数据变量名，一般各个数据模型的值都不一样 */
    // variableName:string;

    /* 数据名，显示在节点标题的部分 */
    public readonly name: string = '';

    /** 数据  */
    protected dataWidget: AbstractWidget[] = [];

    constructor(name: string) {
        this.name = name;
    }

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
    abstract builder(index: number): any;

    abstract calculate(index: number): void;

    abstract inputData(index: number, data: any): void;

    abstract outputData(index: number): void;

    setDataWidget(index: number, widget: AbstractWidget) {
        this.dataWidget[index] = widget;
    }

    getDataWidget(index: number) {
        return this.dataWidget[index];
    }
}

export class DM_SimpleOut extends AbstractDataModel {
    constructor(name: string) {
        super(name);
        this.dataWidget = [new MonoInputNumber('输出')];
    }

    builder(index: number): any {
        switch (index) {
            case 0:
                return {
                    ui: this.dataWidget[0],
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

export class DM_SimpleIn extends AbstractDataModel {
    constructor(name: string) {
        super(name);
        this.dataWidget = [new MonoLabel('输入')];
    }

    builder(index: number) {
        switch (index) {
            case 0:
                return {
                    ui: this.dataWidget,
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
