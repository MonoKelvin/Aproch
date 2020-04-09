import { IDataModel, OutDataModel } from '../core/DataModel.js';
import { ALabelWidget, AInputNumberWidget } from '../core/AWidget.js';
import { AFlowView, ANode, AConnection, AInterface, APort} from '../core/Aproch.js';
import ANodeModelRegistry from '../core/ANodeModelRegistry.js';

$(document).ready(function() {
    // 防止按下(F5、Ctrl+R、Ctrl+Shift+R)刷新
    window.onkeydown = function(e) {
        var ev = window.event || e;
        var code = ev.keyCode || ev.which;
        if (code == 82 && (ev.metaKey || ev.ctrlKey)) {
            return false;
        }
    };

    customElements.define('a-label-widget', ALabelWidget);
    customElements.define('a-input-number', AInputNumberWidget);
    customElements.define('a-flow-view', AFlowView);
    customElements.define('a-node', ANode);
    customElements.define('a-interface', AInterface);
    customElements.define('a-port', APort);
    customElements.define('a-connection', AConnection);

    //var FVManager = [];
    const CurrentFV = document.querySelector('#fv_1');
    const NodesTable = new ANodeModelRegistry();

    NodesTable.registryDataModel('OutDataModel', 'MyCategory');
    NodesTable.registryDataModel('InDataModel', 'MyCategory');
    NodesTable.createNode('OutDataModel', 'MyCategory', CurrentFV);
    NodesTable.createNode('OutDataModel', 'MyCategory', CurrentFV, 0, 200);
    NodesTable.createNode('InDataModel', 'MyCategory', CurrentFV, 200, 150);
    NodesTable.createNode('InDataModel', 'MyCategory', CurrentFV, 200, 250);
});
