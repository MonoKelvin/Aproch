import { setOverflowTooltip } from '../Utilities.js';
var MonoList = (function () {
    function MonoList(option) {
        this.selectable = true;
        this.multiSelect = false;
        this.draggable = false;
        this.delegate = null;
        this.itemCount = 0;
        this.currentIndex = -1;
        this.deselectType = 2;
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
        var jqEle = this._jqEle;
        var THIS = this;
        this._jqEle.on('contextmenu', function (e) {
            var _a;
            e.preventDefault();
            (_a = option.onContextMenu) === null || _a === void 0 ? void 0 : _a.call(THIS, e, THIS._getListItemFromElement(e.target));
        });
        this.getAllJqueryItems().on('click', function (e) {
            var _a;
            var jqItem = $(this);
            do {
                if (THIS.multiSelect || THIS.deselectType == 0) {
                    if (jqItem.hasClass('mono-list-item-selected')) {
                        jqItem.removeClass('mono-list-item-selected');
                        break;
                    }
                }
                if (!THIS.multiSelect) {
                    jqEle.children('li, mono-list-item').removeClass('mono-list-item-selected');
                }
                if (THIS.selectable) {
                    jqItem.addClass('mono-list-item-selected');
                }
            } while (0);
            var item = THIS._getListItemFromElement(e.target);
            if (item) {
                THIS.currentIndex = jqEle.children('li, mono-list-item').index(item);
                (_a = option.onItemClicked) === null || _a === void 0 ? void 0 : _a.call(THIS, e, item);
            }
        });
        this.getAllJqueryItems().on('dblclick', function (e) {
            var _a;
            (_a = option.onItemDoubleClicked) === null || _a === void 0 ? void 0 : _a.call(THIS, e, THIS._getListItemFromElement(e.target));
        });
        this._updateItemsSize();
        this.setCurrentIndex(this.currentIndex);
    }
    MonoList.prototype._updateItemsSize = function () {
        var items = this._jqEle.children('li, .mono-list-item');
        items.each(function (_, item) {
            var badge = item.querySelector('.badge-num');
            if (badge) {
                item.style.paddingRight = $(badge).outerWidth(true) + 'px';
            }
            var text = item.querySelector('.list-item-text');
            if (text) {
                setOverflowTooltip(item, text);
            }
        });
    };
    MonoList.prototype._getListItemFromElement = function (target) {
        if (target.tagName.compareWith('li', false) || target.classList.contains('mono-list-item')) {
            return target;
        }
        return target.offsetParent;
    };
    MonoList.prototype.getAllItems = function () {
        return this._jqEle.children('li, .mono-list-item').toArray();
    };
    MonoList.prototype.getAllJqueryItems = function () {
        return this._jqEle.children('li, .mono-list-item');
    };
    MonoList.prototype.setCurrentIndex = function (index) {
        var _a;
        var c = this._jqEle.children('li, mono-list-item');
        if (index >= 0 && index < c.length) {
            this.currentIndex = index;
            c.removeClass('mono-list-item-selected');
            (_a = c[index]) === null || _a === void 0 ? void 0 : _a.classList.add('mono-list-item-selected');
        }
    };
    MonoList.prototype.getSelectedItems = function () {
        return this._jqEle.children('.mono-list-item-selected');
    };
    MonoList.prototype.setDelegate = function (delegate) {
        if (!delegate) {
            this._jqEle.html('');
            return;
        }
        var d = $(delegate);
        if (d.length > 0) {
            for (var i = 0; i < this.itemCount; i++) {
                this._jqEle.append($('<li></li>').html(d.html().replace(/\${index}/, i.toString())));
            }
        }
        else {
            this._jqEle.html('');
        }
    };
    MonoList.prototype.onContextMenu = function (event, item) { };
    MonoList.prototype.onItemClicked = function (event, item) {
        console.log(event);
        console.log(item);
    };
    MonoList.prototype.onItemDoubleClicked = function (event, item) { };
    return MonoList;
}());
export default MonoList;
