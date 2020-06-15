$(document).ready(function () {
    'use strict';

    $('.switcher').each(function () {
        this.type = 'checkbox';
    });

    $('.btn-close').each(function () {
        var t = $(this);
        t.html(`<svg class="svg-icon" xmlns="http://www.w3.org/2000/svg" viewBox="0 0 2048 2048" width="14" height="14">
                    <path d="M1115 1024l690 691-90 90-691-690-691 690-90-90 690-691-690-691 90-90 691 690 691-690 90 90-690 691z">
                    </path>
                </svg>`);
        var closeObj = $(t.attr('data-close-id'));
        if (closeObj.length > 0) {
            t.on('click', () => {
                const dis = t.attr('data-destroy');
                if (typeof dis === undefined || dis == false) {
                    closeObj.addClass('display-none');
                } else {
                    closeObj.remove();
                }
            });
        }
    });

    $('[data-menu]').each(function () {
        let mObj = $('#' + this.getAttribute('data-menu'));
        // 菜单存在
        if (mObj.length === 1) {
            const t = $(this);
            t.on('click', () => {
                mObj.css('left', this.offsetLeft + 'px').css('top', this.offsetTop + t.outerHeight() + 'px');
                mObj.removeClass('display-none');
                $(document).bind('mousedown', () => {
                    mObj.addClass('display-none');
                    $(document).unbind('mousedown');
                });
            });
        }
    });

    $('.menu').each(function () {
        // 阻止事件冒泡，防止鼠标按下菜单就关闭
        $(this).mousedown(() => false);
        $(this).click(() => {
            $(this).addClass('display-none');
        });
    });

    $('.menuitem').each(function () {
        var t = $(this);
        if (t.attr('disabled')) {
            t.addClass('disabled no-pointer-event');
        }

        let ctr = t.children();
        if (ctr.hasClass('menuitem-self-control')) {
            ctr.css('z-index', t.css('z-index') + 100);
            ctr.on('click', () => {
                return false;
            });
        }
    });

    $('.slider').each(function () {
        let t = $(this);
        this.max = t.attr('max') == undefined ? 100 : this.max;
        this.min = t.attr('min') == undefined ? 0 : this.min;
        if (t.attr('value') === undefined) {
            this.value = this.min;
        }

        t.attr('type', 'range');
        t.bind('input', () => {
            let v = Math.min(Math.max(this.min, this.value), this.max);
            v = ((v - this.min) / (this.max - this.min)) * 100;

            t.attr('value', v).css('background-image', `linear-gradient(to right, #006cbe ${v}%, #454545 ${v}%`);
        });
        t.trigger('input');
    });

    $('.btn-dropdown').each(function () {
        // 添加箭头
        let arrow = $(document.createElement('i'));
        arrow.addClass('fa fa-angle-down arrow-down');
        arrow.insertAfter(this);

        let curIndex = this.dataset.index;
        var opList = $(this).siblings('.option-list');

        if (curIndex === undefined || this.dataset.index < 0) {
            this.dataset.index = 0;
            curIndex = 0;
        } else {
            this.innerText = '';
        }

        // 弹出下拉框事件
        var popupOptionListFunc = () => {
            opList.removeClass('display-none');
            // 点击其他区域隐藏下拉框
            $(document).bind('mousedown', () => {
                opList.addClass('display-none');
                $(document).unbind('mousedown');
            });
            opList.css('min-width', $(this).outerWidth() + 'px');
        };
        $(this).click(popupOptionListFunc);
        arrow.click(popupOptionListFunc);
        opList.mousedown(() => false);

        // 点击下拉框选项
        opList.children().each((i, op) => {
            // 设置初始选项
            if (i == curIndex) {
                this.dataset.index = i;
                this.innerText = op.innerText;
                this.value = op.innerText;
            }

            $(op).on('click', () => {
                this.dataset.index = i;
                this.innerText = op.innerText;
                this.value = op.innerText;
                opList.addClass('display-none');
            });
        });
    });

    $('.mono-icon-input').each(function () {
        Array.from(this.getElementsByTagName('i')).forEach((i) => {
            const input = $(this).children('input');
            const icon = $(i);
            console.log(icon.css('right'), icon.outerWidth());
            if (icon.hasClass('d-left')) {
                input.css('padding-left', 6 + parseInt(icon.css('left')) + icon.outerWidth() + 'px');
            }
            if (icon.hasClass('d-right')) {
                input.css('padding-right', 6 + parseInt(icon.css('right')) + icon.outerWidth() + 'px');
            }
        });
    });
});
