(function() {
    function b(nodeWidgets, moveSettings) {
        this.t = nodeWidgets;
        this.c = nodeWidgets.find('.' + moveSettings.moveableElement).first();
        this.isWidthCanChange = moveSettings.isWidthCanChange;
        this.isHeightCanChange = moveSettings.isHeightCanChange;
        // this.cs = t.find(s.closed).first();
        this.m = false;
        this.s = false;
        this.hide = 5;
        this.size_bg = 20;
        this.init();
    }
    b.prototype = {
        init: function() {
            var t = this;
            t.box_sizing = t.t.css('box-sizing');
            t.top = t.t.offset().top - $(window).scrollTop();
            t.left = t.t.offset().left - $(window).scrollLeft();
            t.height = t.t.outerHeight();
            t.width = t.t.outerWidth();
            t.w_width = $(window).width();
            t.w_height = $(window).height();
            if (t.box_sizing != 'border-box') {
                t.c_width = t.width - t.t.width();
                t.c_height = t.height - t.t.height();
            } else {
                t.c_width = 0;
                t.c_height = 0;
            }
            t.t.css({
                'max-height': t.w_height - t.c_height,
                'max-width': t.w_width - t.c_width,
                position: 'fixed'
            });
            $(window).resize(function() {
                t.w_width = $(window).width();
                t.w_height = $(window).height();
                if (t.box_sizing != 'border-box') {
                    t.c_width = t.width - t.t.width();
                    t.c_height = t.height - t.t.height();
                } else {
                    t.c_width = 0;
                    t.c_height = 0;
                }
                if (t.t.width() > t.w_width - t.c_width) {
                    t.t.width(t.w_width - t.c_width);
                }
                if (t.t.height() > t.w_height - t.c_height) {
                    t.t.height(t.w_height - t.c_height);
                }
                t.t.css({ 'max-height': t.w_height - t.c_height, 'max-width': t.w_width - t.c_width });
                t.height = t.t.outerHeight();
                t.width = t.t.outerWidth();
                if (t.width + t.left >= t.w_width) {
                    t.left = t.w_width - t.width;
                    if (parseInt(t.t.css('left')) < 0) {
                        t.t.css('left', -t.width + t.hide);
                    } else if (parseInt(t.t.css('left')) > t.w_width - t.width) {
                        t.t.css('left', t.w_width - t.hide);
                    } else {
                        t.t.css('left', t.left);
                    }
                }
                if (t.height + t.top >= t.w_height) {
                    t.top = t.w_height - t.height;
                    if (parseInt(t.t.css('top')) < 0) {
                        t.t.css('top', -t.height + t.hide);
                    } else {
                        t.t.css('top', t.top);
                    }
                }
            });
            t.move();
            t.size();
        },
        move: function() {
            var t = this;
            // t.cs.on('mousedown', function() {
            //     t.top = 0;
            //     t.left = 0;
            //     t.t.animate({ top: 0, left: 0 }, 300, function() {
            //         t.top_animate();
            //     });
            //     return false;
            // });
            t.t.on('mousedown', () => {
                return false;
            });
            t.c.on('mousedown', function(e) {
                t.m = true;
                t.x = e.pageX;
                t.y = e.pageY;
                t.height = t.t.outerHeight();
                t.width = t.t.outerWidth();
                $(document).on('mousemove', function(e) {
                    t.left2 = t.left + e.pageX - t.x;
                    t.top2 = t.top + e.pageY - t.y;
                    // 可以控制元素只在视图范围内
                    /*if (t.left2 <= -t.width) {
                        t.left2 = -t.width;
                    } else if (t.left2 >= t.w_width) {
                        t.left2 = t.w_width;
                    }
                    if (t.top2 <= -t.height) {
                        t.top2 = -t.height;
                    } else if (t.top2 >= t.w_height) {
                        t.top2 = t.w_height;
                    }*/
                    t.t.css({ top: t.top2, left: t.left2 });
                    return false;
                });
                $(document).on('mouseup', function(e) {
                    t.top = t.t.offset().top - $(window).scrollTop();
                    t.left = t.t.offset().left - $(window).scrollLeft();
                    $(document).off('mousemove');
                    $(document).off('mouseup');
                    t.m = false;
                });
                return false;
            });
        },
        size: function() {
                if (!this.isWidthCanChange && !this.isHeightCanChange) {
                    return;
                }
                var t = this;
                t.resizeIndicator = document.createElement('span');
                t.resizeIndicator.setAttribute('class', 'resize-indicator');
                t.t.append(t.resizeIndicator);
                t.resizeIndicator.onmousedown = function(e) {
                    t.s = true;
                    t.old_width = t.t.width();
                    t.old_size_x = e.pageX;
                    t.old_size_y = e.pageY;
                    $(document).on('mousemove', function(e) {
                        t.new_width = e.pageX - t.old_size_x + t.old_width;
                        t.t.width(t.new_width);
                        if (t.t.outerWidth() + t.left >= t.w_width) {
                            t.t.width(t.w_width - t.left - t.c_width);
                        }
                        return false;
                    });
                    $(document).on('mouseup', function() {
                        t.width = t.t.outerWidth();
                        t.s = false;
                        $(document).off('mousemove');
                        $(document).off('mouseup');
                    });
                    return false;
                };
            }
            // leave: function(xx, yy) {
            //     var t = this;
            //     if (
            //         xx >= t.t.offset().left &&
            //         xx <= t.t.offset().left + t.width &&
            //         yy >= t.t.offset().top &&
            //         yy <= t.t.offset().top + t.height
            //     ) {
            //         return false;
            //     } else {
            //         return true;
            //     }
            // }
    };

    /** 移动相关设置 */
    var moveSettings = {
        /** 可以移动部分的元素名称，用class名 */
        moveableElement: 'node-title',

        /** 是否可以改变宽度 */
        isWidthCanChange: true,

        /** 是否可以改变高度 */
        isHeightCanChange: false
    };

    /** 注册移动组件
     * @note 针对类名来使用，如果只想为一个有id的节点添加，建议使用 @see addMoveComponent();
     */
    $.fn.registryMoveComponent = function() {
        $.extend(moveSettings, 'node-title');
        $(this).each(function() {
            new b($(this), moveSettings);
        });
    };

    /** 注册移动组件
     * @note 针对id名来使用，如果想为大量的class为node-widget添加，建议使用 @see registryMoveComponent();
     */
    $.fn.addMoveComponent = function() {
        $.extend(moveSettings, 'node-title');
        new b($(this), moveSettings);
    };
})(jQuery);