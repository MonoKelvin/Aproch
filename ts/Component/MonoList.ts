/**
 * 取消选择的类型
 */
export const enum EDeselectType {
    /** 重复点击时 */
    RepeatClick,

    /** 当内选择项失去焦点时 */
    NoFocus,

    /** 不可取消选择 */
    NoDeselect,
}

export interface IMonoListItemEventFunc {
    (event?: MouseEvent, item?: any): void;
}

export interface IMonoListOption {
    ele: string;
    selectable?: boolean;
    multiSelect?: boolean;
    draggable?: boolean;
    currentIndex?: number;

    /** 代理 */
    delegate?: any;

    /** 只有delegate不为空，设置itemCount才有效 */
    itemCount?: number;

    // forcedSelect | repeatClick | noFocus
    deselectType?: EDeselectType;

    onContextMenu?: IMonoListItemEventFunc;
    onItemClicked?: IMonoListItemEventFunc;
    onItemDoubleClicked?: IMonoListItemEventFunc;
}

export default class MonoList implements IMonoListOption {
    /** jquery 元素 */
    private readonly _jqEle: JQuery;

    public ele: string;
    public selectable?: boolean = true;
    public multiSelect?: boolean = false;
    public draggable?: boolean = false;
    public delegate: any = null;
    public itemCount: number = 0;
    public currentIndex?: number = -1;
    public deselectType?: EDeselectType = EDeselectType.NoDeselect;

    constructor(option: IMonoListOption) {
        this.ele = option.ele;
        this._jqEle = $(this.ele);
        if (!this._jqEle) {
            return;
        }

        option.selectable != undefined && (this.selectable = option.selectable);
        option.multiSelect != undefined && (this.multiSelect = option.multiSelect);
        option.draggable != undefined && (this.draggable = option.draggable);
        option.currentIndex != undefined && (this.currentIndex = option.currentIndex);
        option.itemCount != undefined && (this.itemCount = option.itemCount);
        option.delegate && (this.delegate = option.delegate);

        this._jqEle.on('contextmenu', (e) => {
            e.preventDefault();
            this.onContextMenu(e);
        });

        const THIS = this._jqEle;
        const t = this;
        this.getAllJqueryItems().on('click', function (e) {
            var item = $(this);

            do {
                if (t.multiSelect || t.deselectType == EDeselectType.RepeatClick) {
                    if (item.hasClass('list-item-selected')) {
                        item.removeClass('list-item-selected');
                        break;
                    }
                }

                if (!t.multiSelect) {
                    THIS.children('li, .list-item').removeClass('list-item-selected');
                }

                if (t.selectable) {
                    item.addClass('list-item-selected');
                }
            } while (0);

            t.onItemClicked(e, this);
        });

        this.getAllJqueryItems().on('dblclick', function (e) {
            if (t.onItemDoubleClicked) {
                t.onItemDoubleClicked(e, this);
            }
        });
    }

    /** 获得所有列表元素 */
    public getAllItems() {
        return this._jqEle.children('li, .mono-list-item').toArray();
    }

    /** 获得所有列表元素，并以jquery对象返回 */
    public getAllJqueryItems() {
        return this._jqEle.children('li, .mono-list-item');
    }

    public setCurrentIndex(index: number) {
        this.currentIndex = index;

        const c = this._jqEle.children();
        if (this.currentIndex >= 0 && this.currentIndex < c.length) {
            this._jqEle.children().removeClass('mono-list-item-selected');
            c[this.currentIndex]?.classList.add('mono-list-item-selected');
        }
    }

    private _updateIndexForView() {
        this._jqEle.children('li, .mono-list-item').each((i) => {
            this.getAllJqueryItems().html(this.delegate.innerHTML.replace(/\${index}/, i.toString()));
        });
    }

    public getSelectedItems() {
        return this._jqEle.children('.list-item-selected');
    }

    public setDelegate(delegate?: string) {
        if (!delegate) {
            this._jqEle.html('');
            return;
        }
        const d = $(delegate);
        if (d.length > 0) {
            for (let i = 0; i < this.itemCount; i++) {
                this._jqEle.append($('<li></li>').html(d.html().replace(/\${index}/, i.toString())));
            }
        } else {
            this._jqEle.html('');
        }
    }

    onContextMenu(event?: MouseEvent, item?: any): void {}
    onItemClicked(event?: MouseEvent, item?: any): void {}
    onItemDoubleClicked(event?: MouseEvent, item?: any): void {}
}
