import { AInputNumberWidget, ALabelWidget } from './AWidget.js';

export class IDataModel {
    constructor() {
        /* 数据变量名，一般各个数据模型的值都不一样 */
        // this.variableName = '';

        /* 数据名，显示在节点标题的部分 */
        this.name = '';

        /**
         * UI构建器，返回类型必须时空值`null`或者一个`对象`，该对象定义为：
         * @param {number} index 从上到下，从0开始对每一个接口的编号
         * @example
         *  this.uiBuilder = index => {
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
        this.uiBuilder = index => {};

        this.calculate = (index) => {};

        this.inputData = (index, data) => {};

        this.outputData = index => {};
    }
}

export class OutDataModel extends IDataModel {
    constructor() {
        super();

        this.name = 'Output Model';

        this.inputWidget = new AInputNumberWidget('输出');

        this.inputData = (index, data) => {
            // this.ui.value = data;
        };

        this.outputData = index => { };

        this.calculate = index => {};

        this.uiBuilder = index => {
            switch (index) {
                case 0:
                    return {
                        ui: this.inputWidget,
                        isInPort: false,
                        isOutPort: true
                    };
                default:
                    break;
            }
            return null;
        };
    }
}


export class InDataModel extends IDataModel {
    constructor() {
        super();

        this.name = 'Input Model';

        this.label = new ALabelWidget('输入');

        this.inputData = (index, data) => {
            // this.ui.value = data;
        };

        this.outputData = index => {};

        this.calculate = index => {};

        this.uiBuilder = index => {
            switch (index) {
                case 0:
                    return {
                        ui: this.label,
                        isInPort: true,
                        isOutPort: false
                    };
                default:
                    break;
            }
            return null;
        };
    }
}
