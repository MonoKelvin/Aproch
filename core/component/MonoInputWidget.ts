import AbstractWidget from './MonoWidget';

export const enum EInputType {
    Int,
    Float,
    Number,
    String,
    Text,
    Bool,
    Custom,
}

interface IInputEvent {
    onComplete(): void;
    onValueChanged(): void;
    validate(): boolean;
}

export default class MonoInputWidget extends AbstractWidget implements IInputEvent {
    /**
     * 输入控件的类型
     */
    public type: EInputType = EInputType.Custom;

    /**
     * 占位内容
     */
    public placeholder: string | number = '';

    /**
     * 默认内容，当还原控件内容时就会被设置为该值
     */
    protected readonly mDefaultValue: string | number;

    constructor(label: string, defaultValue: string | number = '') {
        super(label);

        this.mDefaultValue = defaultValue;

        this.setAttribute('class', 'input-widget-container');

        /* 设置输入控件属性 */
        this.widget = document.createElement('input');
        this.widget.setAttribute('class', 'input-widget');
        this.widget.defaultValue = defaultValue;
    }

    onComplete(): void {
        throw new Error('Method not implemented.');
    }
    onValueChanged(): void {
        throw new Error('Method not implemented.');
    }
    validate(): boolean {
        throw new Error('Method not implemented.');
    }
}

export class MonoInputNumberWidget extends MonoInputWidget {
    constructor(label: string, defValue = 0, minValue = -Number.MAX_VALUE, maxValue = Number.MAX_VALUE) {
        super(label, defValue);

        this.widget.min = minValue;
        this.widget.max = maxValue;
        this.widget.value = 0;
        this.widget.type = 'number';

        /* 设置事件 */
        this.widget.oninput = function () {
            this.value = this.value.replace(/[^\-?\d.]/g, '');
        };
        this.addEventListener('change', () => {
            if (this.widget.value.trim() === '') {
                this.widget.value = 0;
            }
            this.onComplete();
            this.widget.blur();
        });

        /* 添加元素 */
        this.append(this.label);
        this.append(this.widget);
    }
}
