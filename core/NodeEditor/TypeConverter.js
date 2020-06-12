var ABaseTypeConverter = (function () {
    function ABaseTypeConverter() {
    }
    ABaseTypeConverter.prototype.canConvert = function (itf1, itf2) {
        throw new Error('Method not implemented.');
    };
    ABaseTypeConverter.prototype.convertTo = function (dm1) {
        throw new Error('Method not implemented.');
    };
    return ABaseTypeConverter;
}());
export { ABaseTypeConverter };
