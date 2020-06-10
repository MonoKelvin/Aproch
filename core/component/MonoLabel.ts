import AbstractWidget from './MonoWidget';

export class MonoLabelWidget extends AbstractWidget {
    constructor(label: string = '数据源') {
        super(label);

        this.setAttribute('class', 'input-widget-container');

        this.append(this.mLabel);
    }
}
