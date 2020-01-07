export const EWidgetType = {
    Label: 'label',
    Input: 'input',
    Check: 'check',
    Vector: 'vector',
    Matrix: 'matrix',
    Image: 'image',
    File: 'file'
};

export const EInputType = {
    Int: 'int',
    Float: 'int',
    Number: 'int',
    String: 'int',
    Text: 'int',
    Bool: 'int'
};

export class IWidget extends HTMLElement {
    constructor() {
        super();

        this.label = null;

        this.widget = null;

        this.label = document.createElement('span');
    }

    /**
     * 设置控件的标签
     * @param {string} content 标签内容
     * @param {boolean} associateControl 是否关联控件来决定样式，如果有其他输入类型控件，则使用true
     */
    setLabel(content, associateControl = true) {
        this.label.innerHTML = content;
        if (associateControl === true) {
            this.label.setAttribute('class', 'control-label');
        } else {
            this.label.setAttribute('class', 'node-label');
        }
    }

    /**
     * 销毁内部控件，在所有子类的disconnectedCallback()中调用
     */
    destroyWidgets() {
        this.label = null;
        delete this.label;

        this.widget = null;
        delete this.widget;
    }
}

export class ALabelWidget extends IWidget {
    constructor(label = '数据源') {
        super();

        this.setAttribute('class', 'input-widget-container');

        this.setLabel(label, false);

        this.append(this.label);
    }

    disconnectedCallback() {
        this.destroyWidgets();
    }
}

export class AInputNumberWidget extends IWidget {
    constructor() {
        super();

        /** 当输入完成时触发的事件 */
        this.onComplete = function () { };
    }

    connectedCallback() {
        this.setAttribute('class', 'input-widget-container');

        this.setLabel('输入');

        this.widget = document.createElement('input');
        this.widget.setAttribute('class', 'input-widget');
        this.widget.value = 0;
        this.widget.max = Number.MAX_VALUE;
        this.widget.min = -Number.MAX_VALUE;
        this.widget.defaultValue = 0;
        this.widget.type = 'number';

        this.widget.oninput = function () {
            this.value = this.value.replace(/[^\-?\d.]/g, '');
        };
        this.addEventListener('change', function () {
            if (this.widget.value.trim() === '') {
                this.widget.value = 0;
            }
            this.onComplete();
            this.widget.blur();
        });

        this.append(this.label);
        this.append(this.widget);
    }

    disconnectedCallback() {
        this.destroyWidgets();
    }

    /** 增删改属性时调用 */
    attributeChangedCallback() {
        console.log('this.widget.value :', this.widget.value);
    }
}