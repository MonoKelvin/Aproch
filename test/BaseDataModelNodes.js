import { AFlowView } from '../core/Aproch.js';

$(document).ready(function () {
    //var FVManager = [];
    const CurrentFV = document.querySelector('#fv_1');

    AFlowView.NodeTable.createNode('OutDataModel', 'MyCategory', CurrentFV, -200, -150);
    AFlowView.NodeTable.createNode('OutDataModel', 'MyCategory', CurrentFV, -200, 150);
    AFlowView.NodeTable.createNode('InDataModel', 'MyCategory', CurrentFV, 100, -150);
    AFlowView.NodeTable.createNode('InDataModel', 'MyCategory', CurrentFV, 100, 150);
});