export class ITypeConverter {
    constructor() {

    }

    static CanConvert(dm1, dm2) {
        return false;
    }

    static ConvertTo(dm1) {
        // return IDataModel();
    }
}

export class ABaseTypeConverter extends ITypeConverter {
    constructor(){
        super();

    }

    static CanConvert() {
        return true;
    }
}
