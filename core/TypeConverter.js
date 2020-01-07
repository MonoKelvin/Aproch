class ITypeConverter {
    constructor() {
        this.canConvert = () => { return false };
    }
}

class ABaseTypeConverter extends ITypeConverter {
    constructor(){
        super();

        this.canConvert = function() {
            return true;
        };
    }
}