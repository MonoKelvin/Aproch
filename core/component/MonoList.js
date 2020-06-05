$.fn.MonoList = function (options) {
    this.selectable = options && options.selectable !== undefined ? Boolean(options.selectable) : true;
    this.multiSelect = options && options.multiSelect !== undefined ? Boolean(options.multiSelect) : false;
    this.draggable = options && options.draggable !== undefined ? Boolean(options.draggable) : false;
    this.delegate = options && options.delegate ? options.delegate : null;
    this.currentIndex = options && options.currentIndex !== undefined ? options.currentIndex : -1;
    this.onContextMenu = options && options.onContextMenu ? options.onContextMenu : null;
    this.onItemClicked = options && options.onItemClicked ? options.onItemClicked : null;
    this.onItemDoubleClicked = options && options.onItemDoubleClicked ? options.onItemDoubleClicked : null;

    // 只有delegate不为空设置item数量才有效
    this.itemCount = options && !isNaN(parseInt(options.itemCount)) ? Number(options.itemCount) : 0;

    // forcedSelect | repeatClick | noFocus
    this.deselectType = options && options.deselectType ? options.deselectType : 'repeatClick';

    /** Methods */

    this.updateIndexForView = function () {
        items.html(delegate.html().replace(/\${index}/, i));
    };

    this.getSelectedItems = function (isGetJqueryObj = false) {
        var items = new Set();
        if (isGetJqueryObj) {
            THIS.children('li, .list-item').each(function () {
                if ($(this).hasClass('list-item-selected')) {
                    items.add($(this));
                }
            });
        } else {
            THIS.children('li, .list-item').each(function () {
                if ($(this).hasClass('list-item-selected')) {
                    items.add(this);
                }
            });
        }
        return Array.from(items);
    };

    var THIS = $(this);
    var t = this;

    // Delegate
    var delegate = null;
    if (this.delegate[0] == '#') {
        delegate = $(this.delegate);
    } else {
        delegate = $('#' + this.delegate);
    }
    if (delegate && delegate.length > 0) {
        const html = delegate.html();
        for (let i = 0; i < this.itemCount; i++) {
            let item = $('<li></li>').html(html.replace(/\${index}/, i));
            THIS.append(item);
        }
    }

    // Set Current Index
    if (!isNaN(parseInt(this.currentIndex))) {
        if (this.currentIndex >= 0 && this.currentIndex < THIS.children.length) {
            THIS.children[this.currentIndex].addClass('list-item-selected');
        }
    }

    var items = THIS.children('li, .list-item');

    // Context Menu
    THIS.on('contextmenu', (e) => {
        if (this.onContextMenu) {
            e.preventDefault();
            this.onContextMenu(e);
        }
    });

    // Item Clicked Event
    items.on('click', function () {
        var item = $(this);

        do {
            if (t.multiSelect || t.deselectType === 'repeatClick') {
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

        if (t.onItemClicked) {
            t.onItemClicked(this);
        }
    });

    // Item Double Clicked Event
    items.on('dblclick', function () {
        if (t.onItemDoubleClicked) {
            t.onItemDoubleClicked(this);
        }
    });

    return this;
};
