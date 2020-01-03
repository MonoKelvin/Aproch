class AFlowView {
    /** 创建一个或所有指定id或类别的节点编辑视图控件
     * @param {string} view 视图的id或者class字符串
     * @note 如果时id则必须使用`'#id'`，如果时class则必须使用`'.class'`
     */
    constructor(view) {
        /** 视图元素 */
        this.flowView = $(view);

        /** 注册事件 */
        this.flowView.each(function() {
            this.onmousedown = function(ed) {
                ed.preventDefault();
                this.style.cursor = 'grabbing';
                let oldX = parseInt($(this).css('left'));
                let oldY = parseInt($(this).css('top'));
                this.onmousemove = function(em) {
                    em.preventDefault();
                    let x = em.clientX - ed.clientX;
                    let y = em.clientY - ed.clientY;
                    this.style.left = String(oldX + x) + 'px';
                    this.style.top = String(oldY + y) + 'px';
                    return false;
                };
                return false;
            };
            this.onmouseup = function() {
                this.style.cursor = 'grab';
                this.onmousemove = null;
            };
        });
    }

    /**
     * 添加一个节点
     * @param {ANode} node 要加入场景的节点
     */
    addNode(node) {
        if (node === null || !(node instanceof ANode)) {
            throw '节点无效！';
        }

        this.flowView.append(node.nodeWidget);
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
        node.nodeWidget.remove();
        node = null;
    }
}

var flowView = new AFlowView('.flow-view');