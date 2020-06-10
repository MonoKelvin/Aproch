export const enum EWidgetType {
    Label,
    Input,
    Check,
    Vector,
    Matrix,
    Image,
    File,
}

export default abstract class AbstractWidget extends HTMLElement {
    /**
     * 控件标签
     */
    protected mLabel: HTMLElement = document.createElement('span');

    /**
     * 提示内容
     */
    public toolTip: string = '';

    /**
     * 控件包含的具体元素
     */
    public widget: any = null;

    constructor(name: string) {
        super();

        this.mLabel.innerHTML = name;

        this.mLabel.classList.add('node-label');
    }

    public set label(name: string) {
        this.mLabel.innerHTML = name;
    }

    public get label(): string {
        return this.mLabel.innerHTML;
    }
}
