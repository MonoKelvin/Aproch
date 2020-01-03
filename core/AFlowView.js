class AFlowView extends HTMLElement {
    /** 创建一个指定name的节点编辑视图控件
     * @param {string} name 该视图的名称
     */
    constructor(name) {
        super();

        this.name = this.getAttribute('name') ? this.getAttribute('name') : name;

        /** 场景中所有的节点 */
        this.nodes = [];

        /** 场景中选择的节点 */
        this.selectedNodes = [];

        /** 注册事件 */
        this.onmousedown = function(evt) {
            let oldX = parseInt($(this).css('left'));
            let oldY = parseInt($(this).css('top'));

            // 按下shift键就多选
            if (evt.shiftKey == 1) {
                var selList = [];
                var fileNodes = document.getElementsByTagName('div');
                for (let i = 0; i < fileNodes.length; i++) {
                    if (fileNodes[i].className.indexOf('node-widget') != -1) {
                        fileNodes[i].className = 'node-widget';
                        selList.push(fileNodes[i]);
                    }
                }

                var isSelect = true;
                // var evt = window.event || arguments[0];
                var startX = evt.offsetX; //.clientX;
                var startY = evt.offsetY; //.clientY;
                console.log('evt :', evt);
                var selDiv = document.createElement('div');

                selDiv.style.cssText =
                    'position:absolute;width:0px;height:0px;font-size:0px;margin:0px;padding:0px;border:1px dashed #AAA;background-color:#333;z-index:1000;filter:alpha(opacity:60);opacity:0.6;display:none;';

                this.append(selDiv);
                selDiv.style.left = startX + 'px';
                selDiv.style.top = startY + 'px';

                var _x = null;
                var _y = null;
                clearEventBubble(evt);

                this.onmousemove = function(evt) {
                    // evt = window.event || arguments[0];

                    if (isSelect) {
                        if (selDiv.style.display == 'none') {
                            selDiv.style.display = '';
                        }
                        _x = evt.offsetX;
                        _y = evt.offsetY;
                        selDiv.style.left = Math.min(_x, startX) + 'px';
                        selDiv.style.top = Math.min(_y, startY) + 'px';
                        selDiv.style.width = Math.abs(_x - startX) + 'px';
                        selDiv.style.height = Math.abs(_y - startY) + 'px';

                        // ---------------- 关键算法 ---------------------

                        let _l = selDiv.offsetLeft,
                            _t = selDiv.offsetTop;

                        let _w = selDiv.offsetWidth,
                            _h = selDiv.offsetHeight;

                        for (let i = 0; i < selList.length; i++) {
                            let sl = selList[i].offsetWidth + selList[i].offsetLeft;
                            let st = selList[i].offsetHeight + selList[i].offsetTop;
                            if (
                                sl > _l &&
                                st > _t &&
                                selList[i].offsetLeft < _l + _w &&
                                selList[i].offsetTop < _t + _h
                            ) {
                                if (selList[i].className.indexOf('seled') == -1) {
                                    selList[i].className = selList[i].className + ' seled';
                                }
                            } else {
                                if (selList[i].className.indexOf('seled') != -1) {
                                    selList[i].className = 'node-widget';
                                }
                            }
                        }
                    }
                    clearEventBubble(evt);
                };
            } else {
                this.style.cursor = 'grabbing';
                this.onmousemove = function(em) {
                    em.preventDefault();
                    let x = em.clientX - evt.clientX;
                    let y = em.clientY - evt.clientY;
                    this.style.left = String(oldX + x) + 'px';
                    this.style.top = String(oldY + y) + 'px';
                    return false;
                };
            }

            this.onmouseup = function() {
                this.style.cursor = 'grab';
                this.onmousemove = null;

                isSelect = false;

                if (selDiv) {
                    selDiv.remove();
                    let count = 0;
                    for (let i = 0; i < selList.length; i++) {
                        if (selList[i].className.indexOf('seled') != -1) {
                            count++;
                        }
                    }
                    console.log(selList instanceof ANode);
                }

                selList = null;
                _x = null;
                _y = null;
                selDiv = null;
                startX = null;
                startY = null;
                evt = null;
            };

            return false;
        };
    }

    /**
     * 添加一个节点
     * @param {ANode} node 要加入场景的节点
     */
    addNode(node) {
        if (node === null || !(node instanceof ANode)) {
            throw '节点无效！';
        }

        this.append(node.nodeWidget);
    }

    /**
     * 删除一个节点
     * @param {ANode} node 要删除的节点
     */
    deleteNode(node) {
        if (node === null || !(node instanceof ANode)) {
            console.log('删除了一个无效的节点');
            return;
        }
        node.remove();
        node = null;
    }
}

customElements.define('aproch-flow-view', AFlowView);

var flowView = new AFlowView('#flow-view');