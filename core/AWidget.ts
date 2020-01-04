enum EInputType {
    Int,
    Float,
    Number,
    String,
    Text,
    Bool
}

interface IInputWidget extends HTMLInputElement {
    inputType: EInputType;
}
// customElements.define('aproch-input', IInputWidget);
