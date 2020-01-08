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

    customElements.define('aproch-label-widget', ALabelWidget);
    customElements.define('aproch-input-number', AInputNumberWidget);
    customElements.define('aproch-flow-view', AFlowView);
    customElements.define('aproch-node', ANode);
    customElements.define('aproch-interface', AInterface);
    customElements.define('aproch-port', APort);
    customElements.define('aproch-connection', AConnection);

    //var FVManager = [];
    const CurrentFV = document.querySelector('#fv_1');
    const NodesTable = new ANodeModelRegistry();

    NodesTable.registryDataModel('OutDataModel', 'MyCategory');
    NodesTable.createNode('OutDataModel', 'MyCategory', CurrentFV);
});
