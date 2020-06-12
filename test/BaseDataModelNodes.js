import AFlowView from '../core/NodeEditor/AFlowView.js';

$(document).ready(function () {
    //var FVManager = [];
    const CurrentFV = document.querySelector('#fv_1');

    AFlowView.NodeTable.createNode('DM_Simple', 'MyCategory', CurrentFV, -200, -150);
    AFlowView.NodeTable.createNode('DM_Simple', 'MyCategory', CurrentFV, -200, 150);
    AFlowView.NodeTable.createNode('InDataModel', 'MyCategory', CurrentFV, 100, -150);
    AFlowView.NodeTable.createNode('InDataModel', 'MyCategory', CurrentFV, 100, 150);
});
