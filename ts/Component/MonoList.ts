import { setOverflowTooltip } from '../Utilities.js';
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

export interface IMonoListOption {
    readonly ele: string;
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

    onContextMenu?: IItemEventFunc;
    onItemClicked?: IItemEventFunc;
    onItemDoubleClicked?: IItemEventFunc;
}

export default class MonoList {
    /** jquery 元素 */
    private readonly _jqEle: JQuery;

    public selectable?: boolean = true;
    public multiSelect?: boolean = false;
    public draggable?: boolean = false;
    public delegate: any = null;
    public itemCount: number = 0;
    public currentIndex?: number = -1;
    public deselectType?: EDeselectType = EDeselectType.NoDeselect;

    constructor(option: IMonoListOption) {
        this._jqEle = $(option.ele);
        if (!this._jqEle) {
            return;
        }

        option.selectable != undefined && (this.selectable = option.selectable);
        option.multiSelect != undefined && (this.multiSelect = option.multiSelect);
        option.draggable != undefined && (this.draggable = option.draggable);
        option.currentIndex != undefined && (this.currentIndex = option.currentIndex);
        option.itemCount != undefined && (this.itemCount = option.itemCount);
        option.delegate && (this.delegate = option.delegate);

        const jqEle = this._jqEle;
        const THIS = this;

        this._jqEle.on('contextmenu', function (e) {
            e.preventDefault();
            option.onContextMenu?.call(THIS, e, THIS._getListItemFromElement(e.target));
        });

        this.getAllJqueryItems().on('click', function (e) {
            var item = $(this);

            do {
                if (THIS.multiSelect || THIS.deselectType == EDeselectType.RepeatClick) {
                    if (item.hasClass('mono-list-item-selected')) {
                        item.removeClass('mono-list-item-selected');
                        break;
                    }
                }

                if (!THIS.multiSelect) {
                    jqEle.children('li').removeClass('mono-list-item-selected');
                }

                if (THIS.selectable) {
                    item.addClass('mono-list-item-selected');
                }
            } while (0);

            option.onItemClicked?.call(THIS, e, THIS._getListItemFromElement(e.target));
        });

        this.getAllJqueryItems().on('dblclick', function (e) {
            option.onItemDoubleClicked?.call(THIS, e, THIS._getListItemFromElement(e.target));
        });

        this._updateItemsSize();

        this.setCurrentIndex(this.currentIndex as number);
    }

    /**
     * 更新item尺寸
     */
    private _updateItemsSize(): void {
        const items = this._jqEle.children('li');
        items.each((_, item) => {
            const badge = item.querySelector('.badge-num');
            if (badge) {
                item.style.paddingRight = $(badge).outerWidth(true) + 'px';
            }
            const text = item.querySelector('.list-item-text');
            if (text) {
                setOverflowTooltip(item, text as HTMLElement);
            }
        });
    }

    private _updateIndexForView() {
        this._jqEle.children('li, .mono-list-item').each((i) => {
            this.getAllJqueryItems().html(this.delegate.innerHTML.replace(/\${index}/, i.toString()));
        });
    }

    private _getListItemFromElement(target: Element) {
        // 如果本身就是list-item就返回
        if (target.tagName == 'li' || target.classList.contains('mono-list-item')) {
            return target;
        }

        return (target as HTMLElement).offsetParent;
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
        const c = this._jqEle.children('li, mono-list-item');
        if (index >= 0 && index < c.length) {
            this.currentIndex = index;
            c.removeClass('mono-list-item-selected');
            c[index]?.classList.add('mono-list-item-selected');
        }
    }

    public getSelectedItems() {
        return this._jqEle.children('.mono-list-item-selected');
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

    onContextMenu(event?: any, item?: any): void {}
    onItemClicked(event?: any, item?: any): void {
        console.log(event);
        console.log(item);
    }
    onItemDoubleClicked(event?: any, item?: any): void {}
}
