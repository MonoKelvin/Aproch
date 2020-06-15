var MonoList = (function () {
    function MonoList(option) {
        var _this = this;
        this.selectable = true;
        this.multiSelect = false;
        this.draggable = false;
        this.delegate = null;
        this.itemCount = 0;
        this.currentIndex = -1;
        this.deselectType = 2;
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
        this._jqEle.on('contextmenu', function (e) {
            e.preventDefault();
            _this.onContextMenu(e);
        });
        var THIS = this._jqEle;
        var t = this;
        this.getAllJqueryItems().on('click', function (e) {
            var item = $(this);
            do {
                if (t.multiSelect || t.deselectType == 0) {
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
    MonoList.prototype.getAllItems = function () {
        return this._jqEle.children('li, .mono-list-item').toArray();
    };
    MonoList.prototype.getAllJqueryItems = function () {
        return this._jqEle.children('li, .mono-list-item');
    };
    MonoList.prototype.setCurrentIndex = function (index) {
        var _a;
        this.currentIndex = index;
        var c = this._jqEle.children();
        if (this.currentIndex >= 0 && this.currentIndex < c.length) {
            this._jqEle.children().removeClass('mono-list-item-selected');
            (_a = c[this.currentIndex]) === null || _a === void 0 ? void 0 : _a.classList.add('mono-list-item-selected');
        }
    };
    MonoList.prototype._updateIndexForView = function () {
        var _this = this;
        this._jqEle.children('li, .mono-list-item').each(function (i) {
            _this.getAllJqueryItems().html(_this.delegate.innerHTML.replace(/\${index}/, i.toString()));
        });
    };
    MonoList.prototype.getSelectedItems = function () {
        return this._jqEle.children('.list-item-selected');
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
    MonoList.prototype.onItemClicked = function (event, item) { };
    MonoList.prototype.onItemDoubleClicked = function (event, item) { };
    return MonoList;
}());
export default MonoList;
