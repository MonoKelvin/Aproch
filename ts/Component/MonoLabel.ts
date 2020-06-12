import AbstractWidget from './MonoWidget';

export default class MonoLabel extends AbstractWidget {
    constructor(label: string = '数据源') {
        super(label);

        this.setAttribute('class', 'input-widget-container');

        this.append(this.mLabel);
    }
}
