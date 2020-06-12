var __extends = (this && this.__extends) || (function () {
    var extendStatics = function (d, b) {
        extendStatics = Object.setPrototypeOf ||
            ({ __proto__: [] } instanceof Array && function (d, b) { d.__proto__ = b; }) ||
            function (d, b) { for (var p in b) if (b.hasOwnProperty(p)) d[p] = b[p]; };
        return extendStatics(d, b);
    };
    return function (d, b) {
        extendStatics(d, b);
        function __() { this.constructor = d; }
        d.prototype = b === null ? Object.create(b) : (__.prototype = b.prototype, new __());
    };
})();
import { addResizeComponent, getElementsByClassName } from '../Utilities';
import ANode from './ANode';
import AConnection from './AConnection';
var EViewEventState;
(function (EViewEventState) {
    EViewEventState[EViewEventState["NoneState"] = 0] = "NoneState";
    EViewEventState[EViewEventState["SelectItem"] = 1] = "SelectItem";
    EViewEventState[EViewEventState["DeselectItem"] = 2] = "DeselectItem";
    EViewEventState[EViewEventState["MultiSelectItem"] = 3] = "MultiSelectItem";
    EViewEventState[EViewEventState["MoveItem"] = 4] = "MoveItem";
})(EViewEventState || (EViewEventState = {}));
var AFlowView = (function (_super) {
    __extends(AFlowView, _super);
    function AFlowView(name) {
        var _this = _super.call(this) || this;
        _this.name = 'Flow View';
        _this.name = _this.getAttribute('name') ? _this.getAttribute('name') : name;
        _this.eventState = EViewEventState.NoneState;
        _this.onwheel = function (evt) {
            var t = $(this);
            if (evt.ctrlKey == true) {
                evt.preventDefault();
            }
        };
        _this.oncontextmenu = function (e) {
            e.preventDefault();
            return false;
        };
        _this.onclick = function (evt) {
            if (evt.target == _this && !evt.shiftKey && !evt.ctrlKey) {
                var selectedNodes = _this.getSelectedItems();
                $(selectedNodes).removeClass('fv-item-slt');
            }
        };
        _this.onmousedown = function (evt) {
            var t = $(_this);
            var tdom = _this;
            var oldX = parseInt(t.offset().left);
            var oldY = parseInt(t.offset().top);
            var startEvtX = evt.clientX;
            var startEvtY = evt.clientY;
            var sfX = null;
            var sfY = null;
            var sfDiv = null;
            var curItem = ANode.GetNodeByChildComponent(evt.target);
            if (curItem && curItem.classList.contains('a-node')) {
                if (!evt.shiftKey && _this.getSelectedItems().length <= 1) {
                    _this.clearSelectedItems();
                }
                curItem.classList.add('fv-item-slt');
                tdom.getSelectedItems().forEach(function (item) {
                    var jqItem = $(item);
                    var left = parseInt(jqItem.css('left'));
                    var top = parseInt(jqItem.css('top'));
                    $(document).on('mousemove', function (e) {
                        jqItem.css({ top: top + e.pageY - startEvtY, left: left + e.pageX - startEvtX });
                        item.onMoving();
                    });
                    $(document).on('mouseup', function () {
                        $(document).off('mousemove');
                        $(document).off('mousedown');
                        $(document).off('mouseup');
                    });
                });
                return;
            }
            sfDiv = $('<div class="selection-frame"></div>');
            $(document).on('mousemove', function (em) {
                if (evt.shiftKey == true) {
                    t.append(sfDiv);
                    sfDiv.css('left', startEvtX + 'px');
                    sfDiv.css('top', startEvtY + 'px');
                    sfX = em.clientX;
                    sfY = em.clientY;
                    sfDiv.css('left', Math.min(sfX, startEvtX) - oldX + 'px');
                    sfDiv.css('top', Math.min(sfY, startEvtY) - oldY + 'px');
                    sfDiv.css('width', Math.abs(sfX - startEvtX) + 'px');
                    sfDiv.css('height', Math.abs(sfY - startEvtY) + 'px');
                    var _l = sfDiv.offset().left, _t = sfDiv.offset().top;
                    var _w = sfDiv.innerWidth(), _h = sfDiv.innerHeight();
                }
                else {
                    t.css('cursor', 'grabbing');
                    var tLeft = oldX + em.clientX - evt.clientX;
                    var tTop = oldY + em.clientY - evt.clientY;
                    if (tLeft > 0) {
                        t.css('left', '0px');
                    }
                    else if (tLeft < t.parent().innerWidth() - t.innerWidth()) {
                        t.css('left', t.parent().innerWidth() - t.innerWidth());
                    }
                    else {
                        t.css('left', tLeft + 'px');
                    }
                    if (tTop > 0) {
                        t.css('top', '0px');
                    }
                    else if (tTop < t.parent().innerHeight() - t.innerHeight()) {
                        t.css('top', t.parent().innerHeight() - t.innerHeight());
                    }
                    else {
                        t.css('top', tTop + 'px');
                    }
                }
                return false;
            });
            $(document).on('mouseup', function () {
                t.css('cursor', 'grab');
                if (sfDiv) {
                    sfDiv.remove();
                }
                sfX = null;
                sfY = null;
                sfDiv = null;
                $(document).off('mousemove');
                $(document).off('mouseup');
            });
            return false;
        };
        _this.addCenterGuidLine();
        _this.moveToOrigin();
        return _this;
    }
    AFlowView.prototype.onItemPressed = function (event) { };
    AFlowView.prototype.onItemsMoving = function (event) { };
    AFlowView.prototype.onBoxSelect = function (event) { };
    AFlowView.prototype.onItemContextMenu = function (event) { };
    AFlowView.prototype.onContextMenu = function (event) { };
    AFlowView.prototype.clearSelectedItems = function () {
        $(this).children('.fv-item-slt').removeClass('fv-item-slt');
    };
    AFlowView.prototype.getSelectedItems = function () {
        return getElementsByClassName(this, 'fv-item-slt');
    };
    AFlowView.prototype.addNode = function (node, x, y) {
        if (x === void 0) { x = 0; }
        if (y === void 0) { y = 0; }
        var offset = this.getOrigin();
        this.append(node);
        node.setPosition(x + offset.x, y + offset.y);
        addResizeComponent(node, function () {
            node.updateConnectionPosition();
        });
    };
    AFlowView.prototype.getNodes = function () {
        return getElementsByClassName(this, 'a-node');
    };
    AFlowView.prototype.deleteNode = function (node) {
        if (node) {
            node.remove();
        }
        else {
            console.log('删除了一个无效的节点');
        }
    };
    AFlowView.prototype.addLinkingConnection = function (sourcePort) {
        var conn = new AConnection(this);
        var p = sourcePort.getPositionInView();
        conn.setStartFixedPoint({
            x: p.x + parseInt(this.style.left),
            y: p.y + parseInt(this.style.top),
        });
        return conn;
    };
    AFlowView.prototype.addCenterGuidLine = function () {
        if ($(this).children('[class^="guid-line"]').length > 0) {
            return;
        }
        $(this).prepend($('<div class="guid-line-h"></div><div class="guid-line-v"></div>'));
    };
    AFlowView.prototype.removeCenterGuidLine = function () {
        $(this).children('[class^="guid-line"]').remove();
    };
    AFlowView.prototype.getOrigin = function () {
        return {
            x: this.offsetWidth / 2,
            y: this.offsetHeight / 2,
        };
    };
    AFlowView.prototype.getOriginOffset = function () {
        return {
            x: (this.offsetLeft + this.offsetWidth) / 2,
            y: (this.offsetTop + this.offsetHeight) / 2,
        };
    };
    AFlowView.prototype.getViewOffset = function () {
        return {
            x: -this.offsetLeft,
            y: -this.offsetTop,
        };
    };
    AFlowView.prototype.viewportToFlowView = function (point) {
        return {
            x: point.x - this.offsetLeft,
            y: point.y - this.offsetTop,
        };
    };
    AFlowView.prototype.moveToOrigin = function () {
        var t = $(this);
        var hw = (t.parent().innerWidth() - t.innerWidth()) / 2, hh = (t.parent().innerHeight() - t.innerHeight()) / 2;
        t.css('left', hw + 'px').css('top', hh + 'px');
    };
    AFlowView.prototype._updateSelectionSet = function (event, curSelectedNode) {
        var node = $(curSelectedNode);
        if (event.ctrlKey == 1 || event.shiftKey == 1) {
            if (node.hasClass('fv-item-slt')) {
                node.removeClass('fv-item-slt');
                return;
            }
        }
        else {
            $(this).children('.a-node').removeClass('fv-item-slt');
        }
        node.addClass('fv-item-slt');
    };
    return AFlowView;
}(HTMLElement));
export default AFlowView;
